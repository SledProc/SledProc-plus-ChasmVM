
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef GRAPH_CALL_S__H
#define GRAPH_CALL_S__H

#include "token/rz-asg-token.h"

#include "rz-graph-valuer/valuer/rz-asg-valuer.h"

#include "functions/rz-asg-function-families.h"

RZNS_(GRun)


typedef RZ::RZ_Core::ChasmRZ_Node tNode;

#define RZ_ASG_FUNCTION_DECLARE(str, name, arity, status) name,

#define RZ_TEMP_MACRO_(X) \
 template<> \
 struct RZ_ASG_Function_Family_<X> \
 { enum Code { \
  null = 0, \


#define _RZ_TEMP_MACRO }; };


RZ_ASG_FUNCTION_CODES_(RZ_Graph_Call_S)
 null = 0,
 #include "core-functions-s.h"
_RZ_ASG_FUNCTION_CODES

#undef RZ_ASG_FUNCTION_DECLARE

//#define RZ_GCALL_TEMP_DEFAULT_IMPLEMENTATION(count) \
// RZ_GCALL_IMPLEMENT <RZ_GCALL_S(Leave_Logical_Scope_##count, Internal)> \
// { \
//  static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& start_token, caon_ptr<tNode> pass_node) \
//  { \
//   start_token.redirect_paste("#_rz-class"); \
//  } \
// }; \

#define RZ_GCALL_TEMP_DEFAULT_IMPLEMENTATION(count) \
 RZ_GCALL_IMPLEMENT <RZ_GCALL_S(Leave_Logical_Scope_##count, Internal)> \
 { \
  static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& start_token, caon_ptr<tNode> pass_node) \
  { \
   rh.valuer().leave_logical_scope(count, rh); \
  } \
 }; \


RZ_GCALL_TEMP_DEFAULT_IMPLEMENTATION(3)
RZ_GCALL_TEMP_DEFAULT_IMPLEMENTATION(4)
RZ_GCALL_TEMP_DEFAULT_IMPLEMENTATION(5)



//RZ_GCALL_IMPLEMENT <RZ_GCALL_S(Leave_Logical_Scope_4, Internal)>
//{
// static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& start_token, caon_ptr<tNode> pass_node)
// {
//  //QString scope_kind =
//  rh.valuer().leave_logical_scope(4, rh);
////  scope_kind.prepend("#_rz-");
////  rh.function_token()->redirect_paste(scope_kind);
// }
//};



#undef RZ_GCALL_TEMP_DEFAULT_IMPLEMENTATION

#define RZ_GCALL_TEMP_DEFAULT_IMPLEMENTATION(fun_name, v_method) \
 RZ_GCALL_IMPLEMENT <RZ_GCALL_S(fun_name, Internal)> \
 { \
  static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& start_token, caon_ptr<tNode> pass_node) \
  { \
   rh.valuer->v_method(rh, start_token, pass_node); \
  } \
 }; \


#undef RZ_GCALL_TEMP_DEFAULT_IMPLEMENTATION

#define RZ_GCALL_TEMP_DEFAULT_IMPLEMENTATION(fun_name, v_method) \
 RZ_GCALL_IMPLEMENT <RZ_GCALL_S(fun_name, Internal)> \
 { \
  static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& start_token, caon_ptr<tNode> pass_node) \
  { \
   rh.valuer->v_method(start_token, pass_node); \
  } \
 }; \

#undef RZ_GCALL_TEMP_DEFAULT_IMPLEMENTATION

_RZNS(GRun)

#endif //GRAPH_CALL_S__H
