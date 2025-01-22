
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "chasm-rz-graph-build.h"

USING_RZNS(RZ_Core)


#include "chasm-rz-graph-build.h"

#include "kernel/graph/chasm-rz-node.h"

#include "kernel/chasm-rz-root.h"

#include "chasm-rz-grammar.h"

#include "code/chasm-rz-call-entry.h"
#include "code/chasm-rz-casement-call-entry.h"

#include "code/chasm-rz-block-entry.h"
#include "code/chasm-rz-casement-block-entry.h"

#include "kernel/graph/chasm-rz-graph.h"

#include "kernel/chasm-rz-dominion.h"


#include "token/chasm-rz-token.h"


#include "kernel/document/chasm-rz-document.h"

#include "code/chasm-rz-code-representation.h"

#include "code/chasm-rz-function-def-entry.h"

#include "chasm-rz-string-plex-builder.h"

#include "rzns.h"
USING_RZNS(RZ_Core)


ChasmRZ_Graph_Build::ChasmRZ_Graph_Build(ChasmRZ_Document* d,
  ChasmRZ_Parser& p, ChasmRZ_Graph& g)
 : Flags(0)
  ,document_(d)
  ,graph_(g)
  ,parser_(p)
  ,current_line_(1)
  ,Cf(ChasmRZ_Frame::instance("casement"))
  ,Sf(ChasmRZ_Frame::instance("semantic"))
  ,asg_position_(this)
  ,active_run_node_(nullptr)
  ,active_chief_token_(nullptr)
  ,current_run_comment_left_(0)
  ,current_run_comment_right_(0)
  ,call_entry_count_(0)
  ,block_entry_count_(0)
  ,tuple_entry_count_(0)
  ,current_string_plex_builder_(nullptr)
{
}

void ChasmRZ_Graph_Build::init()
{
 caon_ptr<ChasmRZ_Root> root = new ChasmRZ_Root(document_);
 active_run_node_ = root_node_ = new ChasmRZ_Node(root);

 RELAE_SET_NODE_LABEL(root_node_, "<root>");
 graph_.set_root_node(root_node_);
 asg_position_.set_current_node(root_node_);

 QString sct = "_asg";
 QStringList sctl = sct.split(' ');

 for(QString s : sctl)
  automatic_statement_closing_tokens_.insert(s);

}


void ChasmRZ_Graph_Build::declare_lexical_symbol(QString raw_text)
{
 caon_ptr<ChasmRZ_Token> token = new ChasmRZ_Token(raw_text);
 CAON_PTR_DEBUG(ChasmRZ_Token ,token)
 caon_ptr<ChasmRZ_Node> token_node = make_new_node(token);

 caon_ptr<ChasmRZ_Token> observer = new ChasmRZ_Token("lex-let");
 CAON_PTR_DEBUG(ChasmRZ_Token ,observer)
 caon_ptr<ChasmRZ_Node> observer_node = make_new_node(observer);

 asg_position_.add_casement_entry(observer_node);
 asg_position_.add_casement_sequence(token_node);



 // <<

}



void ChasmRZ_Graph_Build::check_run_comment_begin(int left, int right)
{
 if(current_run_comment_left_)
 {
  return;
 }
 if(left > 6)
  left = 6;
 if(right > 6)
  right = 6;
 current_run_comment_left_ = left;
 current_run_comment_right_ = right;
 parse_context_.flags.inside_run_comment = true;
}

void ChasmRZ_Graph_Build::check_run_comment_end(int left, int right)
{
 if(current_run_comment_left_)
 {
  if(left > 6)
   left = 6;
  if(right > 6)
   right = 6;
  if(current_run_comment_left_ != right)
   return;
  if(current_run_comment_right_ != left)
   return;

  current_run_comment_left_ = 0;
  current_run_comment_right_ = 0;
  parse_context_.flags.inside_run_comment = false;
 }
 // // If not, for now just ignore ...
 //    Otherwise syntax error ...
}


caon_ptr<ChasmRZ_Node> ChasmRZ_Graph_Build::make_new_node(caon_ptr<ChasmRZ_Token> token)
{
 caon_ptr<ChasmRZ_Node> result = new ChasmRZ_Node(token);
 RELAE_SET_NODE_LABEL(result, token->string_summary());
 return result;
}


caon_ptr<ChasmRZ_Node> ChasmRZ_Graph_Build::make_new_node(caon_ptr<RZ_String_Plex_Builder> rzspb)
{
 caon_ptr<ChasmRZ_Node> result = new ChasmRZ_Node(rzspb);

 return result;
}


caon_ptr<ChasmRZ_Node> ChasmRZ_Graph_Build::make_new_node(caon_ptr<ChasmRZ_Casement_Call_Entry> rce)
{
 caon_ptr<ChasmRZ_Node> result = new ChasmRZ_Node(rce);
 RELAE_SET_NODE_LABEL(result, QString("<casement-call %1>").arg(rce->call_id()));
 return result;
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Graph_Build::make_new_node(caon_ptr<ChasmRZ_Call_Entry> rce)
{
 caon_ptr<ChasmRZ_Node> result = new ChasmRZ_Node(rce);
 RELAE_SET_NODE_LABEL(result, QString("<call %1>").arg(rce->call_id()));
 return result;
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Graph_Build::make_new_node(caon_ptr<ChasmRZ_Function_Def_Entry> fdef)
{
 caon_ptr<ChasmRZ_Node> result = new ChasmRZ_Node(fdef);
 RELAE_SET_NODE_LABEL(result, "<fdef>");
 return result;
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Graph_Build::make_new_node(caon_ptr<ChasmRZ_Block_Entry> rbe)
{
 caon_ptr<ChasmRZ_Node> result = new ChasmRZ_Node(rbe);
 RELAE_SET_NODE_LABEL(result, QString("<block %1>").arg(rbe->block_id()));
 return result;
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Graph_Build::make_new_node(caon_ptr<ChasmRZ_Casement_Block_Entry> cbe)
{
 caon_ptr<ChasmRZ_Node> result = new ChasmRZ_Node(cbe);
 RELAE_SET_NODE_LABEL(result, QString("<block %1>").arg(cbe->block_id()));
 return result;
}

void ChasmRZ_Graph_Build::add_assignment_annotation(QString text)
{
 caon_ptr<ChasmRZ_Token> aa_token = new ChasmRZ_Token(text);
 caon_ptr<ChasmRZ_Node> aa_node = make_new_node(aa_token);
 asg_position_.hold_assignment_annotation_node(aa_node);
}

void ChasmRZ_Graph_Build::check_line_increment(QString text)
{
 if(text.endsWith('\\n'))
  line_increment();
}

void ChasmRZ_Graph_Build::line_increment()
{
 ++current_line_;
}

void ChasmRZ_Graph_Build::set_expected_token_end(QString text)
{
 expected_token_end_ = text;
 expected_token_end_.chop(1);
 expected_token_end_.prepend('_');
}


void ChasmRZ_Graph_Build::complete_function_declaration()
{
 caon_ptr<ChasmRZ_Token> arrow_token = new ChasmRZ_Token("->def", "", "");
 caon_ptr<ChasmRZ_Node> arrow_node = make_new_node(arrow_token);

 caon_ptr<ChasmRZ_Token> token = new ChasmRZ_Token("<undef-function-body>", "", "");
 caon_ptr<ChasmRZ_Node> node = make_new_node(token);

 asg_position_.complete_function_declaration(arrow_node, node);


}

void ChasmRZ_Graph_Build::add_semis(QString raw_text, QString suffix,
                               QString space_to_end_of_line)
{
 int length = raw_text.length();
 if(length > 6)
 {
  raw_text.truncate(6);
  length = 6;
 }

 if(flags.added_lr_mode)
 {
  flags.added_lr_mode = false;
  asg_position_.leave_expression();
 }

 switch(length)
 {
 case 1:
  if(suffix.isEmpty())
    asg_position_.close_statement();
  else
    asg_position_.leave_lexical_scope(2, suffix);
  return;
 case 2:
  asg_position_.leave_lexical_scope(2, suffix);
  return;
 case 5:
  terminate_parse();
  // fallthrough
 case 6:
  terminate_read();
  // fallthrough
 default:
  add_run_token(raw_text, space_to_end_of_line);
 }
}

void ChasmRZ_Graph_Build::terminate_read()
{

}

void ChasmRZ_Graph_Build::terminate_parse()
{

}

void ChasmRZ_Graph_Build::string_plex_switch(QString key)
{
 current_string_plex_builder_->add_part(string_literal_acc_.trimmed());
 current_string_plex_builder_->set_current_key(key);
 string_literal_acc_.clear();

}

caon_ptr<ChasmRZ_Node> ChasmRZ_Graph_Build::close_string_plex()
{
 current_string_plex_builder_->add_part(string_literal_acc_.trimmed());
 string_literal_acc_.clear();
 caon_ptr<ChasmRZ_Node> result = make_new_node(current_string_plex_builder_);
 current_string_plex_builder_ = nullptr;
 return result;

}

void ChasmRZ_Graph_Build::string_plex_acc(QString text)
{
 string_literal_acc_ += text;
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Graph_Build::create_tuple(ChasmRZ_Tuple_Info::Tuple_Formations tf,
 ChasmRZ_Tuple_Info::Tuple_Indicators ti, ChasmRZ_Tuple_Info::Tuple_Formations Sf, bool increment_id)
{
 int tuple_id;
 if(increment_id)
  tuple_id = ++tuple_entry_count_;
 else
  tuple_id = 0;

 caon_ptr<ChasmRZ_Tuple_Info> tinfo = new ChasmRZ_Tuple_Info(tf, ti, tuple_id);
 caon_ptr<ChasmRZ_Node> result = new ChasmRZ_Node(tinfo);

 result->set_label(QString("<tinfo %1>").arg(tuple_id));
 return result;
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Graph_Build::make_new_empty_tuple_node(caon_ptr<ChasmRZ_Tuple_Info> rti)
{
 QString rep = rti->token_representation();
 caon_ptr<ChasmRZ_Token> token = new ChasmRZ_Token(rep);
 token->flags.is_empty_tuple_indicator = true;
 caon_ptr<ChasmRZ_Node> result = make_new_node(token);
 result->set_label(rep);
 return result;
}

void ChasmRZ_Graph_Build::check_hold_tuple_suffix(ChasmRZ_Tuple_Info::Tuple_Indicators ti, QString& suffix)
{
 if(suffix.isEmpty())
  return;
 switch(ti)
 {
 case ChasmRZ_Tuple_Info::Tuple_Indicators::Enter_Array:
 case ChasmRZ_Tuple_Info::Tuple_Indicators::Enter_Vector:
 case ChasmRZ_Tuple_Info::Tuple_Indicators::Enter_Map:
 case ChasmRZ_Tuple_Info::Tuple_Indicators::Enter_Set:
  held_suffix_ = suffix;
  parse_context_.flags.take_a_space = true;
  suffix = QString();
  break;
 default:
  break;
 }
}

void ChasmRZ_Graph_Build::finalize_take_a_space()
{
 QString hs = held_suffix_;
 held_suffix_ = QString();
 parse_context_.flags.take_a_space = false;
 add_run_token(hs);
}


void ChasmRZ_Graph_Build::enter_tuple(QString name, QString prefix, QString entry, QString suffix)
{
 ChasmRZ_Tuple_Info::Tuple_Formations tf = ChasmRZ_Tuple_Info::get_tuple_formation(prefix);
 ChasmRZ_Tuple_Info::Tuple_Indicators ti = ChasmRZ_Tuple_Info::get_tuple_indicator(entry);

 // //  want to hold suffix character if ti is an entry ...
 check_hold_tuple_suffix(ti, suffix);

 ChasmRZ_Tuple_Info::Tuple_Formations Sf = ChasmRZ_Tuple_Info::get_tuple_formation(suffix);

 int tuple_id;



 if(tf == ChasmRZ_Tuple_Info::Tuple_Formations::Indicates_Declarations)
 {
  if(asg_position_.current_entry_is_backquoted())
  {
   if(ti == ChasmRZ_Tuple_Info::Tuple_Indicators::Enter_Array)
   {
    asg_position_.add_call_entry(false, prefix);
    return;
   }
   else
   {
    tf = ChasmRZ_Tuple_Info::Tuple_Formations::Indicates_Unquote;
   }
  }
 }

 if(tf == ChasmRZ_Tuple_Info::Tuple_Formations::N_A)
 {
  if(ti == ChasmRZ_Tuple_Info::Tuple_Indicators::Enter_Array)
  {
   asg_position_.add_call_entry(false, prefix);
   return;
  }
  else if(ti == ChasmRZ_Tuple_Info::Tuple_Indicators::Leave_Array)
  {
   ChasmRZ_Tuple_Info::Tuple_Indicators ti1 = asg_position_.data_chief_indicator();
   if(ti1 == ChasmRZ_Tuple_Info::Tuple_Indicators::N_A)
   {
    asg_position_.leave_expression();
    return;
   }
  }
  else if(ti == ChasmRZ_Tuple_Info::Tuple_Indicators::Enter_Map)
  {
   asg_position_.add_block_map_entry();
   return;
  }
  else if(ti == ChasmRZ_Tuple_Info::Tuple_Indicators::Leave_Map)
  {
   if(caon_ptr<ChasmRZ_Tuple_Info> rti = asg_position_.current_tuple_info())
   {
    if(rti->indicator() == ChasmRZ_Tuple_Info::Tuple_Indicators::Enter_Map)
     goto good_to_go;
   }
   asg_position_.add_block_map_leave();
   return;
  }
  else
  {
   tuple_id = 0;
  }
 }
 else
good_to_go:
  tuple_id = ++tuple_entry_count_;


 caon_ptr<ChasmRZ_Tuple_Info> tinfo = new ChasmRZ_Tuple_Info(tf, ti, tuple_id);
 caon_ptr<ChasmRZ_Node> tinfo_node = new ChasmRZ_Node(tinfo);

 tinfo_node->set_label(QString("<tinfo %1>").arg(tuple_id));

 switch(ti)
 {
 case ChasmRZ_Tuple_Info::Tuple_Indicators::Enter_Vector:
  if(tf == ChasmRZ_Tuple_Info::Tuple_Formations::Indicates_Plex)
  {
   parse_context_.flags.inside_string_plex = true;
   current_string_plex_builder_ = new RZ_String_Plex_Builder(tinfo_node);
  }
  // fallthrough
 case ChasmRZ_Tuple_Info::Tuple_Indicators::Enter_Array:
  {
   if(tf != ChasmRZ_Tuple_Info::Tuple_Formations::N_A)
   {
    flags.inside_array_with_formation = true;
   }
  }
  // fallthrough
 case ChasmRZ_Tuple_Info::Tuple_Indicators::Enter_Map:
 case ChasmRZ_Tuple_Info::Tuple_Indicators::Enter_Set:
 case ChasmRZ_Tuple_Info::Tuple_Indicators::Enter_Annotation:
  asg_position_.add_data_entry(tinfo_node);
  break;

 case ChasmRZ_Tuple_Info::Tuple_Indicators::Leave_Vector:
  if(Sf == ChasmRZ_Tuple_Info::Tuple_Formations::Indicates_Plex)
  {
   caon_ptr<ChasmRZ_Node> string_plex_node = close_string_plex();
   CAON_PTR_DEBUG(ChasmRZ_Node ,tinfo_node)
   asg_position_.add_string_plex_node(tinfo_node, string_plex_node);
   parse_context_.flags.inside_string_plex = false;
  }
  //  fallthrough

 case ChasmRZ_Tuple_Info::Tuple_Indicators::Leave_Array:
  flags.inside_array_with_formation = false;
  //  fallthrough
 case ChasmRZ_Tuple_Info::Tuple_Indicators::Leave_Map:
 case ChasmRZ_Tuple_Info::Tuple_Indicators::Leave_Set:
 case ChasmRZ_Tuple_Info::Tuple_Indicators::Leave_Annotation:
  asg_position_.add_data_leave(tinfo_node);
  break;
 }
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Graph_Build::new_function_def_entry_node(ChasmRZ_Node& prior_node,
  ChasmRZ_Function_Def_Kinds kind, caon_ptr<ChasmRZ_Node> label_node)
{
 caon_ptr<ChasmRZ_Function_Def_Entry> fdef = new ChasmRZ_Function_Def_Entry(&prior_node, kind, label_node);
 caon_ptr<ChasmRZ_Node> result = make_new_node(fdef);
 fdef->set_node(result);
 return result;
}

void ChasmRZ_Graph_Build::add_run_token(QString raw_text, QString space_to_end_of_line)
{
 add_run_token(QString(), raw_text, QString(), Token_Formations::Normal, space_to_end_of_line);
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Graph_Build::new_autogenerated_token_node(QString raw_text)
{
 caon_ptr<ChasmRZ_Token> token = new ChasmRZ_Token(raw_text);
 CAON_PTR_DEBUG(ChasmRZ_Token ,token)
 caon_ptr<ChasmRZ_Node> result = make_new_node(token);
 return result;
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Graph_Build::new_run_call_entry_node(bool is_statement_entry,
  QString prefix, caon_ptr<ChasmRZ_Call_Entry> parent_entry)
{
 caon_ptr<ChasmRZ_Call_Entry> new_entry = new ChasmRZ_Call_Entry(call_entry_count_, prefix);

 CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,new_entry)

 if(is_statement_entry)
  new_entry->flags.is_statement_entry = true;

 if(parent_entry)
 {
  CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,parent_entry)
  if(!new_entry->flags.no_normalize)
   new_entry->flags.no_normalize = parent_entry->flags.no_normalize;
  if(!new_entry->flags.no_anticipate)
   new_entry->flags.no_anticipate = parent_entry->flags.no_anticipate;
 }

 ++call_entry_count_;
 caon_ptr<ChasmRZ_Node> result = make_new_node(new_entry);
 return result;
}


caon_ptr<ChasmRZ_Node> ChasmRZ_Graph_Build::new_run_casement_entry_node(bool is_statement_entry,
  QString prefix, caon_ptr<ChasmRZ_Casement_Call_Entry> parent_entry)
{
 caon_ptr<ChasmRZ_Casement_Call_Entry> new_entry = new ChasmRZ_Casement_Call_Entry(call_entry_count_, prefix);

 CAON_PTR_DEBUG(ChasmRZ_Casement_Call_Entry ,new_entry)

 if(is_statement_entry)
  new_entry->flags.is_statement_entry = true;

 if(parent_entry)
 {
  CAON_PTR_DEBUG(ChasmRZ_Casement_Call_Entry ,parent_entry)
  if(!new_entry->flags.no_normalize)
   new_entry->flags.no_normalize = parent_entry->flags.no_normalize;
  if(!new_entry->flags.no_anticipate)
   new_entry->flags.no_anticipate = parent_entry->flags.no_anticipate;
 }

 ++call_entry_count_;
 caon_ptr<ChasmRZ_Node> result = make_new_node(new_entry);
 return result;
}


caon_ptr<ChasmRZ_Node> ChasmRZ_Graph_Build::new_casement_block_entry_node()
{
 caon_ptr<ChasmRZ_Casement_Block_Entry> new_entry = new ChasmRZ_Casement_Block_Entry(block_entry_count_);
 ++block_entry_count_;
 caon_ptr<ChasmRZ_Node> result = make_new_node(new_entry);
 return result;
}


caon_ptr<ChasmRZ_Node> ChasmRZ_Graph_Build::new_run_block_entry_node()
{
 caon_ptr<ChasmRZ_Block_Entry> new_entry = new ChasmRZ_Block_Entry(block_entry_count_);
 ++block_entry_count_;
 caon_ptr<ChasmRZ_Node> result = make_new_node(new_entry);
 return result;
}


void ChasmRZ_Graph_Build::add_to_raw_asg(QString str)
{
 raw_asg_acc_ += str;
}

void ChasmRZ_Graph_Build::process_raw_asg()
{
 caon_ptr<ChasmRZ_Token> token = new ChasmRZ_Token(raw_asg_acc_);
 raw_asg_acc_.clear();
 token->flags.is_raw_asg = true;
 add_raw_asg_token(*token);
}


void ChasmRZ_Graph_Build::process_string_literal()
{
 caon_ptr<ChasmRZ_Token> token = new ChasmRZ_Token(string_literal_acc_);
 string_literal_acc_.clear();
 token->flags.is_string_literal = true;
 add_run_token(*token);
}


void ChasmRZ_Graph_Build::string_literal_start()
{
 parse_context_.flags.inside_string_literal = true;
}


void ChasmRZ_Graph_Build::raw_asg_start()
{
 parse_context_.flags.inside_raw_asg = true;
}

void ChasmRZ_Graph_Build::add_run_token(ChasmRZ_Token& token)
{
 caon_ptr<ChasmRZ_Node> node = make_new_node(&token);
 asg_position_.add_token_node(node);
}

void ChasmRZ_Graph_Build::add_raw_asg_token(ChasmRZ_Token& token)
{
 caon_ptr<ChasmRZ_Node> node = make_new_node(&token);
 asg_position_.add_raw_asg_token(node);
}

void ChasmRZ_Graph_Build::add_token_plus_block(QString token, QString block_entry)
{
 // //  Currently this only has token == "do", "in", or "/do/", block_entry = ".."
 if(token.startsWith('/'))
 {
  token.remove(0, 1).chop(1);
  add_run_token("", token, "", Token_Formations::Strong_Do_Plus_Block);
 }
 else
 {
  if(token != "do")
  {
   if(token == "in")
   {
    if(flags.ghost_in_closes_expression)
    {
     flags.ghost_in_closes_expression = false;
     asg_position_.leave_expression();
    }
   }
   token.prepend(';').append(';');
  }
  add_run_token("", token, "", Token_Formations::Do_Plus_Block);
 }
 asg_position_.add_block_map_entry();
}


void ChasmRZ_Graph_Build::add_run_token(QString raw_text)
{
 add_run_token(QString(), raw_text, QString(), Token_Formations::Normal);
}

void ChasmRZ_Graph_Build::add_equalizer_to_type(QString raw_text)
{
 if(asg_position_.current_node_is_symbol_declaration())
 {
  add_run_token(raw_text);
 }
 else
 {
  flags.pending_convert_to_type_symbol = true;
 }
}


void ChasmRZ_Graph_Build::add_type_indicator(QString raw_text)
{
 if(flags.inside_array_with_formation)
 {
  add_run_token(raw_text);
  return;
 }
 caon_ptr<ChasmRZ_Token> token = new ChasmRZ_Token(raw_text);
 CAON_PTR_DEBUG(ChasmRZ_Token ,token)
 caon_ptr<ChasmRZ_Node> node = make_new_node(token);
 asg_position_.add_type_indicator(node);
}


void ChasmRZ_Graph_Build::add_run_token(QString prefix, QString raw_text,
 QString suffix, Token_Formations tf, QString space_to_end_of_line)
{
 bool matches_expected_token_end = false;
 bool this_token_closes_expression = false;

 if(flags.next_token_closes_expression)
 {
  flags.next_token_closes_expression = false;
  this_token_closes_expression = true;
 }

 bool raw_text_to_type_symbol = false;

 if(raw_text.startsWith("$:"))
 {
  raw_text.replace(0, 2, ":|");
  raw_text.append('|');
  raw_text_to_type_symbol = true;
 }

 if(flags.pending_convert_to_type_symbol)
 {
  flags.pending_convert_to_type_symbol = false;

  if(!raw_text_to_type_symbol)
  {
   raw_text.prepend(":|");
   raw_text.append('|');
  }
 }

 caon_ptr<ChasmRZ_Token> token = new ChasmRZ_Token(raw_text, prefix, suffix);

 CAON_PTR_DEBUG(ChasmRZ_Token ,token)

 if(token->flags.is_symbol_declaration)
 {
  asg_position_.check_add_implied_my();
 }

 caon_ptr<ChasmRZ_Node> node = make_new_node(token);

 if(tf == Token_Formations::Cpp_Scoped)
 {
  token->flags.is_cpp_scoped = true;
 }
 else if(tf == Token_Formations::Do_Plus_Block)
 {
  token->flags.is_do = false;
  token->flags.precedes_ghost_scope = true;
  token->flags.not_entry = true;
 }
 else if(tf == Token_Formations::Strong_Do_Plus_Block)
 {
  token->flags.precedes_ghost_scope = true;

  // // do we need this, for other potential /.../ formations?
   //?token->flags.strong_ghost_scope = true;
 }

 if(token->flags.is_equalizer)
 {
  asg_position_.add_equalizer_token_node(node);
 }
 else if(token->flags.is_arrow)
 {
  asg_position_.add_arrow_token_node(node);
 }
 else if(token->flags.is_mapkey)
 {
  if(token->flags.is_do_mapkey)
  {
   asg_position_.hold_do_mapkey_node(node);
  }
  else
  {
   asg_position_.hold_mapkey_node(node);
  }
 }
 else if(token->special_token() == ChasmRZ_Code_Representation::Special_Tokens::Auto_Expand_To_Null_Test)
 {
  asg_position_.add_call_entry(false);
  asg_position_.add_token_node(node);
  flags.next_token_closes_expression = true;
 }
 else if(token->special_token() == ChasmRZ_Code_Representation::Special_Tokens::Auto_Expand_To_Not_Null_Test)
 {
  asg_position_.add_call_entry(false);
  asg_position_.add_token_node(node);
  flags.next_token_closes_expression = true;
 }
 else if(token->special_token() == ChasmRZ_Code_Representation::Special_Tokens::Return_Value_Blank_Or_Identity)
 {
  asg_position_.hold_retval_node(node);
 }
 else if(raw_text.startsWith('@') && raw_text != "@" && raw_text != "@@")
 {
  if(asg_position_.awaiting_statement_call_entry())
  {
   raw_text = raw_text.mid(1);
   token->set_raw_text(raw_text);
   caon_ptr<ChasmRZ_Token> ntoken = new ChasmRZ_Token("&lr", QString(), QString());

   CAON_PTR_DEBUG(ChasmRZ_Token ,ntoken)

   caon_ptr<ChasmRZ_Node> nnode = make_new_node(ntoken);
   asg_position_.add_token_node(nnode);

   asg_position_.add_call_entry(false, "\\");
   asg_position_.add_token_node(node);

   flags.added_lr_mode = true;

  }
  else
  {
   asg_position_.add_token_node(node);
  }
 }
 else
 {
  asg_position_.add_token_node(node);
 }

 if(this_token_closes_expression)
 {
  asg_position_.leave_expression();
 }

 switch(token->special_token_kind())
 {
 case ChasmRZ_Code_Representation::Special_Token_Kind::Auto_Statement_End:
  asg_position_.close_statement();
  break;
 case ChasmRZ_Code_Representation::Special_Token_Kind::Auto_Statement_End_If_EOL:
  if(!space_to_end_of_line.isEmpty())
  {
   asg_position_.close_statement();
  }
  break;
 case ChasmRZ_Code_Representation::Special_Token_Kind::Text_Map_Leave:
  asg_position_.close_statement();
  //fallthrough...
 case ChasmRZ_Code_Representation::Special_Token_Kind::Text_Map_Leave_In_Statement:
  parse_context_.flags.inside_text_map_acc = false;
  flags.inside_text_map = false;
  break;
 case ChasmRZ_Code_Representation::Special_Token_Kind::Text_Map_Enter:
  parse_context_.flags.inside_text_map_acc = true;
  flags.inside_text_map = true;
  set_expected_token_end(raw_text);
  break;
 default:
  break;
 }


 check_line_increment(space_to_end_of_line);

}


