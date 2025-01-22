
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef GRAPH_CALL_T_C__H
#define GRAPH_CALL_T_C__H

#include "token/rz-asg-token.h"


#include "rz-graph-valuer/valuer/rz-asg-valuer.h"

#include "functions/rz-asg-function-families.h"

#include "rzns.h"

typedef RZ::RZ_Core::ChasmRZ_Node tNode;

RZNS_(GRun)

#define RZ_ASG_FUNCTION_DECLARE(str, name, arity, status) name,


RZ_ASG_FUNCTION_CODES_(RZ_Graph_Call_TC)
 null = 0,
 #include "core-functions-t-c.h"
_RZ_ASG_FUNCTION_CODES



#undef RZ_ASG_FUNCTION_DECLARE

RZ_GCALL_IMPLEMENT <RZ_GCALL_TC(Precycle, Core_Class)>
{
 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& t1, T2& t2)
 {
  tString str = t1.string_value();
  caon_ptr<tNode> utyn = rh.valuer().register_user_precycle(str);
 //  rh.mark_continue_statement(t1.pRestore<tNode>(), utyn);
  rh.mark_continue_statement(t1.pRestore<tNode>(), utyn);
//  rh.hold(utyn);
  rh.valuer().define_pass_proxy(rh);

  //rh.mark_continue_statement(t1.pRestore<tNode>());
  //rh.hold(uty);
 }

 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, T1& t1, T2& t2)
 {

 };

};





_RZNS(GRun)

#endif //GRAPH_CALL_T_C__H
