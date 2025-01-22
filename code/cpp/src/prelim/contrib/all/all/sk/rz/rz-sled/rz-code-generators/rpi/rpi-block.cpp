
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "rpi-block.h"
#include "rpi-stage-form.h"

#include "rz-graph-core/kernel/graph/chasm-rz-node.h"
#include "rz-graph-core/code/chasm-rz-block-entry.h"

#include "rz-graph-visit/rz-graph-visitor-phaon.h"
#include "rz-graph-token/token/rz-asg-token.h"

#include "rz-graph-valuer/scope/rz-asg-lexical-scope.h"
#include "rz-graph-valuer/valuer/rz-asg-valuer.h"

#include "rz-graph-core/code/chasm-rz-function-def-entry.h"

#include "rz-graph-core/code/chasm-rz-call-entry.h"


#include "rz-graph-core/token/chasm-rz-token.h"

#include "rz-graph-valuer/scope/rz-asg-block-info.h"

#include "rz-function-def/rz-function-def-syntax.h"
#include "rz-function-def/rz-function-def-info.h"

#include "phr-graph-core/kernel/graph/pgb-ir-build.h"

#include "multistep-token.h"

#include "rzns.h"

USING_RZNS(RZ_Core)


USING_RZNS(GVal)

RPI_Block::RPI_Block(PGB_IR_Build& pgb, caon_ptr<RPI_Block> parent_block)
  :  pgb_(pgb),
     parent_block_(parent_block), block_sequence_mode_(Block_Sequence_Modes::N_A),
     current_form_(nullptr), last_form_(nullptr), continue_block_(nullptr),
     block_info_(nullptr), pending_block_info_(nullptr),
     lexical_scope_(nullptr), function_def_info_(nullptr),
     parent_lambda_position_(-1)
{

}

void RPI_Block::build_phaon_graph()
{

}

int RPI_Block::get_new_hd_code()
{
 static int result = 0;
 return ++result;
}

void RPI_Block::scan_top_level(RZ_Graph_Visitor_Phaon& visitor_phaon)
{
 caon_ptr<ChasmRZ_Node> rn = visitor_phaon.visitor().graph_root_node();

 scan(visitor_phaon, *rn);
}

void RPI_Block::scan(RZ_Graph_Visitor_Phaon& visitor_phaon,
  ChasmRZ_Node& start_node)
{
 caon_ptr<RZ_ASG_Block_Info> rbi;
 if(caon_ptr<ChasmRZ_Node> ben = visitor_phaon.visitor().find_block_entry_node(&start_node, rbi))
 {
  CAON_PTR_DEBUG(ChasmRZ_Node ,ben)

  caon_ptr<ChasmRZ_Node> cen = visitor_phaon.visitor().find_run_call_entry(ben);

  // // does this need to be more fine-grained?

  caon_ptr<ChasmRZ_Block_Entry> rbe = ben->chasm_rz_block_entry();

  if(rbe)
  {
   CAON_PTR_DEBUG(ChasmRZ_Block_Entry ,rbe)
   lexical_scope_ = rbe->lexical_scope();
  }

  CAON_PTR_DEBUG(ChasmRZ_Node ,cen)

  if(cen)
  {
   add_form_from_call_entry_node(visitor_phaon, *cen, rbe);
  }

  caon_ptr<ChasmRZ_Node> current_node = cen;
  while(current_node)
  {
   if(current_node = visitor_phaon.find_statement_cross_sequence_node(current_node) )
   {
    CAON_PTR_DEBUG(ChasmRZ_Node, current_node)
    last_form_ = current_form_;
    current_form_ = nullptr;
    add_form_from_call_entry_node(visitor_phaon, *current_node);
   }
  }
  CAON_DEBUG_NOOP
 }

}


void RPI_Block::add_form_from_call_entry_node(RZ_Graph_Visitor_Phaon& visitor_phaon,
  ChasmRZ_Node& entry_node, caon_ptr<ChasmRZ_Block_Entry> rbe,
  caon_ptr<RPI_Stage_Form> prior_form)
{
 if(current_form_)
 {
  // i.e., an expression inside a form

 }
 else
 {
  CAON_PTR_DEBUG(RPI_Stage_Form ,current_form_)
  current_form_ = new RPI_Stage_Form(pgb_);

  if(prior_form)
  {
   CAON_PTR_DEBUG(RPI_Stage_Form ,prior_form)
   current_form_->set_prior_sibling_flags(prior_form);
  }

  caon_ptr<ChasmRZ_Call_Entry> rce = entry_node.chasm_rz_call_entry();

  CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)

  if(rce && rce->flags.is_statement_entry)
  {
   current_form_->flags.is_statement = true;
  }

  if(rbe)
  {
   if(parent_block_)
     current_form_->flags.is_nested_block_entry_statment = true;
   else
     current_form_->flags.is_block_entry_statment = true;
  }

  forms_.push_back(current_form_);
  if(caon_ptr<ChasmRZ_Node> start_node = visitor_phaon.start_node_from_call_entry_node(&entry_node))
  {
   CAON_PTR_DEBUG(ChasmRZ_Node ,start_node)

   if(caon_ptr<RZ_Expression_Review> rer = visitor_phaon.get_expression_review_from_entry_node(start_node))
   {
    CAON_PTR_DEBUG(RZ_Expression_Review ,rer)
    current_form_->set_expression_review(rer);
   }

   if(caon_ptr<RZ_Code_Statement> st = visitor_phaon.get_code_statement_from_statement_entry_node(start_node))
   {
    CAON_PTR_DEBUG(RZ_Code_Statement ,st)
    code_statements_.push_back(st);
    current_form_->set_code_statement(st);

    visitor_phaon.check_assignment_annotation(start_node, st);


   }
   else
   {
    code_statements_.push_back(nullptr);
   }
   scan_form_from_start_node(visitor_phaon, *start_node);
  }
 }

}


void RPI_Block::scan_form_from_start_node(RZ_Graph_Visitor_Phaon& visitor_phaon,
  ChasmRZ_Node& start_node)
{
 scan_form_from_statement_entry_node(visitor_phaon, start_node);
 current_form_ = nullptr;
}

caon_ptr<ChasmRZ_Block_Entry> RPI_Block::get_block_entry()
{
 if(block_info_)
 {
  if(caon_ptr<ChasmRZ_Node> ben = block_info_->block_entry_node())
  {
   return ben->chasm_rz_block_entry();
  }
 }
 return nullptr;
}

// //  contrary to name, can also be expression entry ...
void RPI_Block::scan_form_from_statement_entry_node(RZ_Graph_Visitor_Phaon& visitor_phaon,
  ChasmRZ_Node& start_node)
{
 int lambda_count = 0;
 int implicit_added_depth = 0;

 if(pending_block_info_)
 {
  //  this assumes there will never be "nested" pending block info ...
 }
 else
 {
  pending_block_info_ = visitor_phaon.check_pending_block_info(&start_node);
 }

 if(caon_ptr<RZ_ASG_Token> tok = start_node.asg_token())
 {
  CAON_PTR_DEBUG(RZ_ASG_Token ,tok)

  QString fn;
  RZ_Graph_Visitor_Phaon::Special_Form_Flags sff =
    RZ_Graph_Visitor_Phaon::parse_special_form_flag(tok->raw_text());

  switch(sff)
  {
  case RZ_Graph_Visitor_Phaon::Special_Form_Flags::Type_Declare:
   {
    current_form_->init_type_declaration(tok->raw_text());
    fn = visitor_phaon.function_name_from_token_text(tok->raw_text());
    current_form_->add_instruction_element(RPI_Stage_Element_Kinds::Instruction_Symbol, fn);
   }
   break;
  case RZ_Graph_Visitor_Phaon::Special_Form_Flags::Formula:
   {
    current_form_->init_formula_expression(tok->raw_text());
   }
   break;
  case RZ_Graph_Visitor_Phaon::Special_Form_Flags::Assignment:
   {
    current_form_->init_assignment_expression(tok->raw_text());
   }
   break;
  case RZ_Graph_Visitor_Phaon::Special_Form_Flags::N_A:
   {
    if(tok->flags.is_if_with_elsif)
    {
     current_form_->init_expression();
     fn = "kb::if-with-elsif";
    }
    else if(tok->flags.is_if_with_else)
    {
     current_form_->init_expression();
     fn = "kb::if-with-else";
    }
    else
    {
     current_form_->init_expression();
     fn = visitor_phaon.function_name_from_token_text(tok->raw_text());
     if(tok->raw_text() == "/->")
     {
      if(caon_ptr<RPI_Stage_Form> parent = current_form_->parent())
      {
       parent->set_s1_assignment_check("/->");
       //?current_form_->mark_as_parent_s1_assignment_preempts_s0();
      }
     }
    }
    // // todo ... current_form_->check_init_annotation(fn);
    if(fn.startsWith("kb::"))
    {
     current_form_->add_instruction_element(RPI_Stage_Element_Kinds::Instruction_Symbol, fn);
     if(fn == "kb::write-overloadable-fdef")
     {
      current_form_->init_type_declaration("kb::write-overloadable-fdef");
     }
    }
    else
    {
     if(fn.startsWith('&'))
     {
      current_form_->init_inferred_s0_statement();
     }
     else if(lexical_scope_->contains_function_symbol(fn))
     {
      current_form_->init_inferred_s0_statement();
     }
     else if(lexical_scope_->contains_symbol(fn))
     {
      current_form_->init_inferred_s0_statement();
     }
     current_form_->add_fn_element(RPI_Stage_Element_Kinds::FGround_Symbol, fn);
    }

    // should be a flag ...
    if(tok->raw_text() == "do")
    {
     if(current_form_->parent_lambda_position() != -1)
     {
     }
    }
   }
   break;
  }
 }
 caon_ptr<ChasmRZ_Node> current_node = &start_node;

 RZ_ASG_Visitor::Next_Node_Premise nnp = RZ_ASG_Visitor::Next_Node_Premise::N_A;

 // maybe temp ...
 bool last_nnp_expr = false;

 QString field_index_key;

 while(current_node)
 {
  last_nnp_expr = nnp == RZ_ASG_Visitor::Next_Node_Premise::Expression;

  // nnp is read-write in visitor_phaon.get_next_node(...)
  caon_ptr<ChasmRZ_Node> next_node = visitor_phaon.get_next_node(current_node, nnp);

  CAON_PTR_DEBUG(ChasmRZ_Node ,current_node)
  CAON_PTR_DEBUG(ChasmRZ_Node ,next_node)

  bool skip_increase_lambda_count = false;

  MS_Token_Kinds mstk = MS_Token_Kinds::N_A;

  switch(nnp)
  {

  case RZ_ASG_Visitor::Next_Node_Premise::Normal:
   {
    if(caon_ptr<RZ_ASG_Token> next_tok = next_node->asg_token())
    {
     CAON_PTR_DEBUG(RZ_ASG_Token ,next_tok)

     if(next_tok->flags.is_dep_marker)
     {
      current_node = next_node;
      continue;
     }
     QString bridge_code;

     MS_Token mst = visitor_phaon.lisp_token_from_token_text(next_tok->raw_text(), bridge_code);

     QString lt = mst.raw_text;
     if(mst.kind != MS_Token_Kinds::N_A)
     {
      mstk = mst.kind;
     }

     if(next_tok->flags.is_assign_to_default_ctor)
     {
      skip_increase_lambda_count = true;
     }
     else if(next_tok->flags.precedes_call_arrow)
     {
      skip_increase_lambda_count = true;
      QString ht = lt;

      held_token_ = {MS_Token_Kinds::Sigma_Symbol, ht};
      lt.clear();
      held_sigma_token_ = next_tok;
     }
     else if(next_tok->flags.follows_call_arrow)
     {
      skip_increase_lambda_count = true;

      // // do we need this?
      //lt.replace('-', '_');
     }

     else if(next_tok->flags.is_symbol_declaration ||
        next_tok->flags.is_symbol_matched_to_declaration)
     {
      mstk = MS_Token_Kinds::Scoped_Symbol;
      field_index_key = visitor_phaon.get_field_index_key(next_node, lt);
     }
     else if(next_tok->flags.is_string_literal)
     {
      lt.prepend("\"");
      lt.append("\"");
     }
     else if(lexical_scope_ && lexical_scope_->contains_symbol(lt))
     {
      mstk = MS_Token_Kinds::Scoped_Symbol;
     }

     else if(next_tok->flags.is_type_symbol_in_declaration)
     {
      skip_increase_lambda_count = true;
      mstk = MS_Token_Kinds::Type_Symbol;
     }
     if(!lt.isEmpty())
     {
      if(bridge_code.isEmpty())
      {
       if(next_tok->flags.is_numeric_literal)
       {
        current_form_->add_insert_element(RPI_Stage_Element_Kinds::Kernel_Type_Symbol, "u4");
        current_form_->add_literal_element(RPI_Stage_Element_Kinds::Literal, lt);
       }
       else if(next_tok->flags.is_string_literal)
       {
        current_form_->add_insert_element(RPI_Stage_Element_Kinds::Kernel_Type_Symbol, "str");
        current_form_->add_literal_element(RPI_Stage_Element_Kinds::String_Literal, lt);
       }
       else if(next_tok->flags.has_assignment_initialization_expression)
       {
        current_form_->set_assignment_info(RPI_Assignment_Info(
          RPI_Assignment_Value_Kinds::Expression,
          RPI_Assignment_Initialization_Kinds::Init, lt));
       }
       else if(next_tok->flags.has_assignment_initialization_token)
       {
        current_form_->set_assignment_info(RPI_Assignment_Info(
          RPI_Assignment_Value_Kinds::Literal,
          RPI_Assignment_Initialization_Kinds::Init, lt));
       }
       else if(next_tok->flags.has_assignment_to_type)
       {
        if(next_tok->flags.has_assignment_to_type_default)
          current_form_->set_assignment_info(RPI_Assignment_Info(
          RPI_Assignment_Value_Kinds::Type_Default,
          RPI_Assignment_Initialization_Kinds::Init, lt));
        else if(next_tok->flags.has_assignment_to_ctor_expression)
          current_form_->set_assignment_info(RPI_Assignment_Info(
          RPI_Assignment_Value_Kinds::Ctor,
          RPI_Assignment_Initialization_Kinds::Init, lt));
        else
          current_form_->set_assignment_info(RPI_Assignment_Info(
          RPI_Assignment_Value_Kinds::Type,
          RPI_Assignment_Initialization_Kinds::Init, lt));
       }
       else if(next_tok->flags.is_assignment_initialization_entry)
       {
        current_form_->add_assignment_initialization_element(RPI_Stage_Element_Kinds::Instruction_Symbol, lt);
       }
       else if(next_tok->flags.is_assign_to_default_ctor)
       {
        current_form_->add_argument_element(RPI_Stage_Element_Kinds::Type_Default_Marker, lt);
       }
       else if(next_tok->flags.follows_call_arrow)
       {
        if(held_sigma_token_)
        {
         CAON_PTR_DEBUG(RZ_ASG_Token ,held_sigma_token_)
         current_form_->add_s1_fn_element(lt, held_sigma_token_->raw_text());
         held_sigma_token_ = nullptr;
        }
       }
       else if(mstk == MS_Token_Kinds::Skipped_Flag_Symbol)
       {
        if(lt == "FN-NO-BLOCK")
        {
         current_form_->mark_as_fn_no_block();
        }
        else if(lt == "FN")
        {
         current_form_->mark_as_fn();
        }
       }
       else if(mstk == MS_Token_Kinds::Skip_Token ||
         lt == ":TYPE-DECL")
       {
        skip_increase_lambda_count = true;
       }
       else
       {
        if(!field_index_key.isEmpty())
        {
         current_form_->add_argument_element(RPI_Stage_Element_Kinds::Field_Index_Key, field_index_key);
         field_index_key.clear();
        }
        current_form_->add_argument_element(RPI_Stage_Element_Kinds::Raw_Symbol, lt);
       }
      }
      else
      {
       QString code = visitor_phaon.wrap_token_with_bridge_code(lt, bridge_code, es_argument_);
       current_form_->add_bridge_element(RPI_Stage_Element_Kinds::Bridge_Symbol, code);
      }
      if(!skip_increase_lambda_count)
      {
       ++lambda_count;
      }
      if(!held_token_.raw_text.isEmpty())
      {
       if(held_token_.kind != MS_Token_Kinds::Sigma_Symbol)
       {
        qDebug() << "maybe unexpected held token ...      ";
        current_form_->add_carrier_element(RPI_Stage_Element_Kinds::N_A, held_token_.raw_text);
       }
       held_token_.clear();
      }
     }
    }
    current_node = next_node;
   }
   break;

  case RZ_ASG_Visitor::Next_Node_Premise::Expression:
   {
    CAON_PTR_DEBUG(RPI_Stage_Form ,current_form_)
    RPI_Stage_Form* new_form = new RPI_Stage_Form(pgb_, current_form_);
    new_form->set_parent_lambda_position(lambda_count);
    new_form->set_implicit_added_depth(implicit_added_depth);

    new_form->flags.write_push_expression = true;

    new_form->set_prior_sibling_flags(current_form_);
    current_form_->add_expression(new_form);

    caon_ptr<ChasmRZ_Call_Entry> rce = next_node->chasm_rz_call_entry();
    CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)
    if( (lambda_count > 0) && !last_nnp_expr )
      new_form->flags.has_preceder_token = true;
    if(rce->flags.is_deferred)
    {
     new_form->mark_deferred(0);
     current_form_->add_expression_wrapper(new_form, "defer'", get_new_hd_code());
    }
    current_form_ = new_form;
    caon_ptr<ChasmRZ_Node> en = visitor_phaon.visitor().entry_from_call_entry(next_node);
    CAON_PTR_DEBUG(ChasmRZ_Node ,en)
    scan_form_from_statement_entry_node(visitor_phaon, *en);
    current_form_ = new_form->parent();

    if(pending_block_info_)
    {
     visitor_phaon.set_pending_block_info(pending_block_info_);
     pending_block_info_ = nullptr;
    }
    else
    {
     current_node = next_node;
    }
   }
   break;

  case RZ_ASG_Visitor::Next_Node_Premise::Block_Entry:
   {
    // // "hand-off" the pending_block_info_ from visitor_phaon to this
    caon_ptr<RZ_ASG_Block_Info> rbi = visitor_phaon.clear_pending_block_info();

    caon_ptr<RPI_Block> new_block = new RPI_Block(pgb_, this);
    caon_ptr<RPI_Block> current_block = new_block;

    new_block->set_block_info(rbi);
    if(caon_ptr<ChasmRZ_Node> cen = visitor_phaon.call_entry_node_from_block_entry_node(next_node))
    {
     CAON_PTR_DEBUG(RZ_ASG_Block_Info ,rbi)
     CAON_PTR_DEBUG(ChasmRZ_Node ,cen)
     if(caon_ptr<ChasmRZ_Node> nben = rbi->block_entry_node())
     {
      if(caon_ptr<ChasmRZ_Block_Entry> rbe = nben->chasm_rz_block_entry())
      {
       new_block->set_lexical_scope(rbe->lexical_scope());
      }
     }
     new_block->add_form_from_call_entry_node(visitor_phaon, *cen);
     current_form_->add_nested_block(new_block);


     caon_ptr<RZ_ASG_Block_Info> nn_bi;
     caon_ptr<ChasmRZ_Node> continue_node = visitor_phaon.leave_nested_block(rbi, nn_bi);
     while(continue_node)
     {
      caon_ptr<ChasmRZ_Node> nben = nn_bi->block_entry_node();
      CAON_PTR_DEBUG(ChasmRZ_Node ,continue_node)
      CAON_PTR_DEBUG(RZ_ASG_Block_Info ,nn_bi)
      CAON_PTR_DEBUG(ChasmRZ_Node ,nben)
      caon_ptr<ChasmRZ_Block_Entry> nrbe = nben->chasm_rz_block_entry();
      CAON_PTR_DEBUG(ChasmRZ_Block_Entry ,nrbe)

      caon_ptr<RPI_Stage_Form> expression_form = nullptr;

      if(nrbe->flags.elsif_block)
      {
       if(caon_ptr<ChasmRZ_Node> sen = nrbe->statement_entry_node())
       {
        CAON_PTR_DEBUG(ChasmRZ_Node ,sen)
        caon_ptr<ChasmRZ_Node> en = visitor_phaon.visitor().entry_from_call_entry(sen);
        CAON_PTR_DEBUG(ChasmRZ_Node ,en)

        if(en)
        {
         caon_ptr<ChasmRZ_Node> nen = visitor_phaon.visitor().entry_from_call_entry(en);
         CAON_PTR_DEBUG(ChasmRZ_Node ,nen)
         if(nen)
         {
          expression_form = new RPI_Stage_Form(pgb_, current_form_);
          current_form_ = expression_form;

          caon_ptr<ChasmRZ_Node> nene = visitor_phaon.visitor().entry_from_call_entry(nen);
          CAON_PTR_DEBUG(ChasmRZ_Node ,nene)

          scan_form_from_statement_entry_node(visitor_phaon, *nene);

          current_form_ = expression_form->parent();

          current_form_->add_expression(expression_form);
         }
        }

        caon_ptr<RZ_ASG_Block_Info> new_rbi;

        caon_ptr<ChasmRZ_Node> new_ben = visitor_phaon.visitor().find_block_entry_node(en, new_rbi);
        CAON_PTR_DEBUG(ChasmRZ_Node ,new_ben)
        CAON_PTR_DEBUG(RZ_ASG_Block_Info ,new_rbi)
        if(!new_ben)
        {
         // // should this be handled in find_block_entry_node()? ...
         if(new_rbi)
         {
          new_ben = new_rbi->block_entry_node();
         }
        }
        if(new_ben)
        {
         caon_ptr<RPI_Block> new_block = new RPI_Block(pgb_, this);
         new_block->set_block_info(new_rbi);

         if(caon_ptr<ChasmRZ_Node> new_cen = visitor_phaon.call_entry_node_from_block_entry_node(new_ben))
         {
          CAON_PTR_DEBUG(ChasmRZ_Node ,new_cen)
          new_block->add_form_from_call_entry_node(visitor_phaon, *new_cen);
          current_form_->add_nested_block(new_block);
         }
        }
       }

      }

      else if(nrbe->flags.else_block)
      {
       if(caon_ptr<ChasmRZ_Node> sen = nrbe->statement_entry_node())
       {
        CAON_PTR_DEBUG(ChasmRZ_Node ,sen)
        caon_ptr<ChasmRZ_Node> en = visitor_phaon.visitor().entry_from_call_entry(sen);
        CAON_PTR_DEBUG(ChasmRZ_Node ,en)

        caon_ptr<RZ_ASG_Block_Info> new_rbi;

        caon_ptr<ChasmRZ_Node> new_ben = visitor_phaon.visitor().find_block_entry_node(en, new_rbi);
        CAON_PTR_DEBUG(ChasmRZ_Node ,new_ben)
        CAON_PTR_DEBUG(RZ_ASG_Block_Info ,new_rbi)

        if(new_ben)
        {
         caon_ptr<RPI_Block> new_block = new RPI_Block(pgb_, this);
         new_block->set_block_info(new_rbi);

         if(caon_ptr<ChasmRZ_Node> new_cen = visitor_phaon.call_entry_node_from_block_entry_node(new_ben))
         {
          CAON_PTR_DEBUG(ChasmRZ_Node ,new_cen)
          new_block->add_form_from_call_entry_node(visitor_phaon, *new_cen);
          current_form_->add_nested_block(new_block);
         }
        }


        // handle the expression ...
       }
      }

      caon_ptr<RPI_Block> nb = new RPI_Block(pgb_, this);
      nb->set_block_info(nn_bi);
      nb->set_preceding_expression_form(expression_form);
      current_block->set_continue_block(nb);
      current_block = nb;

      if(caon_ptr<ChasmRZ_Node> cen = visitor_phaon.call_entry_node_from_block_entry_node(nben))
      {
       nb->add_form_from_call_entry_node(visitor_phaon, *cen);
      }
      continue_node = visitor_phaon.leave_nested_block(nn_bi, nn_bi);
     }
    }
    current_node = nullptr;
   }
   break;
  case RZ_ASG_Visitor::Next_Node_Premise::Function_Def_Entry:
   {
    QString entry_code;
    CAON_PTR_DEBUG(ChasmRZ_Node ,next_node)
    if(caon_ptr<ChasmRZ_Function_Def_Entry> fde = next_node->chasm_rz_function_def_entry())
    {
     CAON_PTR_DEBUG(ChasmRZ_Function_Def_Entry ,fde)
     caon_ptr<ChasmRZ_Node> prior_node = fde->prior_node();
     CAON_PTR_DEBUG(ChasmRZ_Node ,prior_node)

     if(caon_ptr<ChasmRZ_Token> tok = prior_node->chasm_rz_token())
     {
      CAON_PTR_DEBUG(ChasmRZ_Token ,tok)
      CAON_DEBUG_NOOP
     }

     if(fde->kind() == ChasmRZ_Function_Def_Kinds::Call_Arrow_Note)
     {
      caon_ptr<ChasmRZ_Node> note_node = fde->note_node();
      QString note = note_node->chasm_rz_token()->raw_text();

      if(caon_ptr<RZ_Function_Def_Info> function_def_info =
        visitor_phaon.get_function_def_info_from_entry(fde))
      {
       current_form_->write_fdef_entry();
       function_def_info->write_phr_signature_code(pgb_, current_form_->step_forms());
      }
     }
     else if(fde->kind() == ChasmRZ_Function_Def_Kinds::Call_Arrow_Async)
     {
      // //?
     }
     else
     {
      if(caon_ptr<RZ_Function_Def_Info> function_def_info =
        visitor_phaon.get_function_def_info_from_entry(fde))
      {
       QString signature_code = function_def_info->dynamo_signature_code_string();
       MS_Token note {MS_Token_Kinds::Note_Symbol, ":fdef"};

       caon_ptr<ChasmRZ_Node> prior_node = fde->prior_node();
       CAON_PTR_DEBUG(ChasmRZ_Node ,prior_node)

       if(caon_ptr<ChasmRZ_Token> tok = prior_node->chasm_rz_token())
       {
        if(!tok->flags.is_do)
        {
         current_form_->write_fdef_entry();
         function_def_info->write_phr_signature_code(pgb_, current_form_->step_forms());
        }
       }
      }
     }

     caon_ptr<ChasmRZ_Node> fdi_node = fde->node();
     CAON_PTR_DEBUG(ChasmRZ_Node ,fdi_node)

     if(caon_ptr<ChasmRZ_Node> ben = visitor_phaon.block_entry_node_from_function_def_entry_node(prior_node))
     {
      CAON_PTR_DEBUG(ChasmRZ_Node ,ben)
      CAON_DEBUG_NOOP
      caon_ptr<RPI_Block> new_block = new RPI_Block(pgb_, this);

      // //  does this need to depend on the function def info somehow?
      new_block->set_es_argument("ks");

      if(caon_ptr<ChasmRZ_Block_Entry> rbe = ben->chasm_rz_block_entry())
      {
       caon_ptr<RZ_ASG_Lexical_Scope> ls = rbe->lexical_scope();

       if(caon_ptr<RZ_Function_Def_Info> function_def_info =
         visitor_phaon.get_function_def_info_from_entry(fde))
       {
        CAON_PTR_DEBUG(RZ_Function_Def_Info ,function_def_info)
        entry_code = function_def_info->kauvir_entry_code_string(*ls);
        new_block->set_function_def_info(function_def_info);
       }


       CAON_PTR_DEBUG(RZ_ASG_Lexical_Scope ,ls)
       new_block->set_lexical_scope(ls);
       new_block->set_entry_lisp_code(entry_code);
      }


      new_block->set_parent_lambda_position(parent_lambda_position_);
      ++lambda_count;
      if(caon_ptr<ChasmRZ_Node> cen = visitor_phaon.call_entry_node_from_block_entry_node(ben))
      {
       CAON_PTR_DEBUG(ChasmRZ_Node ,cen)
       caon_ptr<ChasmRZ_Block_Entry> rbe = ben->chasm_rz_block_entry();
       CAON_PTR_DEBUG(ChasmRZ_Block_Entry ,rbe)
       new_block->add_form_from_call_entry_node(visitor_phaon, *cen, rbe, current_form_);

       caon_ptr<ChasmRZ_Node> next_statement_node = cen;
       while(next_statement_node = visitor_phaon.get_next_statement_node(next_statement_node))
       {
        CAON_PTR_DEBUG(ChasmRZ_Node ,next_statement_node)
        new_block->add_form_from_call_entry_node(visitor_phaon, *next_statement_node);
       }

       current_form_->add_nested_block(new_block);
      }
     }
    }

    current_node = nullptr;

   }
   break;

  case RZ_ASG_Visitor::Next_Node_Premise::Token_To_Block_Entry:
   {
    caon_ptr<RPI_Block> new_block = new RPI_Block(pgb_, this);

    caon_ptr<ChasmRZ_Block_Entry> rbe = next_node->chasm_rz_block_entry();

    if(rbe->flags.ghost_block)
    {
     new_block->set_block_sequence_mode(Block_Sequence_Modes::Ghost);
    }

    ++lambda_count;
    if(caon_ptr<ChasmRZ_Node> cen = visitor_phaon.call_entry_node_from_block_entry_node(next_node))
    {
     new_block->add_form_from_call_entry_node(visitor_phaon, *cen);
     current_form_->add_nested_block(new_block);
    }

    current_node = nullptr;
   }
   break;

  case RZ_ASG_Visitor::Next_Node_Premise::N_A:
   current_node = nullptr;
   break;
  }
 }
}

void RPI_Block::write_top_level(QList<PGB_IR_Build::Text_With_Purpose>& tps, QTextStream* qts)
{
 pgb_(tps).make_root_node("!last_block_pre_entry_node");
 write(tps, qts);
}


void RPI_Block::write(QList<PGB_IR_Build::Text_With_Purpose>& tps, QTextStream* qts)
{
 caon_ptr<RPI_Stage_Form> prior = nullptr;
 for(caon_ptr<RPI_Stage_Form> rsf : forms_)
 {
  rsf->write_as_statement(qts, prior);

  CAON_PTR_DEBUG(RPI_Stage_Form ,rsf)

  if(!rsf->is_effective_block_entry_statment())
  {
   if(!rsf->type_declaration())
   {
    RPI_Assignment_Info& rai = rsf->assignment_info();
    if(rai.has_text())
    {
     pgb_.insert_before_purpose(rsf->step_forms(),  Purpose_Codes::Copy_To_Last_Statement_Entry_Node)
       .make_statement_info_node(
       rai.text().prepend('@'), ":result",
       rai.encode_ikind().prepend(':'), "&si-node");
     pgb_.insert_before_purpose(rsf->step_forms(),  Purpose_Codes::Copy_To_Last_Statement_Entry_Node)
       .add_statement_sequence_node("!last_statement_entry_node",
       "&fsym-node", "&si-node");
    }
    else
      pgb_.insert_before_purpose(rsf->step_forms(),  Purpose_Codes::Copy_To_Last_Statement_Entry_Node)
      .add_statement_sequence_node("!last_statement_entry_node",
      "&fsym-node");
   }
  }

  tps.append(rsf->step_forms());
  prior = rsf;
 }

 caon_ptr<ChasmRZ_Block_Entry> rbe = get_block_entry();

// //  todo ...
// if(continue_block_)
// {
// }
// else if(rbe)
// {
//  CAON_PTR_DEBUG(ChasmRZ_Block_Entry ,rbe)
//  if(rbe->flags.if_block)
//  {
//  }
//  else if(rbe->flags.elsif_block)
//  {
//  }
// }
}
