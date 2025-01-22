
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "rz-asg-scope-token.h"

#include "token/rz-asg-token.h"

USING_RZNS(GVal)
USING_RZNS(GBuild)

RZ_ASG_Scope_Token::RZ_ASG_Scope_Token(QString channel_code)
 : channel_code_(channel_code), function_token_(nullptr),
   declaration_token_(nullptr), value_node_(nullptr)
{

}



RZ_ASG_Scope_Token::RZ_ASG_Scope_Token(caon_ptr<RZ_ASG_Token> f,
  caon_ptr<RZ_ASG_Token> d, caon_ptr<ChasmRZ_Node> n)
 : function_token_(f), declaration_token_(d), value_node_(n)
{

}

RZ_ASG_Scope_Token::RZ_ASG_Scope_Token(RZ_ASG_Token& f,
  RZ_ASG_Token& d, QString index_key)
 :  function_token_(&f), declaration_token_(&d),
    value_node_(nullptr), index_key_(index_key)
{

}


RZ_ASG_Scope_Token::RZ_ASG_Scope_Token(RZ_ASG_Token& f, RZ_ASG_Token& d,
  QString dtn, caon_ptr<ChasmRZ_Node> n)
 : function_token_(&f), declaration_token_(&d), declared_type_name_(dtn), value_node_(n)
{

}


QString RZ_ASG_Scope_Token::value_to_string() const
{
 return vh_.to_string();
}

QString RZ_ASG_Scope_Token::type_to_string() const
{
 if(declared_type_name_.isEmpty())
  return vh_.type_to_string();
 else
 {
  return declared_type_name_;
 }
}


bool RZ_ASG_Scope_Token::type_matches(caon_ptr<RZ_Type_Object> rto) const
{
 return vh_.type_object() == rto;
}

QString RZ_ASG_Scope_Token::value_to_string(QString& type, RZ_Type_Variety&) const
{
 return value_to_string(type);
}


QString RZ_ASG_Scope_Token::value_to_string(QString& type) const
{
 if(declared_type_name_.isEmpty())
  return vh_.to_string_with_type(type);
 else
 {
  type = declared_type_name_;
  return vh_.to_string();
 }
}

QString RZ_ASG_Scope_Token::value_to_asg_string() const
{
 const RZ_Type_Object& rto = *vh_.type_object();
 if(rto.id() == RZ_Run_Types::Opc)
 {
  return "nil";
 }
 else if(rto.name() == "Vec")
 {
  return "nil";
 }
 else if(rto.id() == RZ_Run_Types::FnDefInfo)
 {
  return "nil";
 }
 return vh_.to_asg_string();
}

Basic_Token_Kinds RZ_ASG_Scope_Token::initial_kind_and_string_value(QString& result) const
{
 CAON_PTR_DEBUG(RZ_ASG_Token ,function_token_)
 CAON_PTR_DEBUG(RZ_ASG_Token ,declaration_token_)

 result = value_to_asg_string();
 return Basic_Token_Kinds::Symbol_Token;
}

