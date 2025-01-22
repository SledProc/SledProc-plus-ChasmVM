
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "rz-function-def-info.h"
#include "rz-graph-core/tuple/chasm-rz-tuple-info.h"

#include "rz-graph-token/token/rz-asg-token.h"

#include "rz-graph-core/code/chasm-rz-function-def-entry.h"

#include "rz-graph-core/kernel/graph/chasm-rz-node.h"

#include "rz-graph-core/kernel/query/chasm-rz-query.h"

#include "rz-type-formation.h"

#include "rz-graph-core/code/chasm-rz-function-def-entry.h"
#include "rz-function-def-syntax.h"

#include "rz-graph-valuer/scope/rz-asg-lexical-scope.h"

#include "token/chasm-rz-token.h"

#include "multistep-token.h"
#include "multigraph-token.h"

#include "rzns.h"

USING_RZNS(GVal)

RZ_Function_Def_Info::RZ_Function_Def_Info(ChasmRZ_Function_Def_Entry& function_def_entry
  , type_name_callback_type type_name_callback, ls_callback_type ls_callback)
 : Flags(0), function_def_entry_(&function_def_entry), type_name_callback_(type_name_callback),
   ls_callback_(ls_callback), Qy(ChasmRZ_Query::instance()),
   map_key_sequence_ref_node_(nullptr),
   map_key_sequence_order_(0), ref_fdi_(nullptr)
{
 caon_ptr<tNode> fdef_node = function_def_entry.node();
 switch(function_def_entry.kind())
 {
 case ChasmRZ_Function_Def_Kinds::Call_Arrow: flags.lambda = true; break;
 case ChasmRZ_Function_Def_Kinds::Call_Arrow_Async:
  flags.lambda = true;
  flags.async = true;
  break;

 case ChasmRZ_Function_Def_Kinds::Call_Arrow_No_Def:
  flags.no_def = true;
  flags.lambda = true;
  break;
 case ChasmRZ_Function_Def_Kinds::Call_Arrow_Type_Expression: flags.type_expression = true; break;
 case ChasmRZ_Function_Def_Kinds::Call_Arrow_With_Matching: flags.with_matching = true; break;
 case ChasmRZ_Function_Def_Kinds::Call_Arrow_Monad: flags.lambda = true; flags.monad = true; break;
 default: break;
 }

 if(fdef_node)
 {
  init_channels(*fdef_node);
 }

}


QString RZ_Function_Def_Info::get_label()
{
 CAON_PTR_DEBUG(ChasmRZ_Function_Def_Entry ,function_def_entry_)
 QString result;
 if(function_def_entry_->label_node())
 {
  if(caon_ptr<ChasmRZ_Token> token = function_def_entry_->label_node()->chasm_rz_token())
  {
   result = token->raw_text();
  }
 }
 return result;
}

caon_ptr<RZ_Function_Def_Info::tNode> RZ_Function_Def_Info::function_def_entry_node()
{
 return function_def_entry_->node();
}

QString RZ_Function_Def_Info::channel_string(Channel_Types ct, const RZ_Function_Def_Syntax& syntax)
{
 auto it = entry_nodes_map_.find(ct);
 if(it != entry_nodes_map_.end())
 {
  return channel_string(syntax, it.value());
 }
 return QString();
}

QString RZ_Function_Def_Info::lambda_channel_string(const RZ_Function_Def_Syntax& syntax)
{
 return channel_string(Channel_Types::Lambda, syntax);
}

QString RZ_Function_Def_Info::sigma_channel_string(const RZ_Function_Def_Syntax& syntax)
{
 return channel_string(Channel_Types::Sigma, syntax);
}

QString RZ_Function_Def_Info::channel_string(const RZ_Function_Def_Syntax& syntax, caon_ptr<ChasmRZ_Node> sequence_node)
{
 QString separator = syntax.symbol_separator();
 QString argument_default_type = syntax.argument_default_type();
 QString type_from_value_separator = syntax.type_from_symbol_separator();
 QString result;
 QString type_name;
 QString symbol_name;
 QString type_expression;
 bool awaiting_type_name = false;
 bool current_symbol_name = false;
 bool held_symbol_name = false;
 int depth_change = 0;
 int current_depth = 0;
 int held_depth_change = 0;

 while(sequence_node)
 {
  CAON_PTR_DEBUG(tNode ,sequence_node)
  held_depth_change = depth_change;
  if(caon_ptr<RZ_ASG_Token> tok = channel_sequence(sequence_node, depth_change))
  {
   CAON_PTR_DEBUG(RZ_ASG_Token ,tok)
   if(tok->flags.is_empty_tuple_indicator)
   {
    //  for a .() -- but check if this can appear nested...
    return QString();

   }

    if(held_depth_change > 0)
    {
     current_depth += held_depth_change;
     type_expression += QString(held_depth_change, '(');
    }
    else if(held_depth_change < 0)
    {
     current_depth += held_depth_change;
     type_expression += QString(-held_depth_change, ')');
    }

   RZ_ASG_Token& token = *tok;
   if(current_depth > 0)
   {
    QString type_symbol = type_name_callback_(token.string_value());
    type_expression += type_symbol + " ";
    continue;
   }
   if(awaiting_type_name && !type_expression.isEmpty())
   {
    awaiting_type_name = false;
    if(syntax.flags.hide_type)
    {
     result += symbol_name + ' ' + separator;
    }
    else if(syntax.flags.hide_symbol)
    {
     result += type_expression;
     result += ' ' + separator;
    }
    else
    {
     // //?
    }
    type_expression.clear();
   }
   if(token.string_value() == "<-")
   {
    held_symbol_name = true;
    awaiting_type_name = true;
    current_symbol_name = false;
   }
   else if(awaiting_type_name)
   {
    held_symbol_name = false;
    awaiting_type_name = false;
    type_name = type_name_callback_(token.string_value());
    if(syntax.flags.hide_type)
    {
     result += symbol_name + ' ' + separator;
    }
    else
    {
     RZ_Type_Formation formation(type_name);
     if(syntax.flags.type_before_symbol)
     {
      result += formation.cpp_out();
      if(!syntax.flags.hide_symbol)
       result += ' ' + symbol_name;
      if(held_depth_change == 0)
       result += separator + ' ';
     }
     else
     {
      if(!syntax.flags.hide_symbol)
       result += symbol_name + ' ';
      result += type_from_value_separator + ' ' + formation.cpp_out() + " " + separator + ' ';
     }
    }
   }
   else if(current_symbol_name)
   {
    if(syntax.flags.hide_symbol)
     continue;
    if(argument_default_type.isEmpty())
     result += symbol_name + separator + ' ';
    else if(syntax.flags.type_before_symbol)
     result += argument_default_type + ' ' + symbol_name + separator + ' ';
    else
     result += symbol_name + ' ' + type_from_value_separator + ' ' + separator + ' ';
    symbol_name = token.string_value();
    syntax.check_symbol_rename(symbol_name);
   }
   else
   {
    symbol_name = token.string_value();
    current_symbol_name = true;
    syntax.check_symbol_rename(symbol_name);
   }
  }
 }

 if(!syntax.flags.hide_symbol)
 {
  if(held_symbol_name)
  {
   result += symbol_name + ' ';
  }
 }

 if(!syntax.flags.hide_type)
 {
  if(awaiting_type_name)
  {
   if(!type_expression.isEmpty())
   {
    result += type_expression;
   }
  }

  if(depth_change < 0)
  {
   current_depth -= depth_change;
   result += QString(-depth_change, ')');
  }
 }

 if(current_symbol_name)
 {
  if(syntax.flags.hide_symbol) ;
  else if(argument_default_type.isEmpty())
   result += symbol_name + separator + ' ';
  else if(syntax.flags.type_before_symbol)
   result += argument_default_type + ' ' + symbol_name + separator + ' ';
  else
   result += symbol_name + ' ' + type_from_value_separator + ' ' + separator + ' ';
 }

 if(result.endsWith(' '))
  result.chop(1);
 if(result.endsWith(separator))
  result.chop(separator.length());
 return result;
}

// //?  should we check symbols for the lexical scope before this ...?
QString RZ_Function_Def_Info::kauvir_entry_code_string(RZ_ASG_Lexical_Scope& ls)
{
 QString result = kauvir_entry_code_string_by_channel_type(Channel_Types::Sigma, ls);
 result += kauvir_entry_code_string_by_channel_type(Channel_Types::Lambda, ls);

 return result;
}

QString RZ_Function_Def_Info::dynamo_signature_code_string()
{
 QString result =
   // // Just progn?
   "\nprogn ; sig ..."
    //   Split up the string to add the progn comment...
   "\n(ka::kc :|kcg_clear_all| kcg)";
 result += dynamo_signature_code_string_by_channel_type(Channel_Types::Sigma);
 result += dynamo_signature_code_string_by_channel_type(Channel_Types::Lambda);
 result += dynamo_signature_code_string_by_channel_type(Channel_Types::Return);
 result += "\n(setq kto (ka::kc :|kcm_type_object_from_channel_group| kcg))";

 //? what was this for?
 //?result += "\n()";
 return result;
}

QString RZ_Function_Def_Info::dynamo_signature_code_for_type(QString carrier_kind, QString type_name)
{
 // //  ka, right, not kb?
 QString result =
   QString("\n(ka::kc :|kcg_add_%1_carrier_via_type_name| kcg \"%2\")").arg(carrier_kind).arg(type_name);
 return result;
}

QString RZ_Function_Def_Info::dynamo_signature_code_for_symbol(QString carrier_kind, QString symbol_name)
{
  // //  ka, right, not kb?

 QString result =
   QString("\n(ka::kc :|kcg_add_%1_carrier_via_symbol_name| kcg \"%2\")").arg(carrier_kind).arg(symbol_name);
 return result;
}

QString RZ_Function_Def_Info::dynamo_signature_code_for_symbol(QString carrier_kind, QString symbol_name, QString type_name)
{
 QString result =
   QString("\n(ka::kc :|kcg_add_%1_carrier_via_symbol_and_type_name| kcg \"%2\" \"%3\")").arg(carrier_kind).arg(symbol_name).arg(type_name);
 return result;
}

QString RZ_Function_Def_Info::dynamo_signature_code_for_empty_channel(QString carrier_kind)
{
 QString result =
   QString("\n(ka::kc :|kcg_add_empty_%1_channel| kcg)").arg(carrier_kind);
 return result;
}

QString RZ_Function_Def_Info::dynamo_signature_code_string_by_channel_type(Channel_Types ct)
{
 caon_ptr<ChasmRZ_Node> sequence_node = nullptr;

 QString channel_name_code;

 switch(ct)
 {
 case Channel_Types::Lambda:
  channel_name_code = "lambda";
  goto ok_sequence_node;


 case Channel_Types::Sigma:
  channel_name_code = "sigma";
  goto ok_sequence_node;


 case Channel_Types::Return:
  channel_name_code = "return";
  goto ok_sequence_node;

 default:
  break;

 ok_sequence_node: sequence_node = entry_nodes_map_.value(ct); break;

 }
 QString result;

 signed int depth_change;

 QString symbol_name;
 bool awaiting_type_name = false;

 int position_count = 0;
 while(sequence_node)
 {
  // this advances sequence_node
  if(caon_ptr<RZ_ASG_Token> rzlt = channel_sequence(sequence_node, depth_change))
  {
   CAON_PTR_DEBUG(RZ_ASG_Token ,rzlt)
   if(rzlt->flags.is_empty_tuple_indicator)
   {
    return dynamo_signature_code_for_empty_channel(channel_name_code);
   }
   QString rt = rzlt->raw_text();
   if(rt == "<-")
   {
    awaiting_type_name = true;
   }
   else if(awaiting_type_name)
   {
    result += dynamo_signature_code_for_symbol(channel_name_code, symbol_name, rt);
    symbol_name.clear();
   }
   else if(symbol_name.isEmpty())
   {
    symbol_name = rt;
   }
   else
   {
    result += dynamo_signature_code_for_type(channel_name_code, symbol_name);
    result += dynamo_signature_code_for_type(channel_name_code, rt);
    symbol_name.clear();
   }
  }
 }
 if(!symbol_name.isEmpty())
 {
  // left over type ...
  result += dynamo_signature_code_for_type(channel_name_code, symbol_name);
 }
 return result;
}


QString RZ_Function_Def_Info::kauvir_entry_code_string_by_channel_type(Channel_Types ct, RZ_ASG_Lexical_Scope& ls)
{
 caon_ptr<ChasmRZ_Node> sequence_node;

 QString channel_name_code;

 switch(ct)
 {
 case Channel_Types::Lambda:
  channel_name_code = "L";
  goto ok_sequence_node;

 case Channel_Types::Sigma:
  channel_name_code = "S";
  goto ok_sequence_node;

 default: break;

 ok_sequence_node: sequence_node = entry_nodes_map_.value(ct); break;

 }
 QString result;

 signed int depth_change;

 QString symbol_name;
 bool awaiting_type_name = false;

 int position_count = 0;
 while(sequence_node)
 {
  // this advances sequence_node
  if(caon_ptr<RZ_ASG_Token> rzlt = channel_sequence(sequence_node, depth_change))
  {
   QString rt = rzlt->raw_text();
   if(rt == "<-")
   {
    awaiting_type_name = true;
   }
   else if(awaiting_type_name)
   {
    awaiting_type_name = false;

    MS_Token sym_ms {MS_Token_Kinds::Scoped_Symbol, symbol_name};
    MS_Token type_ms {MS_Token_Kinds::Type_Symbol, rt};
    result += QString("\n(kb::write-plebod-symbol-declare-with-type %1 %2)")
      .arg(sym_ms.encode()).arg(type_ms.encode());

     ++position_count;

    MS_Token pxyv_ms {MS_Token_Kinds::Proxy_Value_Symbol, channel_name_code + QString::number(position_count)};
    result += QString("\n(kb::write-plebod-symbol-init-with-type %1 %2)")
      .arg(sym_ms.encode()).arg(pxyv_ms.encode());

    symbol_name.clear();
   }
   else
   {
    symbol_name = rt;
   }
  }
 }

 return result;
}


caon_ptr<RZ_ASG_Token> RZ_Function_Def_Info::channel_sequence(caon_ptr<tNode>& sequence_node, signed int& depth_change)
{
 // //   Get or init the token and advance
 caon_ptr<RZ_ASG_Token> result = nullptr;
 CAON_PTR_DEBUG(ChasmRZ_Node ,sequence_node)
 if(caon_ptr<ChasmRZ_Token> retok = sequence_node->chasm_rz_token())
 {
  result = RZ_ASG_Token::check_init_asg_token(*retok);
  depth_change = 0;
 }
 if(caon_ptr<ChasmRZ_Node> next_node = Qy.Run_Call_Sequence(sequence_node))
  sequence_node = next_node;
 else if(next_node = Qy.Run_Call_Entry(sequence_node))
 {
  depth_change = 1;
  entry_nodes_.push(next_node);
  sequence_node = Qy.Run_Call_Entry(next_node);
 }
 else if(entry_nodes_.empty())
 {
  sequence_node = nullptr;
 }
 else
 {
  depth_change = -1;
  caon_ptr<ChasmRZ_Node> en = entry_nodes_.pop();
  CAON_PTR_DEBUG(ChasmRZ_Node ,en)
  sequence_node = Qy.Run_Cross_Sequence(en);
  CAON_PTR_DEBUG(ChasmRZ_Node ,sequence_node)
  if(sequence_node)
  {
   if(caon_ptr<ChasmRZ_Node> next_node = Qy.Run_Call_Entry(sequence_node))
   {
    entry_nodes_.push(sequence_node);
    sequence_node = Qy.Run_Call_Entry(next_node);
   }
  }
 }
 return result;
}

QString RZ_Function_Def_Info::context_channel_string()
{
 if(caon_ptr<tNode> context_channel_entry_node = entry_nodes_map_.value(Channel_Types::Context))
 {
  QString result = "[[";
  CAON_PTR_DEBUG(ChasmRZ_Node ,context_channel_entry_node)
  if(caon_ptr<ChasmRZ_Token> retok = context_channel_entry_node->chasm_rz_token())
  {
   CAON_PTR_DEBUG(ChasmRZ_Token ,retok)
   caon_ptr<RZ_ASG_Token> rzlt = RZ_ASG_Token::check_init_asg_token(*retok);
   result += rzlt->string_value();
  }
  result += "]]";
  return result;
 }
 return QString();
}


QString RZ_Function_Def_Info::return_channel_string(int& token_count)
{
 QString result;
 token_count = 0;
 caon_ptr<tNode> sequence_node = entry_nodes_map_.value(Channel_Types::Context);
 int depth_change = 0;
 while(sequence_node)
 {
  int held_depth_change = depth_change;
  if(caon_ptr<RZ_ASG_Token> tok = channel_sequence(sequence_node, depth_change))
  {
   if(held_depth_change > 0)
   {
    result += QString(held_depth_change, '(');
   }
   else if(held_depth_change < 0)
   {
    result += QString(-held_depth_change, ')');
   }
   RZ_ASG_Token& token = *tok;
   ++token_count;
   result += type_name_callback_(token.string_value());
   if(held_depth_change == 0)
    result += ',';
   result += ' ';
  }
 }
 if(depth_change < 0)
 {
  if(result.endsWith(' '))
   result.chop(1);
  if(result.endsWith(','))
   result.chop(1);
  result += QString(-depth_change, ')');
 }

 if(result.endsWith(' '))
  result.chop(1);
 if(result.endsWith(','))
  result.chop(1);
 return result;
}

void RZ_Function_Def_Info::init_channels(tNode& fdef_node)
{
 caon_ptr<tNode> data_entry_node;
 caon_ptr<tNode> tuple_info_node;
 data_entry_node = Qy.Run_Call_Entry(&fdef_node);
 while(data_entry_node)
 {
  CAON_PTR_DEBUG(ChasmRZ_Node ,data_entry_node)
  if(tuple_info_node = Qy.Run_Data_Entry(data_entry_node))
  {
   CAON_PTR_DEBUG(ChasmRZ_Node ,tuple_info_node)

   tuple_info_node->debug_connections();

   if(caon_ptr<ChasmRZ_Tuple_Info> pRti = tuple_info_node->chasm_rz_tuple_info())
   {
    ChasmRZ_Tuple_Info& rti = *pRti;
    ChasmRZ_Tuple_Info::Function_Def_Channels ch = rti.match_function_def_channel();
    switch(ch)
    {
    case ChasmRZ_Tuple_Info::Function_Def_Channels::Lambda_Channel:
     if(flags.monad)
       entry_nodes_map_[Channel_Types::Monad] = Qy.Run_Data_Entry(tuple_info_node);
     else
       entry_nodes_map_[Channel_Types::Lambda] = Qy.Run_Data_Entry(tuple_info_node);
     break;
    case ChasmRZ_Tuple_Info::Function_Def_Channels::Context_Channel:
     entry_nodes_map_[Channel_Types::Context] = Qy.Run_Data_Entry(tuple_info_node);
     break;
    case ChasmRZ_Tuple_Info::Function_Def_Channels::Sigma_Channel:
      entry_nodes_map_[Channel_Types::Sigma] = Qy.Run_Data_Entry(tuple_info_node);
     break;
    case ChasmRZ_Tuple_Info::Function_Def_Channels::Return_Channel:
      entry_nodes_map_[Channel_Types::Return] = Qy.Run_Data_Entry(tuple_info_node);
     break;
    case ChasmRZ_Tuple_Info::Function_Def_Channels::Error_Channel:
      entry_nodes_map_[Channel_Types::Error] = Qy.Run_Data_Entry(tuple_info_node);
     break;
    case ChasmRZ_Tuple_Info::Function_Def_Channels::CTOR_Ret_Channel:
      entry_nodes_map_[Channel_Types::CTOR_Ret] = Qy.Run_Data_Entry(tuple_info_node);
     break;
    case ChasmRZ_Tuple_Info::Function_Def_Channels::CTOR_Mem_Channel:
      entry_nodes_map_[Channel_Types::CTOR_Mem] = Qy.Run_Data_Entry(tuple_info_node);
     break;
    }
   }
  }
  data_entry_node = Qy.Run_Cross_Data(data_entry_node);
 }
}



void RZ_Function_Def_Info::write_phr_signature_code(PGB_IR_Build& pgb,
  QList<PGB_IR_Build::Text_With_Purpose>& step_forms)
{
 pgb.hold_token("sig-node", MG_Token_Kinds::Arg_Ledger_Target);

 write_phr_signature_code_by_channel_type(pgb, step_forms, Channel_Types::Sigma);
 write_phr_signature_code_by_channel_type(pgb, step_forms, Channel_Types::Lambda);
 write_phr_signature_code_by_channel_type(pgb, step_forms, Channel_Types::Return);

 pgb.write_with_held_tokens("signature", step_forms);
 pgb.clear_held_tokens();
}


void RZ_Function_Def_Info::write_phr_signature_code_by_channel_type(PGB_IR_Build& pgb,
  QList<PGB_IR_Build::Text_With_Purpose>& step_forms, Channel_Types ct)
{
 caon_ptr<ChasmRZ_Node> sequence_node = nullptr;

 QString channel_name_code;

 switch(ct)
 {
 case Channel_Types::Lambda:
  channel_name_code = "lambda";
  goto ok_sequence_node;


 case Channel_Types::Sigma:
  channel_name_code = "sigma";
  goto ok_sequence_node;


 case Channel_Types::Return:
  channel_name_code = "return";
  goto ok_sequence_node;

 default:
  break;

 ok_sequence_node: sequence_node = entry_nodes_map_.value(ct); break;

 }
 QString result;

 signed int depth_change;

 QString symbol_name;
 bool awaiting_type_name = false;

 int position_count = 0;
 while(sequence_node)
 {
  // this advances sequence_node
  if(caon_ptr<RZ_ASG_Token> rzlt = channel_sequence(sequence_node, depth_change))
  {
   CAON_PTR_DEBUG(RZ_ASG_Token ,rzlt)
   if(rzlt->flags.is_empty_tuple_indicator)
   {
    write_phr_signature_code_for_empty_channel(pgb, step_forms, channel_name_code);
    return;
   }
   else if(position_count == 0)
   {
    write_phr_signature_code_channel_entry(pgb, step_forms, channel_name_code);
   }
   QString rt = rzlt->raw_text();
   if(rt == "<-")
   {
    awaiting_type_name = true;
   }
   else if(awaiting_type_name)
   {
    write_phr_signature_code_for_symbol(pgb, step_forms, channel_name_code, symbol_name, rt);
    ++position_count;
    symbol_name.clear();
   }
   else if(symbol_name.isEmpty())
   {
    symbol_name = rt;
    ++position_count;
   }
   else
   {
    write_phr_signature_code_for_type(pgb, step_forms, channel_name_code, symbol_name);
    write_phr_signature_code_for_type(pgb, step_forms, channel_name_code, rt);
    symbol_name.clear();
    ++position_count;
   }
  }
 }
 if(!symbol_name.isEmpty())
 {
  // left over type ...
  write_phr_signature_code_for_type(pgb, step_forms, channel_name_code, symbol_name);
 }
}

void RZ_Function_Def_Info::write_phr_signature_code_channel_entry(PGB_IR_Build& pgb,
 QList<PGB_IR_Build::Text_With_Purpose>& step_forms, QString carrier_kind)
{
 pgb.hold_token(carrier_kind, MG_Token_Kinds::Sig_Channel);
}

void RZ_Function_Def_Info::write_phr_signature_code_for_empty_channel(PGB_IR_Build& pgb,
  QList<PGB_IR_Build::Text_With_Purpose>& step_forms, QString carrier_kind)
{
}

void RZ_Function_Def_Info::write_phr_signature_code_for_type(PGB_IR_Build& pgb,
  QList<PGB_IR_Build::Text_With_Purpose>& step_forms,
  QString carrier_kind, QString type_name)
{
 pgb.hold_token(type_name, MG_Token_Kinds::Sig_Type);
}

void RZ_Function_Def_Info::write_phr_signature_code_for_symbol(PGB_IR_Build& pgb,
  QList<PGB_IR_Build::Text_With_Purpose>& step_forms,
  QString carrier_kind, QString symbol_name)
{
 pgb.hold_token(symbol_name, MG_Token_Kinds::Sig_Symbol);
}

void RZ_Function_Def_Info::write_phr_signature_code_for_symbol(PGB_IR_Build& pgb,
  QList<PGB_IR_Build::Text_With_Purpose>& step_forms,
  QString carrier_kind, QString symbol_name, QString type_name)
{
 pgb.hold_token(symbol_name, MG_Token_Kinds::Sig_Symbol_Before_Type);
 pgb.hold_token(type_name, MG_Token_Kinds::Sig_Type);
}
