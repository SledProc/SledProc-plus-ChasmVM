
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef GRAPH_CALL_T__H
#define GRAPH_CALL_T__H

#include "token/rz-asg-token.h"
#include "rz-graph-valuer/valuer/rz-asg-valuer.h"

#include "functions/rz-asg-function-families.h"

#include "rzns.h"

RZNS_(GRun)

#define RZ_ASG_FUNCTION_DECLARE(str, name, arity, status) name,

RZ_ASG_FUNCTION_CODES_(RZ_Graph_Call_T)
 null = 0,
 #include "core-functions-t.h"
_RZ_ASG_FUNCTION_CODES

#undef RZ_ASG_FUNCTION_DECLARE




RZ_GCALL_IMPLEMENT <RZ_GCALL_T(Lex_Decl, Internal)>
{

 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& t1)
 {
  caon_ptr<ChasmRZ_Node> fnode = rh.function_node();
  CAON_PTR_DEBUG(ChasmRZ_Node ,fnode)

  caon_ptr<RZ_Type_Object> rto = t1.type_object();
  CAON_PTR_DEBUG(RZ_Type_Object ,rto)
  CAON_DEBUG_NOOP

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



_RZNS(GRun)


#endif //GRAPH_CALL_T__H
