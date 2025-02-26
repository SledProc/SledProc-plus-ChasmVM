
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef GRAPH_CALL_TC__H
#define GRAPH_CALL_TC__H

#include "rz-graph-token/token/rz-asg-token.h"

#include "rz-graph-valuer/valuer/rz-asg-valuer.h"

#include "functions/rz-asg-function-families.h"

#include "token/rz-block-level-type-declaration.h"


#include "rzns.h"

RZNS_(GVal)

class RZ_ASG_Symbol;

_RZNS(GVal)


USING_RZNS(GVal)
USING_RZNS(GBuild)

RZNS_(GRun)

#define RZ_ASG_FUNCTION_DECLARE(str, name, arity, status) name,

RZ_ASG_FUNCTION_CODES_(RZ_Graph_Call_Tc)
 null = 0,
 #include "core-functions-tc.h"
_RZ_ASG_FUNCTION_CODES

#undef RZ_ASG_FUNCTION_DECLARE



RZ_GCALL_IMPLEMENT <RZ_GCALL_Tc(Lex_Decl, Core_Class)>
{

 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, T1& t1, T2& t2)
 {
 }

 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& token, T2& t2)
 {
 }

 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& token, RZ_Block_Level_Type_Declaration& blt) //T2& t2)
 {

  caon_ptr<RZ_ASG_Token> ft = rh.function_token();

  CAON_PTR_DEBUG(RZ_ASG_Token ,ft)

  if(ft)
  {
   caon_ptr<ChasmRZ_Node> n = rh.valuer().register_block_level_type_declaration(*ft, token, blt);
//?   rh.mark_continue_statement(n);
  }


//  caon_ptr<ChasmRZ_Node> fnode = rh.function_node();
//  CAON_PTR_DEBUG(ChasmRZ_Node ,fnode)

//  caon_ptr<RZ_Type_Object> rto = token.type_object();
//  CAON_PTR_DEBUG(RZ_Type_Object ,rto)

//  caon_ptr<RZ_Block_Level_Type_Declaration> blt =
//    token.pRestore<RZ_Block_Level_Type_Declaration>();
//  CAON_PTR_DEBUG(RZ_Block_Level_Type_Declaration ,blt)

//  caon_ptr<ChasmRZ_Token> btoken = blt->token();
//  CAON_PTR_DEBUG(ChasmRZ_Token ,btoken)


//  caon_ptr<RZ_ASG_Token> asg_token = btoken->asg_token();
//  CAON_PTR_DEBUG(RZ_ASG_Token ,asg_token)

//  CAON_DEBUG_NOOP


 }

// static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& start_token, caon_ptr<tNode> pass_node)
// {
//  caon_ptr<ChasmRZ_Node> fnode = rh.function_node();
//  CAON_PTR_DEBUG(ChasmRZ_Node ,fnode)
//  CAON_DEBUG_NOOP
////  caon_ptr<RZ_ASG_Token> ftok = rh.function_token();
////  CAON_PTR_DEBUG(RZ_ASG_Token ,ftok)
////  CAON_DEBUG_NOOP

//  //QString scope_kind =
////  scope_kind.prepend("#_rz-");
////  rh.function_token()->redirect_paste(scope_kind);
// }
};




RZ_GCALL_IMPLEMENT <RZ_GCALL_Tc(My, Core_Class)>
{
 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& token, T2& t2)
 {

 }
 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& token, RZ_Opaque_Call& opc)
 {
  caon_ptr<RZ_ASG_Token> ft = rh.function_token();

  if(ft)
  {
   caon_ptr<tNode> n = rh.valuer().register_lexical_symbol(*ft, token, opc);
   rh.mark_continue_statement(n);
  }

 }

 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& token, RZ_Opaque_Type_Symbol& ots)
 {
  caon_ptr<RZ_ASG_Token> ft = rh.function_token();

  if(ft)
  {
   caon_ptr<tNode> n = rh.valuer().register_lexical_symbol(*ft, token, ots);
   rh.mark_continue_statement(n);
  }
 }

 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& token,
  RZ_ASG_Symbol& sym)
 {
  caon_ptr<RZ_ASG_Token> ft = rh.function_token();
  if(ft)
  {
   caon_ptr<tNode> n = rh.valuer().register_lexical_symbol(rh, *ft, token, sym);
   rh.mark_continue_statement(n);
  }
 }

 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& token,
  RZ_ASG_Vector& symvec)
 {
  caon_ptr<RZ_ASG_Token> ft = rh.function_token();
  if(ft)
  {
   caon_ptr<tNode> n = rh.valuer().register_lexical_symbols(rh, *ft, token, symvec);
   rh.mark_continue_statement(n);
  }
 }
};


RZ_GCALL_IMPLEMENT <RZ_GCALL_Tc(Our, Core_Class)>
{
 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& token, T2& t2)
 {

 }
 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& token, RZ_Opaque_Call& opc)
 {
  caon_ptr<RZ_ASG_Token> ft = rh.function_token();

  if(ft)
  {
   caon_ptr<tNode> n = rh.valuer().register_lexical_symbol(*ft, token, opc,
     rh.valuer().current_logical_scope());
   rh.mark_continue_statement(n);
  }

 }

 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& token, RZ_Opaque_Type_Symbol& ots)
 {
  caon_ptr<RZ_ASG_Token> ft = rh.function_token();

  if(ft)
  {
   caon_ptr<tNode> n = rh.valuer().register_lexical_symbol(*ft, token, ots,
     rh.valuer().current_logical_scope());
   rh.mark_continue_statement(n);
  }
 }

 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& token,
  RZ_ASG_Symbol& sym)
 {
  caon_ptr<RZ_ASG_Token> ft = rh.function_token();
  if(ft)
  {
   caon_ptr<tNode> n = rh.valuer().register_lexical_symbol(rh, *ft, token, sym,
     rh.valuer().current_logical_scope());
   rh.mark_continue_statement(n);
  }
 }

 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& token,
  RZ_ASG_Vector& symvec)
 {
  caon_ptr<RZ_ASG_Token> ft = rh.function_token();
  if(ft)
  {
   caon_ptr<tNode> n = rh.valuer().register_lexical_symbols(rh, *ft, token, symvec,
     rh.valuer().current_logical_scope());
   rh.mark_continue_statement(n);
  }
 }
};



_RZNS(GRun)



#endif //GRAPH_CALL_TC__H
