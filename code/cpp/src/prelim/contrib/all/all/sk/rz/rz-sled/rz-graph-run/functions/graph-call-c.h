
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef GRAPH_CALL_C__H
#define GRAPH_CALL_C__H

#include "rz-graph-valuer/valuer/rz-asg-valuer.h"

#include "token/rz-asg-token.h"

#include "rz-graph-valuer/string/rz-string.h"
#include "rz-graph-valuer/string/rz-keyword.h"


#include "rz-graph-token/rz-asg-core-casement-function.h"


#include "functions/rz-asg-function-families.h"

#include "types/type-families.h"

#include "rzns.h"


RZNS_(GBuild)

class RZ_Lisp_Empty_Tuple;
class RZ_Lisp_Core_Function;
class RZ_Null_Value;

_RZNS(GBuild)

RZNS_(GVal)

class RZ_Phaon_User_Type;

_RZNS(GVal)

USING_RZNS(GBuild)

USING_RZNS(GVal)



RZNS_(GRun)


#define RZ_ASG_FUNCTION_DECLARE(str, name, arity, status) name,

RZ_ASG_FUNCTION_CODES_(RZ_Graph_Call_C)
 null = 0,
 #include "core-functions-c.h"
_RZ_ASG_FUNCTION_CODES


RZ_GCALL_IMPLEMENT <RZ_GCALL_C(Do, Core_Class)>
{
 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, T1& t1, T2& t2)
 {
 }
// //   It would be better to separate this into the RZ_Function_Def_Info and do-map cases...
 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_Function_Def_Info& rfdi, T2& t2)
 {
  rh.valuer().init_do_block(rh, rfdi);
 }

};


RZ_GCALL_IMPLEMENT <RZ_GCALL_C(If, Core_Class)>
{
 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, T1& t1, RZ_ASG_Value_Holder& t2)
 {
  //rh.valuer().init_if_block(rh, t1);
 }

 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Core_Casement_Function& t1, RZ_ASG_Value_Holder& t2)
 {
  rh.valuer().init_if_block(rh, t1);
 }

 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_Opaque_Type_Symbol& ots, RZ_ASG_Value_Holder& t2)
 {
  rh.valuer().init_if_block(rh, ots);
 }

 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_Opaque_Call& t1, RZ_ASG_Value_Holder& t2)
 {
  rh.valuer().init_if_block(rh, t1);
 }
};


RZ_GCALL_IMPLEMENT <RZ_GCALL_C(Elsif, Core_Class)>
{
 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, T1& t1, RZ_ASG_Value_Holder& t2)
 {
  //rh.valuer().init_if_block(rh, t1);
 }

 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Core_Casement_Function& t1, RZ_ASG_Value_Holder& t2)
 {
  rh.valuer().init_elsif_block(rh, t1);
 }

 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_Opaque_Call& t1, RZ_ASG_Value_Holder& t2)
 {
  rh.valuer().init_elsif_block(rh, t1);
 }
};


#define RZ_GCALL_TEMP_DEFAULT_IMPLEMENTATION(count) \
RZ_GCALL_IMPLEMENT <RZ_GCALL_C(Enter_Logical_Scope_##count, Core_Class)> \
{ \
 template<typename T1, typename T2> \
 static void run(RZ_ASG_Result_Holder& rh, RZ_Phaon_User_Type& uty, RZ_ASG_Value_Holder& vh) \
 { \
  rh.valuer().enter_logical_scope(count, rh, uty); \
 } \
 template<typename T1, typename T2> \
 static void run(RZ_ASG_Result_Holder& rh, RZ_Opaque_Type_Symbol& ots, RZ_ASG_Value_Holder& vh) \
 { \
  rh.valuer().enter_logical_scope(count, rh, ots); \
 } \
 template<typename T1, typename T2> \
 static void run(RZ_ASG_Result_Holder& rh, T1& t1, T2& t2) \
 { \
 } \
}; \


RZ_GCALL_TEMP_DEFAULT_IMPLEMENTATION(3)
RZ_GCALL_TEMP_DEFAULT_IMPLEMENTATION(4)
RZ_GCALL_TEMP_DEFAULT_IMPLEMENTATION(5)
RZ_GCALL_TEMP_DEFAULT_IMPLEMENTATION(6)


#undef RZ_GCALL_TEMP_DEFAULT_IMPLEMENTATION

_RZNS(GRun)


#endif //GRAPH_CALL_C__H



#ifdef HIDE

RZ_GCALL_IMPLEMENT <RZ_GCALL_C(Enter_Logical_Scope_##count, Core_Class)> \
{ \
 template<typename T1, typename T2> \
 static void run(RZ_ASG_Result_Holder& rh, RZ_Lisp_User_Class& cl, RZ_ASG_Value_Holder& vh) \
 { \
  rh.valuer().enter_logical_scope(count, rh, cl); \
 } \
 template<typename T1, typename T2> \
 static void run(RZ_ASG_Result_Holder& rh, RZ_Lisp_User_Resource& ur, RZ_ASG_Value_Holder& vh) \
 { \
  rh.valuer().enter_logical_scope(count, rh, ur); \
 } \
 template<typename T1, typename T2> \
 static void run(RZ_ASG_Result_Holder& rh, RZ_Lisp_User_Package& upkg, RZ_ASG_Value_Holder& vh) \
 { \
  rh.valuer().enter_logical_scope(count, rh, upkg); \
 } \
 template<typename T1, typename T2> \
 static void run(RZ_ASG_Result_Holder& rh, RZ_Opaque_Type_Symbol& ots, RZ_ASG_Value_Holder& vh) \
 { \
  rh.valuer().enter_logical_scope(count, rh, ots); \
 } \
 template<typename T1, typename T2> \
 static void run(RZ_ASG_Result_Holder& rh, T1& t1, T2& t2) \
 { \
 } \
}; \


#endif // HIDE
