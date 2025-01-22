
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "rz-asg-token.h"

#include "rz-graph-core/token/chasm-rz-token.h"

#include "rz-graph-valuer/string/rz-string.h"


//?#include "valuer/rz-asg-core-function.h"
#include "rz-asg-core-casement-function.h"


#include "rzns.h"

USING_RZNS(GBuild)

RZ_ASG_Token::RZ_ASG_Token(QString raw_text, Special_Constructor_Mode scm, int line_number)
 : Flags(0), raw_text_(raw_text), line_number_(line_number), out_(nullptr)
{

}


RZ_ASG_Token::RZ_ASG_Token(QString raw_text, int line_number)
 : Flags(0), raw_text_(raw_text), line_number_(line_number), out_(nullptr)
{
 if(raw_text_.endsWith('?'))
 {
  if(raw_text.size() == 1)
  {
   return;
  }
  if(raw_text.startsWith('='))
  {
   return;
  }
  if(raw_text.startsWith('#'))
  {
   return;
  }
  if(raw_text.startsWith('-'))
  {
   return;
  }
  flags.final_question_mark = true;
  raw_text_.chop(1);
 }
 else if(raw_text[0] == '?')
 {
  raw_text_ = raw_text_.mid(1);
  flags.initial_question_mark = true;
 }

}

caon_ptr<RZ_ASG_Token> RZ_ASG_Token::check_init_asg_token(ChasmRZ_Token& chasm_rz_token)
{
 caon_ptr<RZ_ASG_Token> result = chasm_rz_token.asg_token();
 if(!result)
 {
  init_asg_token(chasm_rz_token);
  result = chasm_rz_token.asg_token();
 }
 return result;
}


void RZ_ASG_Token::init_asg_token(ChasmRZ_Token& chasm_rz_token)
{
 caon_ptr<RZ_ASG_Token> rlt;
 if(chasm_rz_token.flags.is_string_literal)
 {
  rlt = new RZ_ASG_Token(chasm_rz_token.string_value(), RZ_ASG_Token::Special_Constructor_Mode::Raw_Text);
  rlt->flags.is_string_literal = true;
  chasm_rz_token.set_asg_token(rlt);
  return;
 }
 rlt = new RZ_ASG_Token(chasm_rz_token.string_value());

 chasm_rz_token.set_asg_token(rlt);

 rlt->flags.is_keyword = chasm_rz_token.flags.is_keyword;
 rlt->flags.is_symbol_declaration = chasm_rz_token.flags.is_symbol_declaration;
 rlt->flags.is_string_literal = chasm_rz_token.flags.is_string_literal;
 rlt->flags.is_numeric_literal = chasm_rz_token.flags.is_numeric_literal;
 //?rlt->flags.is_xq_literal = chasm_rz_token.flags.is_xq_literal;
 rlt->flags.is_raw_asg = chasm_rz_token.flags.is_raw_asg;

 rlt->flags.is_quoted = chasm_rz_token.flags.is_quoted;

 rlt->flags.is_declaration_arrow = chasm_rz_token.is_declaration_arrow();
 rlt->flags.is_mapkey = chasm_rz_token.flags.is_mapkey;
 rlt->flags.is_do_mapkey = chasm_rz_token.flags.is_do_mapkey;
 rlt->flags.has_mapkey = chasm_rz_token.flags.has_mapkey;
 rlt->flags.is_match_literal = chasm_rz_token.flags.is_match_literal;
 rlt->flags.is_empty_tuple_indicator = chasm_rz_token.flags.is_empty_tuple_indicator;

 rlt->flags.is_cpp_scoped = chasm_rz_token.flags.is_cpp_scoped;
 rlt->flags.not_entry = chasm_rz_token.flags.not_entry;
 rlt->flags.has_type_indicator = chasm_rz_token.flags.has_type_indicator;

 rlt->flags.follows_call_arrow = chasm_rz_token.flags.follows_call_arrow;

 rlt->flags.precedes_call_arrow = chasm_rz_token.flags.precedes_call_arrow;

}


void RZ_ASG_Token::skip_asg_out()
{
 flags.skip_asg_out = true;
}

void RZ_ASG_Token::comment_asg(QString str)
{
 check_out();
}

void RZ_ASG_Token::redirect_asg(QString str)
{
 check_out();
}

void RZ_ASG_Token::redirect_paste(QString str)
{
 check_out();
}


void RZ_ASG_Token::redirect_asg(QString str,
 std::function<void(QString&)>* callback)
{
 flags.out_redirect = true;
 check_out();
}

void RZ_ASG_Token::redirect_asg(QString str,
 RZ_ASG_Node& node)
{
 flags.out_redirect = true;
 check_out();
}

void RZ_ASG_Token::redirect_asg_in_form(QString str,
 RZ_ASG_Node& node)
{
 flags.out_redirect = true;
 check_out();
}


void RZ_ASG_Token::check_out()
{
//? if(!out_)
//  out_ = new RZ_ASG_Token_Out_Info;
}


caon_ptr<RZ_Type_Object> RZ_ASG_Token::type_object()
{
 return vh_.type_object();
}

void RZ_ASG_Token::set_type_object(RZ_Type_Object& cto)
{
 vh_.set_type_object(&cto);
}

void RZ_ASG_Token::mark_as_string_literal()
{
 set_new_cast_value<RZ_String, QString>(raw_text_);
}

caon_ptr<RZ_ASG_Token_Out_Info> RZ_ASG_Token::get_asg_out
 (RZ_Type_Variety& tv, QString& result)
{
 if(out_)
 {
  return out_;
 }
 if(flags.is_empty_tuple)
 {
  result += "(rz-empty-tuple)";
  return nullptr;
 }
 if(caon_ptr<RZ_ASG_Core_Casement_Function> ccf = pRestore<RZ_ASG_Core_Casement_Function>(tv))
 {
  if(ccf->flags.paste)
  {

  }
  result = ccf->get_out_name();
 }
 else if(flags.is_core_function_symbol)
 {
  caon_ptr<RZ_ASG_Core_Casement_Function> ccf = pRestore<RZ_ASG_Core_Casement_Function>();
  result = ccf->get_out_name_or_string();
 }
 else
  result = asg_string_value();
 return nullptr;
}

QString RZ_ASG_Token::asg_string_value()
{
 QString result;
 if(flags.is_keyword)
 {
  result = string_value();
  result.prepend(':');
 }
 else if(flags.follows_call_arrow)
 {
  result = string_value();
  result.prepend(":|.");
  result.append('|');
 }
 else if(flags.is_string_literal)
 {
  if(flags.is_double_back_quoted)
   result = QString("``") + string_value();
  else
   result = QString('"') + string_value() + '"';
 }
 else if(flags.is_double_quoted)
 {
  result = QString("''") + string_value();
 }
 else if(flags.is_quoted)
 {
  result = QString("'") + string_value();
 }
 else
  result = string_value();
 return result;
}


Basic_Token_Kinds RZ_ASG_Token::kind_and_string_value(QString& result) const
{
 result = raw_text_;
 // //  Put this here for now because not every
  //    follows_call_arrow scenario will necessarily
  //    demand the :|.| construction ...
 if(flags.follows_call_arrow)
 {
  result.prepend(":|.");
  result.append('|');
  return Basic_Token_Kinds::Symbol_Token;
 }
 else if(flags.is_string_literal)
  return Basic_Token_Kinds::String_Token;
 else
  return Basic_Token_Kinds::Symbol_Token;
}


QString RZ_ASG_Token::cpp_string_value()
{
 QString result = asg_string_value();
 result.replace('-', '_');
 return result;
}


