
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "rpi-stage-form.h"
#include "rpi-block.h"

#include "rz-graph-core/code/chasm-rz-block-entry.h"

#include "rz-graph-valuer/scope/rz-asg-block-info.h"

#include "rpi-type-declaration.h"
#include "rpi-expression.h"

#include "rpi-stage-element.h"

#include "rz-function-def/rz-function-def-info.h"

#include "rz-code-elements/rz-code-statement.h"
#include "rz-code-elements/rz-expression-review.h"

#include "phr-graph-core/kernel/graph/pgb-ir-build.h"

#include <functional>

#include "rzns.h"

#define ANNOTATION_FLAG(x) \
  (annotation_ && annotation_->flags.x)

USING_RZNS(GVal)

RPI_Stage_Form::RPI_Stage_Form(PGB_IR_Build& pgb, caon_ptr<RPI_Stage_Form> parent)
  :  Flags(0), pgb_(pgb), parent_(parent), parent_lambda_position_(-1),
     first_nested_block_(nullptr), implicit_added_depth_(0),
     nesting_level_(0),
     type_declaration_(nullptr), expression_(nullptr),
     assignment_info_(),
     annotation_(nullptr),
     code_statement_(nullptr),
     expression_review_(nullptr), hdcode_(0)
{

}

RPI_Stage_Form::RPI_Stage_Form(PGB_IR_Build& pgb, caon_ptr<RPI_Block> block)
  :  pgb_(pgb), parent_(nullptr), plene_block_(block),
     first_nested_block_(nullptr), implicit_added_depth_(0),
     type_declaration_(nullptr), expression_(nullptr),
     assignment_info_()
{

}

QString RPI_Stage_Form::get_assignment_target()
{
 return assignment_info_.text();
}


caon_ptr<RPI_Type_Declaration> RPI_Stage_Form::type_declaration_on_block_entry()
{
 if(flags.is_block_entry_statment || flags.is_nested_block_entry_statment)
   return type_declaration_;
 return nullptr;
}


void RPI_Stage_Form::write_fdef_entry()
{
 if(inner_elements_.size() < 2)
   return;

 // // is it always here ([0] is the type-binding instruction?)
 RPI_Stage_Element& rse = inner_elements_[1];

 pgb_(step_forms_).comment("Signature");
 pgb_(step_forms_).make_token_node(rse.text().prepend('@'), "&sig-fground-node")
  = Purpose_Codes::Make_Token_Node_FSym;

 write_cond_statement_entry("&sig-fground-node",
   "!last_statement_entry_node", "!last_block_pre_entry_node", "!last_statement_entry_node");

 pgb_(step_forms_).make_signatuChasmRZ_Node("&sig-fground-node", "&sig-node");

}

void RPI_Stage_Form::write_assignment_initialization_via_expression(
  QTextStream* qts, caon_ptr<RPI_Stage_Form> prior)
{
 RPI_Stage_Element& rse = inner_elements_.first();
 caon_ptr<RPI_Stage_Form> f = rse.form();

 caon_ptr<RPI_Type_Declaration> td = nullptr;
 if(prior)
 {
  td = prior->type_declaration_on_block_entry();
 }
 if(td)
   f->flags.is_block_entry_statment = true;

 f->mark_as_assignment_expression();

 f->write_unmediated(qts, prior);
 step_forms_.append(f->step_forms());
}

void RPI_Stage_Form::write_assignment_initialization_via_token(
  QString key, caon_ptr<RPI_Stage_Form> prior)
{
 pgb_(step_forms_).make_statement_info_node(assignment_info_.text().prepend('@'),
   key.prepend(':'), assignment_info_.encode_ikind().prepend(':'),
   "&si-node");

 QString ty;

 caon_ptr<RPI_Type_Declaration> prior_type_declaration_ben = nullptr;
 if(prior)
 {
  prior_type_declaration_ben = prior->type_declaration_on_block_entry();
 }

 for(RPI_Stage_Element& rse: inner_elements_)
 {
  if(rse.kind() == RPI_Stage_Element_Kinds::Kernel_Type_Symbol)
    ty = rse.text();
  else if(rse.kind() == RPI_Stage_Element_Kinds::Literal)
  {
   if(prior_type_declaration_ben)
     pgb_(step_forms_).add_block_entry_token("!last_block_pre_entry_node",
     rse.text().prepend('$'), "&si-node", "!last_statement_entry_node");
   else
     pgb_(step_forms_).add_statement_sequence_token("!last_statement_entry_node",
     rse.text().prepend('$'), "&si-node", "!last_statement_entry_node");
   break;
  }
  else if(rse.kind() == RPI_Stage_Element_Kinds::Type_Default_Marker)
  {
   if(prior_type_declaration_ben)
     pgb_(step_forms_).add_block_entry_token("!last_block_pre_entry_node",
     rse.text().prepend('@'), "&si-node", "!last_statement_entry_node");
   else
     pgb_(step_forms_).add_statement_sequence_token("!last_statement_entry_node",
     rse.text().prepend('@'), "&si-node", "!last_statement_entry_node");
   break;
  }
 }
}

void RPI_Stage_Form::write_as_statement(QTextStream* qts, caon_ptr<RPI_Stage_Form> prior)
{
 if(type_declaration_)
 {
  CAON_PTR_DEBUG(RPI_Type_Declaration ,type_declaration_)
  type_declaration_->write(qts);
 }
 else if(expression_)
 {
  expression_->write_as_statement(qts, prior);
 }
 else
 {
  write(qts, prior);
 }

}


void RPI_Stage_Form::init_type_declaration(QString cmd)
{
 if(type_declaration_)
 {
  CAON_PTR_DEBUG(RPI_Type_Declaration ,type_declaration_)
  // // this is an error ...
  return;
 }
 // // should distinguish cmd == "my" and "our" ...
 type_declaration_ = new RPI_Type_Declaration(*this);
 type_declaration_->set_cmd(cmd);

 flags.write_type_declaration = true;
}

void RPI_Stage_Form::init_expression()
{
 expression_ = new RPI_Expression(*this);
}

void RPI_Stage_Form::init_assignment_expression(QString tok)
{
 init_expression();
 expression_->set_assignment_token(tok);
}

void RPI_Stage_Form::init_formula_expression(QString tok)
{
 init_expression();
 expression_->set_assignment_token(tok);
}

void RPI_Stage_Form::write(QTextStream* qts, caon_ptr<RPI_Stage_Form> prior)
{
//? check_init_annotation_flags();
 CAON_PTR_DEBUG(RPI_Stage_Form_Annotation ,annotation_)
 if(type_declaration_)
 {
  CAON_PTR_DEBUG(RPI_Type_Declaration ,type_declaration_)
  type_declaration_->write(qts);
 }
 else if(expression_)
 {
  expression_->write(qts, prior);
 }
 else
 {
  write_unmediated(qts, prior);
 }
}

QString RPI_Stage_Form::instruction()
{
 if(inner_elements_.isEmpty())
   return QString();
 if(inner_elements_.first().kind() != RPI_Stage_Element_Kinds::Instruction_Symbol)
   return QString();
 return inner_elements_.first().text();
}



void RPI_Stage_Form::add_expression_wrapper(caon_ptr<RPI_Stage_Form> form,
  QString text, int hdcode)
{
 wrapped_inner_elements_[form] = {text, hdcode};
}

void RPI_Stage_Form::set_prior_sibling_flags(caon_ptr<RPI_Stage_Form> prior)
{
 CAON_PTR_DEBUG(RPI_Stage_Form ,prior)
 if(!prior)
   return;
 RPI_Stage_Element_Kinds k = prior->last_element_kind();
 switch(k)
 {
 case RPI_Stage_Element_Kinds::Form:
  if(prior->last_element_form_instruction("kb::write-anon-fdef"))
    flags.prior_sibling_is_block = true;
  else
    flags.prior_sibling_is_expression = true;
  break;
 case RPI_Stage_Element_Kinds::FGround_Symbol:
  flags.prior_sibling_is_fground_token = true;
  break;
 case RPI_Stage_Element_Kinds::Token:
 case RPI_Stage_Element_Kinds::Literal:
 case RPI_Stage_Element_Kinds::String_Literal:
  flags.prior_sibling_is_token = true;

 case RPI_Stage_Element_Kinds::Instruction_Symbol:
  if(prior->instruction("kb::write-anon-fdef"))
  {
   // // here the prior is basically a wrapper for this form
   flags.prior_sibling_is_block = prior->flags.prior_sibling_is_block;
   flags.prior_sibling_is_expression = prior->flags.prior_sibling_is_expression;
   flags.prior_sibling_is_fground_token = prior->flags.prior_sibling_is_fground_token;
   flags.prior_sibling_is_token = prior->flags.prior_sibling_is_token;
  }
  break;
 default:
  break;
 }
 init_prior_description();
}

void RPI_Stage_Form::init_prior_description()
{
 if(flags.prior_sibling_is_block)
   prior_description_ = "prior_sibling_is_block";
 if(flags.prior_sibling_is_expression)
   prior_description_ = "prior_sibling_is_expression";
 if(flags.prior_sibling_is_fground_token)
   prior_description_ = "prior_sibling_is_fground_token";
 if(flags.prior_sibling_is_token)
   prior_description_ = "prior_sibling_is_token";
}

RPI_Stage_Element_Kinds RPI_Stage_Form::last_element_kind()
{
 if(inner_elements_.isEmpty())
   return RPI_Stage_Element_Kinds::N_A;
 return inner_elements_.last().kind();
}

QString RPI_Stage_Form::last_element_form_instruction()
{
 RPI_Stage_Element_Kinds k = last_element_kind();
 if(k == RPI_Stage_Element_Kinds::Form)
   return inner_elements_.last().form()->instruction();
 return QString();
}

void RPI_Stage_Form::add_expression(caon_ptr<RPI_Stage_Form> form)
{
 inner_elements_.push_back(RPI_Stage_Element(form));
}

void RPI_Stage_Form::add_nested_block(caon_ptr<RPI_Block> block)
{
 if(!first_nested_block_)
 {
  first_nested_block_ = block;
 }
 caon_ptr<RPI_Stage_Form> form = new RPI_Stage_Form(pgb_, block);
 inner_elements_.push_back(form);
}

void RPI_Stage_Form::mark_as_fn_no_block()
{
// check_init_annotation();
// annotation_->flags.is_fn_no_block = true;
// CAON_PTR_DEBUG(RPI_Stage_Form_Annotation ,annotation_)
 flags.is_fn_no_block = true;
}

void RPI_Stage_Form::mark_as_fn()
{
 flags.is_fn = true;
// check_init_annotation();
// annotation_->flags.is_fn = true;
// CAON_PTR_DEBUG(RPI_Stage_Form_Annotation ,annotation_)
}


bool RPI_Stage_Form::is_effective_block_entry_statment()
{
 bool result = flags.is_block_entry_statment;

 if(!result)
   result = flags.is_nested_block_entry_statment;

 if(!result)
 {
  if(inner_elements_.isEmpty())
    return false;
  if(inner_elements_[0].kind() == RPI_Stage_Element_Kinds::Form)
    result = inner_elements_[0].form()->is_effective_block_entry_statment();
 }
 return result;
}

void RPI_Stage_Form::mark_as_assignment_expression()
{
 flags.is_assignment_expression = true;
// check_init_annotation();
// annotation_->flags.is_nested_as_assignment = true;
// CAON_PTR_DEBUG(RPI_Stage_Form_Annotation ,annotation_)
}

//bool RPI_Stage_Form::s1_assignment_preempts_s0()
//{
// return false; // ANNOTATION_FLAG(s1_assignment_preempts_s0);
//}

// //  todo ...
//void RPI_Stage_Form::check_init_annotation(QString fn)
//{
// typedef std::function<void(RPI_Stage_Form*)> fn_type;
// static QMap<QString, fn_type> static_map {{
//  {"kb::if-without-elsif", [](RPI_Stage_Form* _this)
//   {
//    _this->check_init_annotation();
//    _this->annotation_->flags.is_general_if = true;
//    _this->annotation_->flags.is_if = true;
//   }
//  },
//  {"kb::if-with-else", [](RPI_Stage_Form* _this)
//   {
//    _this->check_init_annotation();
//    _this->annotation_->flags.is_general_if = true;
//    _this->annotation_->flags.is_if_with_else = true;
//   }
//  },
//  {"kb::if-with-elsif", [](RPI_Stage_Form* _this)
//   {
//    _this->check_init_annotation();
//    _this->annotation_->flags.is_general_if = true;
//    _this->annotation_->flags.is_if_with_elsif = true;
//   }
//  },
// }};
// if(static_map.contains(fn))
// {
//  static_map[fn](this);
// }
//}

void RPI_Stage_Form::write_statement_entry(
  QString node_name, QString prior_node_name, QString copy)
{
 pgb_(step_forms_).add_statement_sequence_node(prior_node_name, node_name);
 if(!copy.isEmpty())
   pgb_(step_forms_).copy_value(node_name, copy);
}

void RPI_Stage_Form::write_cond_statement_entry(
  QString node_name, QString prior_node_name, QString alt_prior_node_name, QString copy)
{
 pgb_(step_forms_).cond_block_entry_or_statement_sequence(prior_node_name, alt_prior_node_name, node_name);
 if(!copy.isEmpty())
   pgb_(step_forms_).copy_value(node_name, copy);
}


void RPI_Stage_Form::write_statement_entry_qts()
{

}

void RPI_Stage_Form::write_statement_entry_qts(QTextStream* qts)
{
}

void RPI_Stage_Form::write_statement_leave_qts(QTextStream* qts)
{
}

QString RPI_Stage_Form::get_s1_assignment_check()
{
 QString result = s1_assignment_check_;
 return result;
}

void RPI_Stage_Form::check_write_first_nested_is_assignment_leave(QTextStream* qts)
{
 CAON_PTR_DEBUG(RPI_Stage_Form_Annotation ,annotation_)
}


void RPI_Stage_Form::write_checked_unmediated(QTextStream* qts, caon_ptr<RPI_Stage_Form> prior)
{
 write_unmediated(qts, prior);
}

bool RPI_Stage_Form::is_non_block_expression()
{
 // for now ...
 return !plene_block_;
}

RPI_Assignment_Info* RPI_Stage_Form::get_parent_assignmnt_info()
{
 RPI_Assignment_Info* rai = nullptr;
 RPI_Assignment_Info* result = nullptr;
 if(parent_)
 {
  rai = &parent_->assignment_info();
  if(rai->has_text())
    result = rai;
 }
 return result;
}

void RPI_Stage_Form::write_signature_type_declaration(QTextStream* qts)
{

}

void RPI_Stage_Form::check_write_block_signature(QList<PGB_IR_Build::Text_With_Purpose>& step_forms)
{
 if(!first_nested_block_)
   return;

 caon_ptr<RZ_Function_Def_Info> fdi = first_nested_block_->function_def_info();

 CAON_PTR_DEBUG(RZ_Function_Def_Info ,fdi)

 pgb_(step_forms).enter_anon_signature("&bin", "&sig-node");
 fdi->write_phr_signature_code(pgb_, step_forms);
 pgb_(step_forms).leave_anon_signature("&bin");

}


void RPI_Stage_Form::write_type_declaration(QTextStream* qts)
{
 RPI_Stage_Element& rse1 = inner_elements_[1];
 RPI_Stage_Element& rse2 = inner_elements_[2];

 if( (rse1.kind() == RPI_Stage_Element_Kinds::Field_Index_Key) && (inner_elements_.size() > 3) )
 {
  RPI_Stage_Element& rse3 = inner_elements_[3];
  pgb_(step_forms_).add_indexed_type_declaration(rse2.text().prepend('@'),
    rse3.text().prepend('@'), rse1.text().prepend(':'),
    "!last_statement_entry_node", "!last_statement_entry_node");
 }
 else
 {
  pgb_(step_forms_).add_type_declaration(rse1.text().prepend('@'),
    rse2.text().prepend('@'));
 }

}


void RPI_Stage_Form::write_unmediated(QTextStream* qts, caon_ptr<RPI_Stage_Form> prior)
{
 CAON_PTR_DEBUG(RPI_Stage_Form ,prior)
 if(type_declaration_)
 {
  if(!flags.write_type_declaration)
  {
   //  *qts << "skipping type declaration ...";
   return;
  }
  if(flags.is_fn)
  {
   if(inner_elements_.size() >= 2)
     write_signature_type_declaration(qts);
  }
  else if(flags.is_fn_no_block)
  {
   if(inner_elements_.size() >= 2)
     write_signature_type_declaration(qts);
  }
  if(inner_elements_.size() >= 3)
    write_type_declaration(qts);
  return;
 }

 RPI_Assignment_Info* rai = get_parent_assignmnt_info();

 int channel_count = 0;
 RPI_Stage_Element_Kinds last_kind = RPI_Stage_Element_Kinds::N_A;

 // // note that prior_form is not always reset, so
  //   last_form will actually be the prior element only if
  //   last_kind is Form.
 caon_ptr<RPI_Stage_Form> prior_form = nullptr;

 for(RPI_Stage_Element& rse : inner_elements_)
 {
  QString rset = rse.text();
  switch (rse.kind())
  {
  case RPI_Stage_Element_Kinds::Instruction_Symbol:
   {// for now
   }
   break;
  case RPI_Stage_Element_Kinds::S1_FGround_Symbol:
  case RPI_Stage_Element_Kinds::FGround_Symbol:
   if(rset.startsWith('#'))
     pgb_(step_forms_).make_token_node(rset.prepend('$'), "&fsym-node")
     = Purpose_Codes::Make_Token_Node_FSym;
   else
     pgb_(step_forms_).make_token_node(rset.prepend('@'), "&fsym-node")
     = Purpose_Codes::Make_Token_Node_FSym;

   if( flags.is_block_entry_statment
      || flags.is_inferred_block_entry_statment )
   {
    if(rai)
    {
     pgb_(step_forms_).make_statement_info_node(
       rai->text().prepend('@'), ":result",
       assignment_info_.encode_ikind().prepend(':'), "&si-node");
     pgb_(step_forms_).add_block_entry_node("!last_block_pre_entry_node",
       "&fsym-node", "!last_block_entry_node", "&si-node");
    }
    else
      pgb_(step_forms_).add_block_entry_node("!last_block_pre_entry_node",
      "&fsym-node", "!last_block_entry_node");
   }
   else if(flags.is_inferred_block_entry_statment)
   {
    if(rai)
    {
     pgb_(step_forms_).make_statement_info_node(
       rai->text().prepend('@'), ":result",
       rai->encode_ikind().prepend(':'), "&si-node");
     pgb_(step_forms_).add_statement_sequence_node("!last_statement_entry_node",
       "&fsym-node", "&si-node");
    }
    else
      pgb_(step_forms_).add_statement_sequence_node("!last_statement_entry_node", "&fsym-node");
   }
   // // non block entry handled in the block ...

   if(flags.is_statement || flags.is_assignment_expression) //? ANNOTATION_FLAG(is_nested_as_assignment))
     pgb_(step_forms_).copy_value("&fsym-node", "!last_statement_entry_node")
     = Purpose_Codes::Copy_To_Last_Statement_Entry_Node;

   pgb_(step_forms_).copy_value("&fsym-node", "!last_expression_entry_node");
   pgb_(step_forms_).copy_value("&fsym-node", "&channel-seq");
   break;

  case RPI_Stage_Element_Kinds::Literal:
   if(channel_count == 0)
     pgb_(step_forms_).add_channel_entry_token("&channel-seq", "lambda",
       rset.prepend('$'), "&channel-seq");
   else if(last_kind == RPI_Stage_Element_Kinds::Form)
   {
    QString srcn;
    if(prior_form)
    {
     if(prior_form->instruction("kb::write-anon-fdef"))
       srcn = "!prior_block_entry_node";
     else
       srcn = "!prior_expression_entry_node";
    }
    else
      srcn = "!prior_expression_entry_node";
    pgb_(step_forms_).add_channel_continue_token(srcn,
      rset.prepend('$'), "&channel-seq");
   }
   else
     pgb_(step_forms_).add_channel_token("&channel-seq",
       rset.prepend('$'), "&channel-seq");
   ++channel_count;
   break;

  case RPI_Stage_Element_Kinds::S1_Symbol:
    pgb_(step_forms_).add_channel_xentry_token("!last_expression_entry_node",
      "sigma", rset.prepend('@'), "&xchannel");
   break;

  case RPI_Stage_Element_Kinds::String_Literal:
   if(channel_count == 0)
     pgb_(step_forms_).add_channel_entry_token("&channel-seq", "lambda",
       rset.prepend('$'), "&channel-seq");
   else
     pgb_(step_forms_).add_channel_token("&channel-seq",
       rset.prepend('$'), "&channel-seq");
   ++channel_count;
   break;
  case RPI_Stage_Element_Kinds::Raw_Symbol:
   if(channel_count == 0)
     pgb_(step_forms_).add_channel_entry_token("&channel-seq", "lambda",
       rset.prepend('@'), "&channel-seq");
   else
     pgb_(step_forms_).add_channel_token("&channel-seq",
       rset.prepend('@'), "&channel-seq");
   ++channel_count;
   break;

  case RPI_Stage_Element_Kinds::Form:
   {
    caon_ptr<RPI_Stage_Form> f = rse.form();
    CAON_PTR_DEBUG(RPI_Stage_Form ,f)

    CAON_PTR_DEBUG(RPI_Stage_Form ,prior_form)

    if(f->instruction("kb::write-anon-fdef"))
    {
     f->write_unmediated(qts, nullptr);
     if(!f->step_forms().isEmpty())
     {
      pgb_(step_forms_).make_block_info_node("&bin");

      f->check_write_block_signature(step_forms_);

      if(f->flags.prior_sibling_is_fground_token)
        pgb_.insert_after_purpose(f->step_forms(), Purpose_Codes::Make_Token_Node_FSym)
        .add_channel_entry_block_node("!last_expression_entry_node", "lambda",
        "&fsym-node", "&bin", "!last_block_entry_node");
      else if(f->flags.prior_sibling_is_token)
        pgb_.insert_after_purpose(f->step_forms(), Purpose_Codes::Make_Token_Node_FSym)
        .add_channel_sequence_block_node("&channel-seq",
        "&fsym-node", "&bin", "!last_block_entry_node");
      else if(f->flags.prior_sibling_is_expression)
        pgb_.insert_after_purpose(f->step_forms(), Purpose_Codes::Make_Token_Node_FSym)
        .add_channel_continue_block_node("!prior_expression_entry_node",
        "&fsym-node", "&bin",  "!last_block_entry_node");
      else if(f->flags.prior_sibling_is_block)
        pgb_.insert_after_purpose(f->step_forms(), Purpose_Codes::Make_Token_Node_FSym)
        .add_channel_cross_block_node("!prior_block_entry_node",
        "&fsym-node", "&bin",  "!last_block_entry_node");

      pgb_(step_forms_).push_block_entry();

      step_forms_.append(f->step_forms());

      pgb_(step_forms_).pop_block_entry();
     }
    }
    else
    {
     f->write_unmediated(qts, nullptr);
     if(!f->step_forms().isEmpty())
     {
      if(f->is_non_block_expression())
      {
       QString ty = "u4"; //?

       if(f->is_deferred())
         pgb_(step_forms_).make_fsym_ground_node(
         ":?result", ty.prepend(':'),  "&fsym-ground-node");
       else
         pgb_(step_forms_).make_fsym_ground_node(
         ":result", ty.prepend(':'),  "&fsym-ground-node");

       if(f->flags.write_push_expression)
         pgb_(step_forms_).push_expression_entry();

       if(f->flags.prior_sibling_is_token)
         pgb_.insert_after_purpose(f->step_forms(), Purpose_Codes::Make_Token_Node_FSym)
         .add_fsym_ground_node(
         "&channel-seq",
         "&fsym-node", ":lambda", "&fsym-ground-node");
       else if(f->flags.prior_sibling_is_expression)
         pgb_.insert_after_purpose(f->step_forms(), Purpose_Codes::Make_Token_Node_FSym)
         .add_channel_fground_cross_node(
         "!prior_expression_entry_node",
         "&fsym-node", ":lambda", "&fsym-ground-node");
       else if(f->flags.prior_sibling_is_fground_token)
         pgb_.insert_after_purpose(f->step_forms(), Purpose_Codes::Make_Token_Node_FSym)
         .add_channel_fground_coentry_node(
         "!last_expression_entry_node",
         "&fsym-node", ":lambda", "&fsym-ground-node");
       else if(f->flags.prior_sibling_is_block)
         pgb_.insert_after_purpose(f->step_forms(), Purpose_Codes::Make_Token_Node_FSym)
         .add_channel_fground_cross_node(
         "!prior_block_entry_node",
         "&fsym-node", ":lambda", "&fsym-ground-node");
      }
      step_forms_.append(f->step_forms());

      if(f->flags.write_push_expression)
        pgb_(step_forms_).pop_expression_entry();
     }
    }
    ++channel_count;
    prior_form = f;
   }
   break;
  default:
   continue; // skip last_kind assignment ...
  }
  last_kind = rse.kind();
 }

 if(plene_block_)
 {
//  if(caon_ptr<RE_Block_Entry> rbe = plene_block_->get_block_entry())
//  {
//   if(rbe->flags.if_block)
//   {
//   }
//  }
//  else
//  {
//  }
  plene_block_->write(step_forms_, qts);
 }
// else
// {
// }

 QList<caon_ptr<RPI_Stage_Form>> nested_forms;
 int nf_count = 0;
 int count = 0;

 int skip = -1;
 int pseudo_first = -1;

 for(RPI_Stage_Element element : inner_elements_)
 {
  if(count == skip)
  {
   ++count;
   continue;
  }

  if(element.form())
  {
   caon_ptr<RPI_Stage_Form> ef = element.form();
   CAON_PTR_DEBUG(RPI_Stage_Form ,ef)

   QString note = element.text();

   if( (count == 0) && (assignment_info_.has_text() ) )
   {
    element.form()->write(qts, prior);
   }
   else if(note.isEmpty())
   {
    // //  these are deferred to after ...
    nested_forms.push_back(ef);
    ++nf_count;
   }
   else
   {
    element.form()->write(qts, prior);
   }
  }
  else
  {
  }
  ++count;
 }

 int nfb_count = 0;

 for(caon_ptr<RPI_Stage_Form> nf: nested_forms)
 {
  CAON_PTR_DEBUG(RPI_Stage_Form_Annotation ,annotation_)
  ++nfb_count;

  nf->set_nesting_level(nesting_level_ + 1);
  nf->write(qts, prior);
 }

}

//void RPI_Stage_Form::mark_unsurrounded_nested()
//{
//}

bool RPI_Stage_Form::is_deferred()
{
 return flags.is_deferred;
}

void RPI_Stage_Form::mark_deferred(int hdcode)
{
 flags.is_deferred = true;
 hdcode_ = hdcode;
}

//void RPI_Stage_Form::mark_parent_implict_end_form_before_nested_written()
//{
//}

//void RPI_Stage_Form::mark_child_implict_end_form_before_nested_written()
//{
//}

void RPI_Stage_Form::set_assignment_info(RPI_Assignment_Info& assignment_info)
{
// // // always?
// check_init_annotation();
// annotation_->flags.first_nested_is_assignment = true;

 CAON_PTR_DEBUG(RPI_Stage_Form_Annotation ,annotation_)

 assignment_info_ = assignment_info;
}

void RPI_Stage_Form::add_s1_fn_element(QString fn, QString obj)
{
 //?check_init_annotation();
 flags.has_instruction_token = true;

 inner_elements_.push_back(RPI_Stage_Element(
   RPI_Stage_Element_Kinds::S1_FGround_Symbol, fn));

 inner_elements_.push_back(RPI_Stage_Element(
   RPI_Stage_Element_Kinds::S1_Symbol, obj));
}


void RPI_Stage_Form::add_assignment_initialization_element(RPI_Stage_Element_Kinds kind, QString text)
{
 inner_elements_.push_back(RPI_Stage_Element(kind, text));
}

void RPI_Stage_Form::add_literal_element(RPI_Stage_Element_Kinds kind, QString text)
{
 inner_elements_.push_back(RPI_Stage_Element(kind, text));
}

void RPI_Stage_Form::add_insert_element(RPI_Stage_Element_Kinds kind, QString text)
{
 inner_elements_.push_back(RPI_Stage_Element(kind, text));
}

void RPI_Stage_Form::add_carrier_element(RPI_Stage_Element_Kinds kind, QString text)
{
 inner_elements_.push_back(RPI_Stage_Element(kind, text));
}

void RPI_Stage_Form::add_kauvir_element(RPI_Stage_Element_Kinds kind, QString text)
{
 inner_elements_.push_back(RPI_Stage_Element(kind, text));
}

void RPI_Stage_Form::init_inferred_s0_statement()
{
 //?check_init_annotation();
 flags.infer_write_s0_statement = true;
}

void RPI_Stage_Form::add_fn_element(RPI_Stage_Element_Kinds kind, QString text)
{
 inner_elements_.push_back(RPI_Stage_Element(kind, text));
}

void RPI_Stage_Form::add_bridge_element(RPI_Stage_Element_Kinds kind, QString text)
{
 inner_elements_.push_back(RPI_Stage_Element(kind, text));
}

void RPI_Stage_Form::add_instruction_element(RPI_Stage_Element_Kinds kind, QString text)
{
 //?check_init_annotation();
 flags.has_instruction_token = true;

// if(annotation_->flags.parent_s1_assignment_preempts_s0)
// {
//  QString ch = parent_->s1_assignment_check();
//  annotation_->flags.first_inner_element_is_s1_assignment_preempts_s0 = true;
// }

 inner_elements_.push_back(RPI_Stage_Element(kind, text));
}

void RPI_Stage_Form::add_argument_element(RPI_Stage_Element_Kinds kind, QString text)
{
 inner_elements_.push_back(RPI_Stage_Element(kind, text));
}
