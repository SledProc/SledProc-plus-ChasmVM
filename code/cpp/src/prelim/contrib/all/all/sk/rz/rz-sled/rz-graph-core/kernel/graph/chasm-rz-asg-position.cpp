
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "chasm-rz-asg-position.h"

#include "kernel/grammar/chasm-rz-graph-build.h"
#include "kernel/graph/chasm-rz-node.h"
#include "kernel/graph/chasm-rz-graph.h"
#include "token/chasm-rz-token.h"

#include "code/chasm-rz-function-def-entry.h"

#include "code/chasm-rz-call-entry.h"
#include "code/chasm-rz-casement-call-entry.h"


#include "code/chasm-rz-block-entry.h"

#include "kernel/grammar/chasm-rz-string-plex-builder.h"

#include "kernel/graph/chasm-rz-connection.h"

#include "rzns.h"

USING_RZNS(RZ_Core)


ChasmRZ_ASG_Position::ChasmRZ_ASG_Position
 (ChasmRZ_Graph_Build* graph_build)
 : Flags(0), graph_build_(graph_build),
   Cf(ChasmRZ_Frame::instance("casement")),
   Sf(ChasmRZ_Frame::instance("semantic")),
   Qy(ChasmRZ_Query::instance()),
   position_state_(Position_States::Root),
   current_node_(nullptr), last_pchasm_rz_entry_node_(nullptr),
   held_equalizer_node_(nullptr), held_retval_node_(nullptr),
   held_retval_follow_node_(nullptr),
   last_function_definition_arrow_node_(nullptr),
   current_function_def_entry_node_(nullptr),
   held_mapkey_node_(nullptr), held_do_mapkey_node_(nullptr),
   type_indicator_node_(nullptr),
   current_do_map_block_entry_node_(nullptr),
   current_block_map_entry_node_(nullptr),
   last_do_map_block_statement_entry_node_(nullptr),
   last_statement_entry_node_(nullptr),
   last_do_map_inner_block_first_entry_node_(nullptr),
   held_equalizer_context_(Equalizer_Contexts::N_A),
   current_closed_do_entry_node_(nullptr),
   held_assignment_annotation_node_(nullptr)
{
}

#define in_Cf Cf,


QPair<ChasmRZ_ASG_Position::Equalizer_Contexts, QString>
ChasmRZ_ASG_Position::parse_equalizer_context(QString key)
{
 static QMap< QString, QPair<Equalizer_Contexts, QString> > static_map
 {{
   {"#", {Equalizer_Contexts::Number, "let_num"}},
   {"$", {Equalizer_Contexts::String, "let_str"}},
   {"%", {Equalizer_Contexts::Float, "let_float"}},
   {"@", {Equalizer_Contexts::Ratio, "let_ratio"}},
   {"*", {Equalizer_Contexts::Pointer, "let_ptr"}},
   {"&", {Equalizer_Contexts::Function, "let_fun"}},
 }};

 return static_map.value(key, {Equalizer_Contexts::N_A, ""});
}


void ChasmRZ_ASG_Position::add_string_plex_node(caon_ptr<ChasmRZ_Node> tinfo_node,
 caon_ptr<ChasmRZ_Node> string_plex_node)
{
 if(caon_ptr<RZ_String_Plex_Builder> rzspb = string_plex_node->string_plex_builder())
 {
  if(caon_ptr<ChasmRZ_Node> tinfo_node = rzspb->tuple_info_node())
  {
   tinfo_node <<Cf/Qy.String_Plex_Value>> string_plex_node;
  }
 }
}


ChasmRZ_Node& ChasmRZ_ASG_Position::get_current_chief()
{
 return *chiefs_.top();
}

void ChasmRZ_ASG_Position::add_residual_node(caon_ptr<ChasmRZ_Node> node)
{
 CAON_PTR_DEBUG(ChasmRZ_Node ,node)
 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)

 &get_current_chief() <<Cf/Qy.Residual_Node>> node;
}

void ChasmRZ_ASG_Position::check_add_implied_my()
{
 switch(position_state_)
 {
  // //  any others?
 case Position_States::Active_Run_Token:
 case Position_States::Active_Run_Chief:
  break;
 default:
  // //  this would be better not as a raw string ...
  graph_build_->add_run_token("my");
  break;
 }

}

caon_ptr<ChasmRZ_Node> ChasmRZ_ASG_Position::insert_casement_entry_node(const ChasmRZ_Connectors& connector, bool is_statement_entry,
 QString prefix)
{
 caon_ptr<ChasmRZ_Casement_Call_Entry> parent_rce = nullptr;
// if(!chiefs_.isEmpty())
// {
//  caon_ptr<ChasmRZ_Node> top = chiefs_.top();
//  parent_rce = top->chasm_rz_call_entry();
// }


 caon_ptr<ChasmRZ_Node> result = graph_build_->new_run_casement_entry_node(is_statement_entry,
   prefix, parent_rce);

 if(is_statement_entry)
 {
  if(current_do_map_block_entry_node_)
  {
   last_do_map_block_statement_entry_node_ = result;
   if(!last_do_map_inner_block_first_entry_node_)
   {
    last_do_map_inner_block_first_entry_node_ = result;
   }
  }
  else
  {
   last_statement_entry_node_ = result;
  }
 }
 check_append_chief(result);
 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
 CAON_PTR_DEBUG(ChasmRZ_Node ,result)

 current_node_ <<Cf/connector>> result;

 if(caon_ptr<ChasmRZ_Token> token = current_node_->chasm_rz_token())
 {
  if(token->raw_text() == "if")
    result->chasm_rz_call_entry()->flags.may_precede_if_block = true;

  //?
  if(token->raw_text() == "elsif")
    result->chasm_rz_call_entry()->flags.may_precede_elsif_block = true;

 }

 return result;
}


caon_ptr<ChasmRZ_Node> ChasmRZ_ASG_Position::insert_entry_node(
  const ChasmRZ_Connectors& connector, bool is_statement_entry, QString prefix)
{
 caon_ptr<ChasmRZ_Call_Entry> parent_rce = nullptr;
 if(!chiefs_.isEmpty())
 {
  caon_ptr<ChasmRZ_Node> top = chiefs_.top();
  parent_rce = top->chasm_rz_call_entry();
 }


 caon_ptr<ChasmRZ_Node> result = graph_build_->new_run_call_entry_node(is_statement_entry,
   prefix, parent_rce);

 if(is_statement_entry)
 {
  if(current_do_map_block_entry_node_)
  {
   last_do_map_block_statement_entry_node_ = result;
   if(!last_do_map_inner_block_first_entry_node_)
   {
    last_do_map_inner_block_first_entry_node_ = result;   }
  }
  else
  {
   last_statement_entry_node_ = result;
  }
 }
 check_append_chief(result);
 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
 CAON_PTR_DEBUG(ChasmRZ_Node ,result)

 current_node_ <<Cf/connector>> result;

 if(caon_ptr<ChasmRZ_Token> token = current_node_->chasm_rz_token())
 {
  if(token->raw_text() == "if")
    result->chasm_rz_call_entry()->flags.may_precede_if_block = true;

  //?
  if(token->raw_text() == "elsif")
    result->chasm_rz_call_entry()->flags.may_precede_elsif_block = true;

 }

 return result;
}

caon_ptr<ChasmRZ_Node> ChasmRZ_ASG_Position::pop_chief()
{
 return chiefs_.pop();
}

void ChasmRZ_ASG_Position::check_hold_closed_do_entry()
{
 if(chiefs_.isEmpty())
  return;
 caon_ptr<ChasmRZ_Node> tcnode = chiefs_.top();
 if(caon_ptr<ChasmRZ_Call_Entry> rce = tcnode->chasm_rz_call_entry())
 {
  CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)
  current_closed_do_entry_node_ = tcnode;
  position_state_ = Position_States::Active_Closed_Do_Entry;
 }
}

caon_ptr<ChasmRZ_Call_Entry> ChasmRZ_ASG_Position::current_closed_do_entry()
{
 if(current_closed_do_entry_node_)
 {
  return current_closed_do_entry_node_->chasm_rz_call_entry();
 }
}

void ChasmRZ_ASG_Position::check_pop_chief()
{
 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
 caon_ptr<ChasmRZ_Call_Entry> rce = current_node_->chasm_rz_call_entry();

 CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)

 caon_ptr<ChasmRZ_Node> self_node = current_node_;
 current_node_ = pop_chief();
 {
  CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
  CAON_DEBUG_NOOP
 }
 if(rce)
 {
  CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
  // //   Here its ref node should = its parent entry node

   rce->set_ref_node(current_node_);

   rce->set_self_node(self_node);

 }
 if( caon_ptr<ChasmRZ_Call_Entry> current_rce = current_node_->chasm_rz_call_entry() )
 {
  CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,current_rce)
  if(current_rce->flags.was_added_as_implied)
  {
   check_pop_chief();
  }
 }
}

void ChasmRZ_ASG_Position::check_append_chief(caon_ptr<ChasmRZ_Node> new_chief)
{

 CAON_PTR_DEBUG(ChasmRZ_Node ,new_chief)

 caon_ptr<ChasmRZ_Casement_Call_Entry> new_rce = new_chief->chasm_rz_casement_call_entry();
 new_rce->set_call_depth(chiefs_.size());
 if(current_node_)
 {
  CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
  if(caon_ptr<ChasmRZ_Casement_Call_Entry> rce = current_node_->chasm_rz_casement_call_entry())
  {
   CAON_PTR_DEBUG(ChasmRZ_Casement_Call_Entry ,rce)
   CAON_PTR_DEBUG(ChasmRZ_Node ,new_chief)
   if(caon_ptr<ChasmRZ_Node> ref_node = rce->ref_node())
   {
    CAON_PTR_DEBUG(ChasmRZ_Node ,ref_node)
    CAON_DEBUG_NOOP
   }

   // rce->debug_check_entry(Qy.Run_Call_Entry);
   // // //  This overwrites the ref_node to indicate that the
   // //     new_chief is "sibling" to the prior expression
   rce->set_ref_node(new_chief);
   // rce->debug_check_entry(Qy.Run_Call_Entry);

  }
  if(!chiefs_.isEmpty())
  {
   caon_ptr<ChasmRZ_Node> chiefs_top = chiefs_.top();
   CAON_PTR_DEBUG(ChasmRZ_Node ,chiefs_top)
   new_rce->set_parent_entry_node(chiefs_.top());
   // new_rce->debug_check_entry(Qy.Run_Call_Entry);
  }
  if(!block_chiefs_.isEmpty())
  {
   new_rce->set_block_chief_node(block_chiefs_.top());
  }
 }
 chiefs_.append(new_chief);
}

caon_ptr<ChasmRZ_Node> ChasmRZ_ASG_Position::insert_block_entry_node(
  const ChasmRZ_Connectors& connector)
{
 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)

 check_add_implied_call_entry();

 caon_ptr<ChasmRZ_Node> result;

 // //  temp
 if(connector == Qy.Casement_Block_Entry)
   result = graph_build_->new_casement_block_entry_node();
 else
   result = graph_build_->new_run_block_entry_node();

 if(current_node_)
 {
  if(caon_ptr<ChasmRZ_Token> token = current_node_->chasm_rz_token())
  {
   CAON_PTR_DEBUG(ChasmRZ_Token ,token)
   if(token->raw_text() == "else")
   {
    read_chiefs();
    read_block_chiefs();
    if(caon_ptr<ChasmRZ_Block_Entry> new_entry = result->chasm_rz_block_entry())
    {
     new_entry->flags.presumptive_else_block = true;
    }
   }
   else if(token->raw_text() == "elsif")
   {
    read_chiefs();
    read_block_chiefs();
    if(caon_ptr<ChasmRZ_Block_Entry> new_entry = result->chasm_rz_block_entry())
    {
     new_entry->flags.presumptive_elsif_block = true;
    }
   }
  }
 }

 caon_ptr<ChasmRZ_Node> ch;
 if(!chiefs_.isEmpty())
  ch = chiefs_.top();
 if(caon_ptr<ChasmRZ_Block_Entry> rbe = result->chasm_rz_block_entry())
 {
  if(ch)
  {
   CAON_PTR_DEBUG(ChasmRZ_Node ,ch)
   rbe->set_statement_entry_node(ch);
  }
  rbe->set_nested_depth(block_chiefs_.size());
 }

 CAON_PTR_DEBUG(ChasmRZ_Node ,result)

 block_chiefs_.append(result);
 current_node_ <<Cf/connector>> result;
 return result;
}




void ChasmRZ_ASG_Position::check_cancel_implied_call_entry()
{
 if(flags.possible_pending_equalizer_nested_entry)
 {
  flags.possible_pending_equalizer_nested_entry = false;
  current_node_ << Cf/Qy.Run_Call_Sequence >> held_equalizer_node_;
  current_node_ = held_equalizer_node_;
  position_state_ = Position_States::Active_Run_Token;
 }
}


void ChasmRZ_ASG_Position::add_new_node_as_implied_call_entry()
{
 // //  In this case bypassing held entry node

 flags.currently_implied_call_entry = true;
 caon_ptr<ChasmRZ_Node> prior_current_node = current_node_;
 current_node_ = insert_entry_node(Qy.Run_Call_Entry, false);

 read_chiefs();

 if(caon_ptr<ChasmRZ_Call_Entry> rce = current_node_->chasm_rz_call_entry())
 {
  CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)
  rce->flags.was_added_as_implied = true;
 }


 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)

 caon_ptr<ChasmRZ_Node> ival_node = graph_build_->new_autogenerated_token_node("ival");

 caon_ptr<ChasmRZ_Connection> cion = new ChasmRZ_Connection(current_node_);

 prior_current_node << Cf/Qy.Run_Call_Entry_Direct(cion) >> ival_node;

 current_node_ << Cf/Qy.Run_Call_Entry >> ival_node;

 current_node_ = ival_node;

 position_state_ = Position_States::Active_Run_Token;
}


void ChasmRZ_ASG_Position::check_add_implied_call_entry()
{
 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
   //?  Is this appropriate here...
 if(flags.currently_implied_call_entry)
 {
  flags.currently_implied_call_entry = false;
 }
 else if(flags.possible_pending_equalizer_nested_entry)
 {
  flags.possible_pending_equalizer_nested_entry = false;
  flags.currently_implied_call_entry = true;
  caon_ptr<ChasmRZ_Node> prior_current_node = current_node_;
  current_node_ = insert_entry_node(Qy.Run_Call_Entry, false);

  read_chiefs();

  if(caon_ptr<ChasmRZ_Call_Entry> rce = current_node_->chasm_rz_call_entry())
  {
   CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)
   rce->flags.was_added_as_implied = true;

   switch (held_equalizer_context_)
   {
   case Equalizer_Contexts::Number:
    rce->flags.equalizer_context_number = true;
    break;
   case Equalizer_Contexts::String:
    rce->flags.equalizer_context_string = true;
    break;
   default:
    break;
   }

  }

  held_equalizer_context_ = Equalizer_Contexts::N_A;

  CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)

  if(held_equalizer_node_)
  {
   if(caon_ptr<ChasmRZ_Token> token = held_equalizer_node_->chasm_rz_token())
    current_node_->set_label( current_node_->label() + " (" + token->raw_text() + ")");
  }

  caon_ptr<ChasmRZ_Connection> cion = new ChasmRZ_Connection(current_node_);

  prior_current_node << Cf/Qy.Run_Call_Entry_Direct(cion) >> held_equalizer_node_;

  current_node_ << Cf/Qy.Run_Call_Entry >> held_equalizer_node_;

  CAON_PTR_DEBUG(ChasmRZ_Node ,held_equalizer_node_)

  current_node_ = held_equalizer_node_;
  position_state_ = Position_States::Active_Run_Token;
 }
}

void ChasmRZ_ASG_Position::add_casement_sequence(caon_ptr<ChasmRZ_Node> node)
{
 switch(position_state_)
 {
 case Position_States::Active_Run_Casement_Chief:
   current_node_ << Cf/Qy.Run_Casement_Sequence >> node;
   position_state_ = Position_States::Active_Run_Casement_Node;
 }

}

void ChasmRZ_ASG_Position::add_casement_cross(caon_ptr<ChasmRZ_Node> entry_node)
{

}


void ChasmRZ_ASG_Position::add_casement_entry(caon_ptr<ChasmRZ_Node> entry_node, QString prefix)
{
 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)

 bool is_statement_entry = false;

 caon_ptr<ChasmRZ_Node> last_current_node_ = current_node_;


 //? check_add_implied_call_entry();

 switch(position_state_)
 {
 case Position_States::Root:
   current_node_ = insert_block_entry_node(Qy.Casement_Block_Entry);
   CAON_EVALUATE_DEBUG(ChasmRZ_Node ,c1 ,current_node_)
   last_current_node_ = current_node_;

   current_node_ = insert_casement_entry_node(Qy.Run_Casement_Entry, is_statement_entry, prefix);
   CAON_EVALUATE_DEBUG(ChasmRZ_Node ,c2 ,current_node_)

   last_current_node_ << Cf/Qy.Run_Casement_Entry >> current_node_;

   current_node_ << Cf/Qy.Run_Casement_Entry >> entry_node;

   position_state_ = Position_States::Active_Run_Casement_Chief;
 }
}

void ChasmRZ_ASG_Position::add_call_entry(bool is_statement_entry, QString prefix)
{
 caon_ptr<ChasmRZ_Node> old_current_node = nullptr;
 caon_ptr<ChasmRZ_Node> old_closed_do_entry_node = nullptr;
 if(current_closed_do_entry_node_)
 {
  caon_ptr<ChasmRZ_Call_Entry> ccde = current_closed_do_entry();
  CAON_PTR_DEBUG(ChasmRZ_Node ,current_closed_do_entry_node_);
  old_current_node = current_node_;
  current_node_ = current_closed_do_entry_node_;
  if(ccde)
   ccde->flags.is_do_closed_with_follow = 1;
  old_closed_do_entry_node = current_closed_do_entry_node_;
  current_closed_do_entry_node_ = nullptr;
 }


 caon_ptr<ChasmRZ_Node> retval_n = check_add_retval_nodes();

 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)

 check_add_implied_call_entry();

 switch(position_state_)
 {
 case Position_States::Active_Run_Chief:
  current_node_ = insert_entry_node(Qy.Run_Cross_Sequence, is_statement_entry, prefix);

  if(retval_n)
   retval_n << Cf/Qy.Retval_Follow >> current_node_;

  position_state_ = Position_States::Run_Pchasm_rz_Entry;
  break;

 case Position_States::Active_Closed_Do_Entry:
  if(!old_current_node)
  {
   qDebug() << "Warning at: " << __LINE__;
  }
  last_pchasm_rz_entry_node_ = current_node_;
  current_node_ = insert_entry_node(Qy.Run_Cross_Sequence, false, prefix);
  if(retval_n)
   retval_n << Cf/Qy.Retval_Follow >> current_node_;

  position_state_ = Position_States::Run_Pchasm_rz_Entry;
  if(old_closed_do_entry_node)
  {
   CAON_PTR_DEBUG(ChasmRZ_Node ,old_closed_do_entry_node)
   if(caon_ptr<ChasmRZ_Call_Entry> rce = current_node_->chasm_rz_call_entry())
   {
    rce->flags.follows_closed_do = true;

    // //  We need to connect the arrows ...
     //
    if(caon_ptr<ChasmRZ_Node> old_do_node = Qy.Run_Call_Entry(in_Cf old_closed_do_entry_node))
    {
     CAON_PTR_DEBUG(ChasmRZ_Node ,old_do_node)
     old_do_node->debug_connections();
     if(caon_ptr<ChasmRZ_Node> old_arrow_node = Qy.Run_Call_Sequence(in_Cf old_do_node))
     {
      CAON_PTR_DEBUG(ChasmRZ_Node ,old_arrow_node)
      old_arrow_node << Cf/Qy.Run_Fundef_Arrow_Sequence >> current_node_;
     }
    }
   }
  }
  break;
 case Position_States::Active_Run_Token:
  last_pchasm_rz_entry_node_ = current_node_;
  current_node_ = insert_entry_node(Qy.Run_Call_Entry, is_statement_entry, prefix);
  if(retval_n)
   retval_n << Cf/Qy.Retval_Follow >> current_node_;

  position_state_ = Position_States::Run_Pchasm_rz_Entry;
  break;
 default: break;
 }
}

void ChasmRZ_ASG_Position::add_equalizer_token_node(caon_ptr<ChasmRZ_Node> token_node)
{
 bool maybe_set_pending_equalizer_entry = position_state_ == Position_States::Active_Run_Token;

 if(held_assignment_annotation_node_)
 {
  token_node << Cf/Qy.Assignment_Annotation>> held_assignment_annotation_node_;
  held_assignment_annotation_node_ = nullptr;
 }

 add_token_node(token_node);
 if(maybe_set_pending_equalizer_entry)
 {
  flags.pending_equalizer_value = true;
 }
}


void ChasmRZ_ASG_Position::add_arrow_token_node(caon_ptr<ChasmRZ_Node> token_node)
{
 ChasmRZ_Token& token = *token_node->chasm_rz_token();
 switch(token.special_token())
 {
 case ChasmRZ_Code_Representation::Special_Tokens::Call_Arrow:
  add_arrow_node(token_node, ChasmRZ_Function_Def_Kinds::Call_Arrow);
  break;

 case ChasmRZ_Code_Representation::Special_Tokens::Call_Arrow_Fdef:
  add_arrow_node(token_node, ChasmRZ_Function_Def_Kinds::Call_Arrow);
  break;

 case ChasmRZ_Code_Representation::Special_Tokens::Call_Arrow_To_Block:
  add_arrow_node(token_node, ChasmRZ_Function_Def_Kinds::Call_Arrow);
  break;

 case ChasmRZ_Code_Representation::Special_Tokens::Call_Arrow_Async:
  add_arrow_node(token_node, ChasmRZ_Function_Def_Kinds::Call_Arrow_Async);
  break;

 case ChasmRZ_Code_Representation::Special_Tokens::Call_Arrow_Note:
  add_arrow_node(token_node, ChasmRZ_Function_Def_Kinds::Call_Arrow_Note);
  break;

 case ChasmRZ_Code_Representation::Special_Tokens::Call_Arrow_No_Def:
  add_arrow_node(token_node, ChasmRZ_Function_Def_Kinds::Call_Arrow_No_Def);
  break;

 }
}


caon_ptr<ChasmRZ_Node> ChasmRZ_ASG_Position::check_implied_lambda_tuple(ChasmRZ_Function_Def_Kinds kind)
{
 caon_ptr<ChasmRZ_Node> result = nullptr;
 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)

 if(current_function_def_entry_node_)
 {
  CAON_PTR_DEBUG(ChasmRZ_Node ,current_function_def_entry_node_)
  if(caon_ptr<ChasmRZ_Function_Def_Entry> fdef = current_function_def_entry_node_->chasm_rz_function_def_entry())
  {
   CAON_PTR_DEBUG(ChasmRZ_Function_Def_Entry ,fdef)
   if(fdef->kind() == ChasmRZ_Function_Def_Kinds::N_A)
    fdef->set_kind(kind);
  }
  return result;
 }
 if(flags.pending_equalizer_value)
 {
  caon_ptr<ChasmRZ_Node> fdef_node = graph_build_->new_function_def_entry_node(*current_node_, kind);

  current_function_def_entry_node_ = fdef_node;

 }
 else if(position_state_ == Position_States::Active_Run_Chief)
 {
  if(caon_ptr<ChasmRZ_Call_Entry> rce = current_node_->chasm_rz_call_entry())
  {
   CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)

   if(last_pchasm_rz_entry_node_)
   {
    CAON_PTR_DEBUG(ChasmRZ_Node ,last_pchasm_rz_entry_node_)
    caon_ptr<ChasmRZ_Node> fdef_node = graph_build_->new_function_def_entry_node(*last_pchasm_rz_entry_node_, kind);

    last_pchasm_rz_entry_node_->delete_relation(&Cf, Qy.Run_Call_Entry, current_node_);
    current_function_def_entry_node_ = fdef_node;

    caon_ptr<ChasmRZ_Node> tuple_info_node = graph_build_->create_tuple(ChasmRZ_Tuple_Info::Tuple_Formations::Indicates_Input,
     ChasmRZ_Tuple_Info::Tuple_Indicators::Enter_Array, ChasmRZ_Tuple_Info::Tuple_Formations::N_A);

    CAON_PTR_DEBUG(ChasmRZ_Node ,tuple_info_node)

    current_node_->debug_connections();

    caon_ptr<ChasmRZ_Node> entry_node = Qy.Run_Call_Entry(in_Cf current_node_);

    result = current_node_;

    fdef_node << Cf/Qy.Run_Call_Entry >> current_node_;

    CAON_PTR_DEBUG(ChasmRZ_Node ,result)

    current_node_ << Cf/Qy.Run_Data_Entry >> tuple_info_node;
    tuple_info_node << Cf/Qy.Run_Data_Entry >> entry_node;
    current_node_->delete_relation(&in_Cf Qy.Run_Call_Entry, entry_node);

    caon_ptr<ChasmRZ_Node> tuple_leave_node = graph_build_->create_tuple(ChasmRZ_Tuple_Info::Tuple_Formations::N_A,
     ChasmRZ_Tuple_Info::Tuple_Indicators::Leave_Array, ChasmRZ_Tuple_Info::Tuple_Formations::N_A, false);

    tuple_info_node << Cf/Qy.Run_Data_Leave >> tuple_leave_node;

   }
  }
 }
 return result;
}


void ChasmRZ_ASG_Position::add_type_indicator(caon_ptr<ChasmRZ_Node> node)
{
 type_indicator_node_ = node;
 flags.active_type_indicator_node = true;
}

void ChasmRZ_ASG_Position::add_arrow_node(caon_ptr<ChasmRZ_Node> token_node, ChasmRZ_Function_Def_Kinds kind)
{
 CAON_PTR_DEBUG(ChasmRZ_Node ,token_node)
 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)

 read_chiefs();

 caon_ptr<ChasmRZ_Node> new_current_node = check_implied_lambda_tuple(kind);

 if(new_current_node)
 {
  CAON_PTR_DEBUG(ChasmRZ_Node ,new_current_node)
  CAON_DEBUG_NOOP
 }

 if(current_function_def_entry_node_)
 {
  // //  This confirms the arrow is a function definition not a
   //    function call, e.g., "\->" rather than "/->".

  if(last_function_definition_arrow_node_)
  {
   // //  We need this only in a block-map
   if(current_do_map_block_entry_node_)
   {
    last_function_definition_arrow_node_ << Cf/Qy.Run_Fundef_Map_Key_Sequence >> token_node;
   }
  }
  last_function_definition_arrow_node_ = token_node;


  CAON_PTR_DEBUG(ChasmRZ_Node ,current_function_def_entry_node_)


  ChasmRZ_Function_Def_Entry& fdef = *current_function_def_entry_node_->chasm_rz_function_def_entry();
  ChasmRZ_Node& prior_node = *fdef.prior_node();

  // //  I.e., part of a do-map
  if(fdef.label_node())
  {
   // //  Check the nature of the prior
   if(caon_ptr<ChasmRZ_Token> token = prior_node.chasm_rz_token())
   {
    CAON_PTR_DEBUG(ChasmRZ_Token ,token)
    if(token->flags.is_do)
    {
     &prior_node << Cf/Qy.Run_Call_Sequence >> token_node;
    }
    else
    {
     // //  For use by valuer, but obsolete now ...
      // current_function_def_entry_node_ << Cf/Qy.Run_Fundef_Map_Key_Sequence_Ref >> token_node;
    }
    token_node << Cf/Qy.Run_Function_Def_Entry >> current_function_def_entry_node_;
   }
  }
  else
  {
   // //    Assumes arrow node is always Run_Call_Sequence from something
   &prior_node << Cf/Qy.Run_Call_Sequence >> token_node;
   token_node << Cf/Qy.Run_Function_Def_Entry >> current_function_def_entry_node_;
  }

  if(kind == ChasmRZ_Function_Def_Kinds::Call_Arrow_Note)
  {
   flags.awaiting_function_def_note = true;
   current_node_ = token_node;
   return;
  }

  current_function_def_entry_node_ = nullptr;

  if(kind == ChasmRZ_Function_Def_Kinds::Call_Arrow_Type_Expression)
  {
   if(new_current_node)
    current_node_ = new_current_node;
   else
    current_node_ = token_node;
   return;
  }
  //?else
   current_node_ = token_node;


   // //   generic block entry pattern?
  caon_ptr<ChasmRZ_Node> block_entry_node = insert_block_entry_node(Qy.Run_Block_Entry);

  caon_ptr<ChasmRZ_Block_Entry> rbe = block_entry_node->chasm_rz_block_entry();

  if(caon_ptr<ChasmRZ_Token> token = prior_node.chasm_rz_token())
  {
   if(token->flags.is_do)
   {
    rbe->flags.do_block_entry = true;
   }
  }

  switch(kind)
  {
  case ChasmRZ_Function_Def_Kinds::Call_Arrow_Async:
   rbe->flags.function_definition = true;
   rbe->flags.async = true;
   break;
// //  todo ...
//  case ChasmRZ_Function_Def_Kinds::Call_Arrow_Async:
//   rbe->flags.function_definition = true;
//   rbe->flags.async = true;
//   break;
  case ChasmRZ_Function_Def_Kinds::Call_Arrow_With_Matching:
  case ChasmRZ_Function_Def_Kinds::Call_Arrow_No_Def:
  case ChasmRZ_Function_Def_Kinds::Call_Arrow:
   rbe->flags.function_definition = true;
   break;
  case ChasmRZ_Function_Def_Kinds::Call_Arrow_Monad:
   rbe->flags.monad_definition = true;
   break;
  default: break;
  }

  add_block_entry_node(block_entry_node);
 }
 else
 {
  if(caon_ptr<ChasmRZ_Token> chasm_rz_token = token_node->chasm_rz_token())
  {
   QString s = chasm_rz_token->raw_text();
   s.prepend('/');
   chasm_rz_token->set_raw_text(s);
  }
  if(caon_ptr<ChasmRZ_Token> cchasm_rz_token = current_node_->chasm_rz_token())
  {
   cchasm_rz_token->flags.precedes_call_arrow = true;
  }
  add_token_node(token_node);
 }
}

void ChasmRZ_ASG_Position::add_block_entry_node(caon_ptr<ChasmRZ_Node> block_entry_node)
{
 if(current_do_map_block_entry_node_)
 {
  block_entry_node << Cf/Qy.Parent_Do_Map_Block >> current_do_map_block_entry_node_;

  //?
  if(last_do_map_block_statement_entry_node_)
  {
   last_do_map_block_statement_entry_node_ << Cf/Qy.Run_Nested_Do_Map_Block_Entry_Rewind >> block_entry_node;
  }

  if(last_do_map_inner_block_first_entry_node_)
  {
   last_do_map_inner_block_first_entry_node_ << Cf/Qy.Run_Nested_Do_Map_Block_Entry >> block_entry_node;
   last_do_map_inner_block_first_entry_node_ = block_entry_node;
  }
 }
 else if(last_statement_entry_node_)
 {
  last_statement_entry_node_ << Cf/Qy.Run_Nested_Block_Entry >> block_entry_node;
 }
 if(!current_do_map_block_entry_node_)
 {
  if(current_block_map_entry_node_)
  {
   block_entry_node << Cf/Qy.Parent_Block_Map >> current_block_map_entry_node_;
  }
  current_block_map_entry_node_ = block_entry_node;
 }
 current_node_ = block_entry_node;
 caon_ptr<ChasmRZ_Node> entry_node = insert_entry_node(Qy.Run_Call_Entry, true);
 if(caon_ptr<ChasmRZ_Call_Entry> rce = entry_node->chasm_rz_call_entry())
 {
  rce->set_block_entry_node(block_entry_node);
 }
 current_node_ = entry_node;
 position_state_ = Position_States::Block_Entry;
}

void ChasmRZ_ASG_Position::add_block_map_entry()
{
 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)

 caon_ptr<ChasmRZ_Node> block_entry_node = insert_block_entry_node(Qy.Run_Block_Entry);
 CAON_PTR_DEBUG(ChasmRZ_Node ,block_entry_node)

 caon_ptr<ChasmRZ_Block_Entry> rbe = block_entry_node->chasm_rz_block_entry();

 if(caon_ptr<ChasmRZ_Token> token = current_node_->chasm_rz_token())
 {
  CAON_PTR_DEBUG(ChasmRZ_Token ,token)
  rbe->flags.ghost_block = token->flags.precedes_ghost_scope;
  rbe->flags.likely_if_block = token->flags.follows_if_token;
  rbe->flags.likely_elsif_block = token->flags.follows_elsif_token;

  if(token->flags.is_do)
  {
   rbe->flags.do_map = true;

   ChasmRZ_Node& cc = get_current_chief();

   if(caon_ptr<ChasmRZ_Call_Entry> rce = cc.chasm_rz_call_entry())
   {
    CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)
    rce->flags.has_do_map = true;

    // // note that in this case  rbe's statement_entry_node_
     //   is not actually statement entry but the same as
     //   ChasmRZ_Node& cc above.
   }
  }
 }

 else if(caon_ptr<ChasmRZ_Call_Entry> rce = current_node_->chasm_rz_call_entry())
 {
  CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)
  CAON_DEBUG_NOOP
  if(rce->flags.may_precede_if_block)
  {
   rbe->flags.likely_if_block = true;
  }

  //?
  if(rce->flags.may_precede_elsif_block)
  {
   rbe->flags.likely_elsif_block = true;
  }

 }


 CAON_PTR_DEBUG(ChasmRZ_Block_Entry ,rbe)

 // //   Currently just stick do_maps as addenda to do tokens ...
 if(rbe->flags.do_map)
 {
  current_node_ << Cf/Qy.Do_Map_Block >> block_entry_node;
  if(current_do_map_block_entry_node_)
  {
   block_entry_node << Cf/Qy.Parent_Do_Map_Block >> current_do_map_block_entry_node_;
  }
  // //?  don't want arrows outside the map to be confused with those inside ...
  last_function_definition_arrow_node_ = nullptr;
  current_do_map_block_entry_node_ = block_entry_node;
 }
 else
 {
  add_block_entry_node(block_entry_node);
 }

}


caon_ptr<ChasmRZ_Tuple_Info> ChasmRZ_ASG_Position::current_tuple_info()
{
 caon_ptr<ChasmRZ_Tuple_Info> result = nullptr;
 if(!chiefs_.isEmpty())
 {
  caon_ptr<ChasmRZ_Node> chiefs_top = chiefs_.top();
  CAON_PTR_DEBUG(ChasmRZ_Node ,chiefs_top)
  CAON_DEBUG_NOOP
  if(chiefs_top)
  {
   if(caon_ptr<ChasmRZ_Tuple_Info> rti = chiefs_top->chasm_rz_tuple_info())
   {
    CAON_PTR_DEBUG(ChasmRZ_Tuple_Info ,rti)
    result = rti;
   }
  }
 }
 return result;
}


void ChasmRZ_ASG_Position::finalize_overall_if_block()
{
 if(block_chiefs_ifs_.size() > 0)
 {
  current_node_ = block_chiefs_ifs_.pop();

  while(chiefs_.top() != current_node_)
  {
   caon_ptr<ChasmRZ_Node> n = pop_chief();
   CAON_PTR_DEBUG(ChasmRZ_Node ,n)
   CAON_DEBUG_NOOP
  }

  CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
  CAON_DEBUG_NOOP
 }
}

void ChasmRZ_ASG_Position::add_block_map_leave()
{
 // //  handled by reset_if_block_pending_follow,
  //    called by close_statement() ...
 //  leaving from block enclosing nested block;
 //  so the follow's not coming


 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
 caon_ptr<ChasmRZ_Call_Entry> rce = current_node_->chasm_rz_call_entry();
 CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)

 if(current_do_map_block_entry_node_)
 {
  if(caon_ptr<ChasmRZ_Node> n = Qy.Parent_Do_Map_Block(in_Cf current_do_map_block_entry_node_))
  {
   current_do_map_block_entry_node_ = n;
   //? and...?

   //what about?...
   //last_do_map_inner_block_first_entry_node_
  }
  else
  {
   current_do_map_block_entry_node_ = nullptr;
   last_do_map_inner_block_first_entry_node_ = nullptr;

   // //  Always close outer statement here?
   close_statement();
   // it seems that's not popping the chief from
   // Cross_Run_Chief position state ...
   check_pop_chief();

   CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
   CAON_DEBUG_NOOP

  }

  return;
 }

 if(current_block_map_entry_node_)
 {
  if(caon_ptr<ChasmRZ_Node> n = Qy.Parent_Block_Map(in_Cf current_block_map_entry_node_))
  {
   current_block_map_entry_node_ = n;
  }
  else
  {
   current_block_map_entry_node_ = nullptr;
  }
 }

 if(!chiefs_.isEmpty())
 {
  caon_ptr<ChasmRZ_Node> chiefs_top = chiefs_.top();
  CAON_PTR_DEBUG(ChasmRZ_Node ,chiefs_top)
  CAON_DEBUG_NOOP
 }

 // nested if with nothing following it
 if(flags.if_block_pending_follow)
 {
  flags.if_block_pending_follow = false;
  finalize_overall_if_block();
  close_statement();
 }
 if(flags.elsif_block_pending_follow)
 {
  flags.elsif_block_pending_follow = false;
  finalize_overall_if_block();
  close_statement();
 }

 caon_ptr<ChasmRZ_Node> bc_node = block_chiefs_.pop();
 CAON_PTR_DEBUG(ChasmRZ_Node ,bc_node)


 if(bc_node)
 {
  caon_ptr<ChasmRZ_Block_Entry> rbe = bc_node->chasm_rz_block_entry();
  CAON_PTR_DEBUG(ChasmRZ_Block_Entry ,rbe)
  CAON_DEBUG_NOOP
 }


 if(rce)
 {
  caon_ptr<ChasmRZ_Node> ref_node = rce->ref_node();
  caon_ptr<ChasmRZ_Node> rbe_node = rce->block_entry_node();
  caon_ptr<ChasmRZ_Node> rbc_node = rce->block_chief_node();
  CAON_PTR_DEBUG(ChasmRZ_Node ,rbe_node)
  CAON_PTR_DEBUG(ChasmRZ_Node ,rbc_node)
  CAON_PTR_DEBUG(ChasmRZ_Node ,ref_node)
  CAON_DEBUG_NOOP
 }


 current_node_ = bc_node;

 bool need_close_statement = false;
//?
 if(caon_ptr<ChasmRZ_Block_Entry> rbe = bc_node->chasm_rz_block_entry())
 {
  CAON_PTR_DEBUG(ChasmRZ_Block_Entry ,rbe)
  if(rbe->flags.likely_if_block)
  {
   flags.if_block_pending_follow = true;
  }
  else if(rbe->flags.likely_elsif_block)
  {
   flags.elsif_block_pending_follow = true;
  }
  else if(rbe->flags.presumptive_else_block)
  {
   need_close_statement = true;
   // //  this may be duplicated in finalize_overall_if_block()
   if(block_chiefs_ifs_.size() > 0)
   {
    current_node_ = block_chiefs_ifs_.pop();

    while(chiefs_.top() != current_node_)
    {
     caon_ptr<ChasmRZ_Node> n = pop_chief();
     CAON_PTR_DEBUG(ChasmRZ_Node ,n)
     CAON_DEBUG_NOOP
    }

    CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
    CAON_DEBUG_NOOP
   }
  }
  else if(caon_ptr<ChasmRZ_Node> en_node = Qy.Run_Call_Entry(in_Cf bc_node))
  {
   CAON_PTR_DEBUG(ChasmRZ_Node ,en_node)
   current_node_ = bc_node;
   position_state_ = Position_States::Cross_Run_Chief;
  }
 }

 if(need_close_statement)
  close_statement();

}



void ChasmRZ_ASG_Position::hold_do_mapkey_node(caon_ptr<ChasmRZ_Node> node)
{
 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
 held_do_mapkey_node_ = node;
}

void ChasmRZ_ASG_Position::hold_mapkey_node(caon_ptr<ChasmRZ_Node> node)
{
 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
 if(position_state_ == Position_States::Block_Entry
    || position_state_ == Position_States::Cross_Block_Entry)
 {
  if(caon_ptr<ChasmRZ_Call_Entry> rce = current_node_->chasm_rz_call_entry())
  {
   CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)
   rce->flags.has_label = true;
   current_node_ << Cf/Qy.Run_Map_Key_Label >> node;
   return;
  }
 }
 held_mapkey_node_ = node;
}

void ChasmRZ_ASG_Position::check_held_mapkey_node(caon_ptr<ChasmRZ_Node> new_node)
{
 if(held_mapkey_node_)
 {
  CAON_PTR_DEBUG(ChasmRZ_Node ,new_node)
  new_node << Cf/Qy.Run_Map_Key_Value >> held_mapkey_node_;
  held_mapkey_node_ = nullptr;
  if(caon_ptr<ChasmRZ_Token> token = new_node->chasm_rz_token())
  {
   token->flags.has_mapkey = true;
  }
 }
}



void ChasmRZ_ASG_Position::check_if_block_non_continue(caon_ptr<ChasmRZ_Token> token)
{
 if(flags.if_block_pending_follow)
 {
  flags.if_block_pending_follow = false;
  CAON_PTR_DEBUG(ChasmRZ_Token ,token)
  if( (token->raw_text() != "else") && (token->raw_text() != "elsif") )
  {
   finalize_overall_if_block();
   close_statement();
  }
  else
  {
   //? not sure what's going on here ...
   //?reset_if_block_pending_follow();
  }
 }
 if(flags.elsif_block_pending_follow)
 {
  flags.elsif_block_pending_follow = false;
  CAON_PTR_DEBUG(ChasmRZ_Token ,token)
  if( (token->raw_text() != "else") && (token->raw_text() != "elsif") )
  {
   finalize_overall_if_block();
   close_statement();
  }
 }
}

void ChasmRZ_ASG_Position::reset_if_block_pending_follow()
{
 flags.if_block_pending_follow = false;
 flags.elsif_block_pending_follow = false;
}


void ChasmRZ_ASG_Position::add_raw_asg_token(caon_ptr<ChasmRZ_Node> token_node)
{
 CAON_PTR_DEBUG(ChasmRZ_Node ,token_node)
 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)

 current_node_ << Cf/Qy.Raw_ASG_Paste >> token_node;
}

bool ChasmRZ_ASG_Position::current_node_is_symbol_declaration()
{
 if(current_node_)
 {
  if(caon_ptr<ChasmRZ_Token> token = current_node_->chasm_rz_token())
  {
   CAON_PTR_DEBUG(ChasmRZ_Token ,token)
   return token->flags.is_symbol_declaration;
  }
 }
 return false;
}

caon_ptr<ChasmRZ_Node> ChasmRZ_ASG_Position::current_entry_is_backquoted()
{
 caon_ptr<ChasmRZ_Node> result = nullptr;
 if(chiefs_.size() > 0)
 {
  caon_ptr<ChasmRZ_Node> top = chiefs_.top();
  if(caon_ptr<ChasmRZ_Call_Entry> rce = top->chasm_rz_call_entry())
  {
   CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)
   if(rce->flags.is_backquoted)
   {
    result = top;
   }
  }
 }
 return result;
}

void ChasmRZ_ASG_Position::hold_retval_node(caon_ptr<ChasmRZ_Node> token_node)
{
 flags.holding_retval_node = true;
 held_retval_node_ = token_node;
}

bool ChasmRZ_ASG_Position::awaiting_statement_call_entry()
{
 switch(position_state_)
 {
 case Position_States::Root:
 case Position_States::Block_Entry:
 case Position_States::Cross_Block_Entry:
   return true;
 case Position_States::Cross_Run_Chief:
   {
    bool is_statement_entry = true;
    if(current_node_)
    {
     if(caon_ptr<ChasmRZ_Call_Entry> rce = current_node_->chasm_rz_call_entry())
       is_statement_entry = rce->flags.is_statement_entry;
    }
    return is_statement_entry;
   }
 default:
   return false;
 }
}

void ChasmRZ_ASG_Position::add_token_node(caon_ptr<ChasmRZ_Node> token_node)
{
 if(flags.awaiting_function_def_note)
 {
  flags.awaiting_function_def_note = false;
  if(current_function_def_entry_node_)
  {
   if(caon_ptr<ChasmRZ_Function_Def_Entry> fde = current_function_def_entry_node_->chasm_rz_function_def_entry())
   {
    CAON_PTR_DEBUG(ChasmRZ_Function_Def_Entry ,fde)
    fde->set_note_node(token_node);
    current_node_ = token_node;
    current_function_def_entry_node_ = nullptr;
    return;
   }
  }
 }

 if(flags.holding_retval_node)
 {
  //
  if(held_retval_follow_node_)
  {
   flags.holding_retval_node = false;
   // //   held_retval_node_ is not used at all ...
   held_retval_node_ = nullptr;
   held_retval_follow_node_ << Cf/Qy.Retval_Follow >> token_node;
   add_token_node(held_retval_follow_node_);
   held_retval_follow_node_ = nullptr;
  }
  else
  {
   held_retval_follow_node_ = token_node;
   return;
  }

 }

 CAON_PTR_DEBUG(ChasmRZ_Node ,token_node)
 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
 caon_ptr<ChasmRZ_Token> token = token_node->chasm_rz_token();
 CAON_PTR_DEBUG(ChasmRZ_Token ,token)


 if(flags.active_type_indicator_node)
 {
  flags.active_type_indicator_node = false;
  current_node_ << Cf/Qy.Type_Indicator >> token_node;
  if(token)
  {
   token->flags.has_type_indicator = true;
  }
  return;
 }

 check_if_block_non_continue(token);

 caon_ptr<ChasmRZ_Node> entry_node = nullptr;
 caon_ptr<ChasmRZ_Node> maybe_if_entry_node = nullptr;

 bool was_pending_equalizer_value = flags.pending_equalizer_value;
 flags.pending_equalizer_value = false;

 QString key = token->raw_text();
 Equalizer_Contexts equalizer_context;
 if(was_pending_equalizer_value)
 {
  QPair<Equalizer_Contexts, QString> pr = parse_equalizer_context(key);
  equalizer_context = pr.first;
  if(equalizer_context != Equalizer_Contexts::N_A)
  {
   // //  maybe a flag on token? ...
   token->set_raw_text(pr.second);
  }
 }
 else
 {
  equalizer_context = Equalizer_Contexts::N_A;
 }

 check_add_implied_call_entry();

 if(token->flags.is_do)
 {
  if(caon_ptr<ChasmRZ_Call_Entry> rce = current_node_->chasm_rz_call_entry())
  {
   CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
   CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)
   current_node_ << Cf/Qy.Run_Cross_Do >> token_node;
  }

  add_call_entry(false);
  CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
  if(caon_ptr<ChasmRZ_Call_Entry> rce = current_node_->chasm_rz_call_entry())
  {
   CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)
   rce->flags.is_do_lambda = true;
   rce->flags.was_added_as_pchasm_rz_ghost_block = true;
  }
 }

 caon_ptr<ChasmRZ_Node> prior_current_node = nullptr;

 const ChasmRZ_Connectors* pConnector;
 const ChasmRZ_Connectors* aConnector = nullptr;
 caon_ptr<ChasmRZ_Node> aConnector_src_node = nullptr;

 switch(position_state_)
 {
 case Position_States::Root:
 case Position_States::Cross_Run_Chief:
 case Position_States::Run_Pchasm_rz_Entry:
  pConnector = &Qy.Run_Call_Entry;

  if(current_node_)
  {
   if(token)
   {
    aConnector = &Qy.Run_Call_Entry_Direct;
    aConnector_src_node = current_node_;

     //  // debug
    CAON_PTR_DEBUG(ChasmRZ_Token ,token)
    current_node_->set_label(current_node_->label() + " (" + token->raw_text() + ")");
   }
  }
  break;

 case Position_States::Block_Entry:
 {
  CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
  maybe_if_entry_node = current_node_;
  if(caon_ptr<ChasmRZ_Call_Entry> rce = current_node_->chasm_rz_call_entry())
  {
   rce->set_ref_node(current_node_);
   CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)
   CAON_DEBUG_NOOP
  }
  pConnector = &Qy.Run_Call_Entry; break;
 }

 case Position_States::Active_Run_Chief:
  {
   if(caon_ptr<ChasmRZ_Call_Entry> rce = current_node_->chasm_rz_call_entry())
   {
    rce->set_ref_node(current_node_);
   }
   pConnector = &Qy.Run_Cross_Sequence; break;
  }
 case Position_States::Active_Run_Token:
  if(was_pending_equalizer_value)
  {
   flags.possible_pending_equalizer_nested_entry = true;
   held_equalizer_node_ = token_node;
   held_equalizer_context_ = equalizer_context;
   return;
  }
  if(current_node_)
  {
   if(caon_ptr<ChasmRZ_Token> chasm_rz_token = current_node_->chasm_rz_token())
   {
    CAON_PTR_DEBUG(ChasmRZ_Token ,chasm_rz_token)
    // maybe these (if and elsif) should be flags not raw tests ...
    if(chasm_rz_token->raw_text() == "if")
    {
     token->flags.follows_if_token = true;
    }
    else if(chasm_rz_token->raw_text() == "elsif")
    {
     token->flags.follows_elsif_token = true;
    }
    else if(chasm_rz_token->flags.is_arrow)
    {
     switch(chasm_rz_token->special_token())
     {
     case ChasmRZ_Code_Representation::Special_Tokens::Call_Arrow:
      token->flags.follows_call_arrow = true;
      break;
     default: break;
     }

    }
   }
  }
  pConnector = &Qy.Run_Call_Sequence; break;

 case Position_States::Active_Closed_Do_Entry:
  {
  // // maybe a method to get the old arrow node?
   if(current_closed_do_entry_node_)
   {
    CAON_PTR_DEBUG(ChasmRZ_Node ,current_closed_do_entry_node_)
    CAON_PTR_DEBUG(ChasmRZ_Node ,token_node)

    pConnector = &Qy.Run_Cross_Sequence;
    current_node_ = current_closed_do_entry_node_;

    if(caon_ptr<ChasmRZ_Node> old_do_node = Qy.Run_Call_Entry(in_Cf current_closed_do_entry_node_))
    {
     CAON_PTR_DEBUG(ChasmRZ_Node ,old_do_node)
     old_do_node->debug_connections();
     if(caon_ptr<ChasmRZ_Node> old_arrow_node = Qy.Run_Call_Sequence(in_Cf old_do_node))
     {
      CAON_PTR_DEBUG(ChasmRZ_Node ,old_arrow_node)

      old_arrow_node << Cf/Qy.Run_Token_Arrow_Sequence >> token_node;

//        CAON_PTR_DEBUG(ChasmRZ_Node ,token_node)
//        CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
//        caon_ptr<ChasmRZ_Token> token = token_node->chasm_rz_token();
//        CAON_PTR_DEBUG(ChasmRZ_Token ,token)

     }
    }
   }
  }
  break;
 case Position_States::Data_Entry:
  pConnector = &Qy.Run_Data_Entry;
  break;
 default:
  return;
 }

 const ChasmRZ_Connectors& connector = *pConnector;

 switch(position_state_)
 {
 case Position_States::Root:
  {
  caon_ptr<ChasmRZ_Node> block_entry_node = insert_block_entry_node(Qy.Run_Block_Entry);
  caon_ptr<ChasmRZ_Block_Entry> rbe = block_entry_node->chasm_rz_block_entry();
  rbe->flags.file_default = true;
  current_node_ = block_entry_node;
  entry_node = insert_entry_node(Qy.Run_Call_Entry, true);
  prior_current_node = current_node_;
  current_node_ = entry_node;
  maybe_if_entry_node = entry_node;

  // for debug
  if(token)
   entry_node->set_label(entry_node->label() + " (" + token->raw_text() + ")");

  goto Default;
  }
 case Position_States::Cross_Run_Chief:
  {
   // //   This needs to get whether chief is statement_entry
   bool is_statement_entry = true;
   if(current_node_)
   {
    if(caon_ptr<ChasmRZ_Call_Entry> rce = current_node_->chasm_rz_call_entry())
     is_statement_entry = rce->flags.is_statement_entry;
   }

   entry_node = insert_entry_node(Qy.Run_Cross_Sequence, is_statement_entry);

   maybe_if_entry_node = entry_node;

   CAON_PTR_DEBUG(ChasmRZ_Node ,entry_node);

   // for debug
   if(caon_ptr<ChasmRZ_Token> token = token_node->chasm_rz_token())
   {
    CAON_PTR_DEBUG(ChasmRZ_Token ,token);

    entry_node->set_label(entry_node->label() + " (" + token->raw_text() + ")");
   }

   // //?
   if(entry_node)
   {
    if(caon_ptr<ChasmRZ_Call_Entry> rce = entry_node->chasm_rz_call_entry())
    {
     rce->set_ref_node(entry_node);

     if(held_mapkey_node_)
     {
      rce->flags.has_label = true;
      entry_node << Cf/Qy.Run_Map_Key_Label >> held_mapkey_node_;
      held_mapkey_node_ = nullptr;
     }
    }
   }

   current_node_ = entry_node;
   goto Default;
  }
 Default:
 default:
  {
   CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)

   node_frame<RZ::RZ_Core::ChasmRZ_Dominion, RZ::RZ_Core::ChasmRZ_Galaxy>::Frame_With_Connector fwc = Cf/connector;
   node_ptr<RZ::RZ_Core::ChasmRZ_Dominion>::Node_With_Connector nwc = current_node_ << fwc;

   node_frame<RZ::RZ_Core::ChasmRZ_Dominion, RZ::RZ_Core::ChasmRZ_Galaxy>::Frame_With_Connector fwc1 = nwc.frame_with_connector;
   current_node_ << Cf/connector>> token_node;

   // why does == act wierd?
   if( (connector == Qy.Run_Call_Entry) )
   {
    if(prior_current_node)
    {
     caon_ptr<ChasmRZ_Connection> cion = new ChasmRZ_Connection(entry_node);
     prior_current_node << Cf/Qy.Run_Call_Entry_Direct(cion) >> token_node;
    }
    else if(aConnector_src_node)
    {
     caon_ptr<ChasmRZ_Connection> cion = new ChasmRZ_Connection(entry_node);
     aConnector_src_node << Cf/Qy.Run_Call_Entry_Direct(cion) >> token_node;
    }
   }

   current_node_ = token_node;

   if(maybe_if_entry_node)
   {
    if(token)
    {
     if(token->raw_text() == "if")
     {
      block_chiefs_ifs_.push(maybe_if_entry_node);
     }
    }
   }

  }

 }

 // //  Handle the held mapkey possibility
 switch(position_state_)
 {
 default:
  check_held_mapkey_node(current_node_);
 }

 switch(position_state_)
 {
 default:
  position_state_ = Position_States::Active_Run_Token;
 }


}

void ChasmRZ_ASG_Position::close_statement()
{
 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)

 // //  this should mean always a "retval symbol;"
  //    or "retval literal;" kind of statement
 if(flags.holding_retval_node)
 {
  flags.holding_retval_node = false;
  if(held_retval_follow_node_)
  {
   add_token_node(held_retval_node_);
   add_token_node(held_retval_follow_node_);
   held_retval_node_ << Cf/Qy.Retval_Follow >> held_retval_follow_node_;
  }
  held_retval_node_ = nullptr;
  held_retval_follow_node_ = nullptr;
 }

 if(current_function_def_entry_node_)
 {
  graph_build_->complete_function_declaration();
  return;
 }

 reset_if_block_pending_follow();
 read_chiefs();
 check_cancel_implied_call_entry();

 if(!chiefs_.isEmpty())
 {
  caon_ptr<ChasmRZ_Node> chiefs_top = chiefs_.top();
  CAON_PTR_DEBUG(ChasmRZ_Node ,chiefs_top)
  CAON_DEBUG_NOOP
 }


 switch(position_state_)
 {
 case Position_States::Active_Run_Token:
  check_pop_chief();
  position_state_ = Position_States::Cross_Run_Chief;
  break;
 case Position_States::Cross_Run_Chief:
 case Position_States::Active_Run_Chief:
  {
   CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
   check_pop_chief();
   if(current_do_map_block_entry_node_)
   {
    CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
    CAON_DEBUG_NOOP
   }
   position_state_ = Position_States::Cross_Run_Chief;
  }
  break;
 default:
  //? syntax error?
  break;
 }
}



caon_ptr<ChasmRZ_Node>
 ChasmRZ_ASG_Position::check_insert_function_def_entry_node(const ChasmRZ_Tuple_Info& tuple_info)
{
 // //    If the entry is a function definition, create a new Fun_Def node.

 //       Note: currently this only checks .(); later we need to
 //       check for any channel before a -> ...

 if(position_state_ == Position_States::Active_Run_Chief)
 {
  return nullptr;
 }

 if(tuple_info.indicates_function_def_entry())
 {
  return check_add_function_def_entry_node_with_label();
 }
 return nullptr;
}

caon_ptr<ChasmRZ_Node> ChasmRZ_ASG_Position::check_add_function_def_entry_node_with_label()
{
 caon_ptr<ChasmRZ_Node> result;

 if(last_function_definition_arrow_node_)
 {
  // // this is only for do-maps
  if(held_do_mapkey_node_)
  {
   result = graph_build_->new_function_def_entry_node(*last_function_definition_arrow_node_,
     ChasmRZ_Function_Def_Kinds::N_A, held_do_mapkey_node_);
  }

  else
  {
   result = graph_build_->new_function_def_entry_node(*current_node_,
     ChasmRZ_Function_Def_Kinds::N_A);
  }
 }
 else
 {
  result = graph_build_->new_function_def_entry_node(*current_node_,
    ChasmRZ_Function_Def_Kinds::N_A, held_do_mapkey_node_);
 }
 if(held_do_mapkey_node_)
 {
  // //  Nothing else to do here, for now ...
  held_do_mapkey_node_ = nullptr;
 }
 return result;
}

void ChasmRZ_ASG_Position::check_add_retval_nodes(caon_ptr<ChasmRZ_Node> node)
{
 if(flags.holding_retval_node)
 {
  flags.holding_retval_node = false;
  if(held_retval_follow_node_)
  {
   add_token_node(held_retval_follow_node_);
   held_retval_follow_node_ << Cf/Qy.Retval_Follow >> node;
  }
  else
  {
   add_token_node(held_retval_node_);
   held_retval_node_ << Cf/Qy.Retval_Follow >> node;
  }
  held_retval_node_ = nullptr;
  held_retval_follow_node_ = nullptr;
 }
}

caon_ptr<ChasmRZ_Node> ChasmRZ_ASG_Position::check_add_retval_nodes()
{
 caon_ptr<ChasmRZ_Node> result = nullptr;
 if(flags.holding_retval_node)
 {
  flags.holding_retval_node = false;
  if(held_retval_follow_node_)
  {
   add_token_node(held_retval_follow_node_);
   result = held_retval_follow_node_;
  }
  else
  {
   add_token_node(held_retval_node_);
   result = held_retval_node_;
  }
  held_retval_node_ = nullptr;
  held_retval_follow_node_ = nullptr;
 }
 return result;
}

void ChasmRZ_ASG_Position::add_data_entry(caon_ptr<ChasmRZ_Node> tuple_info_node)
{
 check_add_retval_nodes(tuple_info_node);

 CAON_PTR_DEBUG(ChasmRZ_Node ,tuple_info_node)
 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)

 if(caon_ptr<ChasmRZ_Tuple_Info> ti = tuple_info_node->chasm_rz_tuple_info())
 {
  if(!ti->indicates_function_def_entry())
  {
   check_add_implied_call_entry();
  }
 }


 bool was_pending_equalizer_value = flags.pending_equalizer_value;
 flags.pending_equalizer_value = false;


 read_chiefs();


 caon_ptr<ChasmRZ_Node> new_node = check_insert_function_def_entry_node(*tuple_info_node->chasm_rz_tuple_info());
 if(new_node)
 {
  CAON_PTR_DEBUG(ChasmRZ_Node ,new_node)
  CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
  if(caon_ptr<ChasmRZ_Call_Entry> rce = current_node_->chasm_rz_call_entry())
  {
   CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)
   if(rce->flags.has_do_map)
   {
// //  Actually, the sequence should be defined between the arrow token nodes ...
//  if(last_function_definition_arrow_node_)
//    last_function_definition_arrow_node_ << Cf/Qy.Run_Fundef_Map_Key_Sequence >> new_node;

// // This can be used to verify syntax ...
//    if(caon_ptr<ChasmRZ_Node> n = Qy.Run_Call_Entry(current_node_))
//    {
//     CAON_PTR_DEBUG(ChasmRZ_Node ,n)
//     if(caon_ptr<ChasmRZ_Node> n1 = Qy.Run_Call_Sequence(n))
//     {
//      // //  n1 should be arrow token ...
//      CAON_PTR_DEBUG(ChasmRZ_Node ,n1)
//      CAON_DEBUG_NOOP
//     }
//    }
   }

  }

  current_function_def_entry_node_ = new_node;
  current_node_ = new_node;
 }

 else if(was_pending_equalizer_value)
 {
  add_new_node_as_implied_call_entry();
 }

 // //  Reusing new_node ...
 switch(position_state_)
 {
 case Position_States::Active_Run_Token:
  new_node = insert_entry_node(Qy.Run_Call_Entry, false);
  new_node << Cf/Qy.Run_Data_Entry >> tuple_info_node;
  break;

 case Position_States::Active_Run_Chief:
  new_node = insert_entry_node(Qy.Run_Cross_Data, false);
  new_node << Cf/Qy.Run_Data_Entry >> tuple_info_node;
  break;

 case Position_States::Block_Entry:
  new_node = insert_entry_node(Qy.Run_Call_Entry, false);
  new_node << Cf/Qy.Run_Data_Entry >> tuple_info_node;
  break;

 case Position_States::Cross_Run_Chief:
  {
   new_node = insert_entry_node(Qy.Run_Call_Entry, false);
   new_node << Cf/Qy.Run_Data_Entry >> tuple_info_node;
   break;
  }

 }
 CAON_PTR_DEBUG(ChasmRZ_Node ,new_node)

 if(caon_ptr<ChasmRZ_Call_Entry> rce = new_node->chasm_rz_call_entry())
 {
  CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)
  rce->flags.is_tuple_info_entry = true;
 }

 position_state_ = Position_States::Data_Entry;
 current_node_ = tuple_info_node;
 chiefs_.append(current_node_);
}


void ChasmRZ_ASG_Position::read_over_chiefs()
{
 QVectorIterator<caon_ptr<ChasmRZ_Node>> it(over_chiefs_);
 while(it.hasNext())
 {
  caon_ptr<ChasmRZ_Node> n = it.next();
  CAON_PTR_DEBUG(ChasmRZ_Node ,n)
  if(caon_ptr<ChasmRZ_Node> nn = Qy.Run_Call_Entry(in_Cf n))
  {
   CAON_PTR_DEBUG(ChasmRZ_Node ,nn)
   if(caon_ptr<ChasmRZ_Token> tok = nn->chasm_rz_token())
   {
    CAON_PTR_DEBUG(ChasmRZ_Token ,tok)
    qDebug() << tok->string_value();
   }
  }
  else if(caon_ptr<ChasmRZ_Node> dn = Qy.Run_Data_Entry(in_Cf n))
  {
   if(caon_ptr<ChasmRZ_Tuple_Info> rti = dn->chasm_rz_tuple_info())
   {
    CAON_PTR_DEBUG(ChasmRZ_Tuple_Info ,rti)
    qDebug() << rti->asg_out();
   }
  }
 }
}

void ChasmRZ_ASG_Position::read_block_chiefs()
{
 QVectorIterator<caon_ptr<ChasmRZ_Node>> it(block_chiefs_);
 while(it.hasNext())
 {
  caon_ptr<ChasmRZ_Node> n = it.next();
  CAON_PTR_DEBUG(ChasmRZ_Node ,n)
  if(caon_ptr<ChasmRZ_Node> nn = Qy.Run_Call_Entry(in_Cf n))
  {
   CAON_PTR_DEBUG(ChasmRZ_Node ,nn)
   if(caon_ptr<ChasmRZ_Token> tok = nn->chasm_rz_token())
   {
    CAON_PTR_DEBUG(ChasmRZ_Token ,tok)
    qDebug() << tok->string_value();
   }
  }
  else if(caon_ptr<ChasmRZ_Node> dn = Qy.Run_Data_Entry(in_Cf n))
  {
   if(caon_ptr<ChasmRZ_Tuple_Info> rti = dn->chasm_rz_tuple_info())
   {
    CAON_PTR_DEBUG(ChasmRZ_Tuple_Info ,rti)
    qDebug() << rti->asg_out();
   }
  }
 }
}


void ChasmRZ_ASG_Position::read_chiefs()
{
 QVectorIterator<caon_ptr<ChasmRZ_Node>> it(chiefs_);
 while(it.hasNext())
 {
  caon_ptr<ChasmRZ_Node> n = it.next();
  CAON_PTR_DEBUG(ChasmRZ_Node ,n)
  if(caon_ptr<ChasmRZ_Node> nn = Qy.Run_Call_Entry(in_Cf n))
  {
   CAON_PTR_DEBUG(ChasmRZ_Node ,nn)
   if(caon_ptr<ChasmRZ_Token> tok = nn->chasm_rz_token())
   {
    CAON_PTR_DEBUG(ChasmRZ_Token ,tok)
    CAON_DEBUG_NOOP
   }
  }
  else if(caon_ptr<ChasmRZ_Node> dn = Qy.Run_Data_Entry(in_Cf n))
  {
   if(caon_ptr<ChasmRZ_Tuple_Info> rti = dn->chasm_rz_tuple_info())
   {
    CAON_PTR_DEBUG(ChasmRZ_Tuple_Info ,rti)
   }

  }
 }

}

void ChasmRZ_ASG_Position::add_data_leave(caon_ptr<ChasmRZ_Node> tuple_info_node)
{
 CAON_PTR_DEBUG(ChasmRZ_Node ,tuple_info_node)

 read_chiefs();

 CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)

 current_node_ = pop_chief();

 caon_ptr<ChasmRZ_Node> pop_node = pop_chief();
 CAON_PTR_DEBUG(ChasmRZ_Node ,pop_node)

 if(caon_ptr<ChasmRZ_Tuple_Info> rti = tuple_info_node->chasm_rz_tuple_info())
 {
  CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
  CAON_PTR_DEBUG(ChasmRZ_Tuple_Info ,rti)
  rti->set_call_entry_node(current_node_);
 }
 if(caon_ptr<ChasmRZ_Tuple_Info> ce_rti = current_node_->chasm_rz_tuple_info())
 {
  CAON_PTR_DEBUG(ChasmRZ_Tuple_Info ,ce_rti)
  CAON_DEBUG_NOOP
  ce_rti->set_call_entry_node(pop_node);

  if(caon_ptr<ChasmRZ_Node> den = Qy.Run_Data_Entry(in_Cf current_node_))
  {
   CAON_PTR_DEBUG(ChasmRZ_Node ,den)
   CAON_DEBUG_NOOP
  }
  else
  {
   ce_rti->mark_as_empty();
   // //?  Is it better to use a dummy or just rely on graph structure?
   caon_ptr<ChasmRZ_Node> new_node = graph_build_->make_new_empty_tuple_node(ce_rti);
   current_node_ << Cf/Qy.Run_Data_Entry >> new_node;
   current_node_ = new_node;
  }
 }

 current_node_ << Cf/Qy.Run_Data_Leave >> tuple_info_node;

 current_node_ = pop_node;
 position_state_ = Position_States::Active_Run_Chief;
}

ChasmRZ_Tuple_Info::Tuple_Indicators
ChasmRZ_ASG_Position::data_chief_indicator()
{
 if(chiefs_.isEmpty())
  return ChasmRZ_Tuple_Info::Tuple_Indicators::N_A;

 ChasmRZ_Node& n = *chiefs_.top();

 if(caon_ptr<ChasmRZ_Tuple_Info> rti = n.chasm_rz_tuple_info())
 {
  return rti->indicator();
 }
 return ChasmRZ_Tuple_Info::Tuple_Indicators::N_A;


}

void ChasmRZ_ASG_Position::leave_expression()
{
 switch(position_state_)
 {
 case Position_States::Active_Run_Token:
 case Position_States::Active_Run_Chief:
  check_pop_chief();

  position_state_ = Position_States::Active_Run_Chief;
  break;

 case Position_States::Cross_Run_Chief:
  {

  }
  break;

 default:
  break;
 }
}

void ChasmRZ_ASG_Position::hold_assignment_annotation_node(caon_ptr<ChasmRZ_Node> aa_node)
{
 // //  Chance to check position state ...
 held_assignment_annotation_node_ = aa_node;
}

void ChasmRZ_ASG_Position::complete_function_declaration(caon_ptr<ChasmRZ_Node> arrow_node,
  caon_ptr<ChasmRZ_Node> proxy_body_node)
{
 add_arrow_token_node(arrow_node);
 add_token_node(proxy_body_node);
 leave_lexical_scope(2, "-");

 position_state_ = Position_States::Cross_Run_Chief;
}

void ChasmRZ_ASG_Position::leave_lexical_scope(int length, QString suffix)
{
  // // always necessary? ...
  // reset_if_block_pending_follow();

 // this finalization is not necessary with only an if block
 // (i.e., one with no else or elsif) ...
 if(flags.elsif_block_pending_follow)
 {
  flags.elsif_block_pending_follow = false;
  finalize_overall_if_block();
  close_statement();
 }

 if(!block_chiefs_.isEmpty())
 {
  CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)

  caon_ptr<ChasmRZ_Node> ben = block_chiefs_.top();
  if(caon_ptr<ChasmRZ_Block_Entry> rbe = ben->chasm_rz_block_entry())
  {
   CAON_PTR_DEBUG(ChasmRZ_Block_Entry ,rbe)
   CAON_DEBUG_NOOP
   if(rbe->flags.ghost_block)
   {
    if(position_state_ == Position_States::Active_Run_Token)
     close_statement();
    add_block_map_leave();
    close_statement();
    return;
   }
  }
 }

 if(flags.holding_retval_node)
  close_statement();

 switch(position_state_)
 {
 case Position_States::Active_Run_Token:
  close_statement();
  break;
 case Position_States::Cross_Run_Chief:
  {
   if(caon_ptr<ChasmRZ_Block_Entry> rbe = current_node_->chasm_rz_block_entry())
   {
    // //  In this case the statement enclosing a block
     //    will not have been closed...
    CAON_PTR_DEBUG(ChasmRZ_Block_Entry ,rbe)
    close_statement();
   }
   else if(caon_ptr<ChasmRZ_Call_Entry> rce = current_node_->chasm_rz_call_entry())
   {
    CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)
    if(rce->flags.was_added_as_implied ||
      rce->flags.was_added_as_pchasm_rz_ghost_block )
     close_statement();
   }
  }
  break;
 default:
  //?
  close_statement();
  break;
 }


 // //  It looks like the below is only for length == 1,
 //     because for length == 2 the end statements ends
 //     the enclosing statement also.
 if(length == 2)
 {
  // // what about anonymous functions which are not the end
  //    of a statement? ...
  if(current_do_map_block_entry_node_)
  {
   // // No close statement in this case ...
  }
  else if(suffix == ",")
  {
   read_chiefs();
   check_hold_closed_do_entry();

   CAON_PTR_DEBUG(ChasmRZ_Node ,current_closed_do_entry_node_)
   {
    CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
    CAON_DEBUG_NOOP
   }
  }
  else if(suffix == ".")
  {

  }
  else
  {
   close_statement();
   {
    CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
    CAON_DEBUG_NOOP
   }
  }

  if(caon_ptr<ChasmRZ_Call_Entry> rce = current_node_->chasm_rz_call_entry())
  {
   CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
   CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)
   if(rce->flags.is_do_lambda)
   {
    if(rce->flags.has_do_map)
    {

    }
    else
    {
     check_pop_chief();
    }
   }
  }

  // check if we're ending a do sequence ...
  if(suffix.isEmpty())
  {
   CAON_PTR_DEBUG(ChasmRZ_Node ,current_node_)
   if(caon_ptr<ChasmRZ_Call_Entry> rce = current_node_->chasm_rz_call_entry())
   {
    CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)
    if(rce->flags.is_do_closed_with_follow)
    {
     check_pop_chief();
    }
   }
  }

  if(position_state_ != Position_States::Active_Closed_Do_Entry)
    position_state_ = Position_States::Cross_Run_Chief;
  return;
 }

 // // currently length should be <= 2...
 switch(position_state_)
 {
 case Position_States::Active_Run_Token:

  read_block_chiefs();

  current_node_ = over_chiefs_.pop();
  position_state_ = Position_States::Cross_Run_Chief;
  break;

 case Position_States::Cross_Run_Chief:
 {
  read_block_chiefs();

  caon_ptr<ChasmRZ_Node> bc_node = block_chiefs_.pop();

  CAON_PTR_DEBUG(ChasmRZ_Node ,bc_node)
  if(caon_ptr<ChasmRZ_Block_Entry> rbe = bc_node->chasm_rz_block_entry())
  {
   CAON_PTR_DEBUG(ChasmRZ_Block_Entry ,rbe)
   current_node_ = rbe->statement_entry_node();
   position_state_ = Position_States::Cross_Run_Chief;
  }
  break;
 }

 default:
  //? syntax error?
  break;
 }
}


