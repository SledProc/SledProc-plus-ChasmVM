
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef GRAPH_CALL_C_V__H
#define GRAPH_CALL_C_V__H

#include "token/rz-asg-token.h"

#include "rz-graph-valuer/valuer/rz-asg-valuer.h"


#include "functions/rz-asg-function-families.h"

#include "rzns.h"

typedef RZ::RZ_Core::ChasmRZ_Node tNode;

RZNS_(GRun)

#define RZ_ASG_FUNCTION_DECLARE(str, name, arity, status) name,

RZ_ASG_FUNCTION_CODES_(RZ_Graph_Call_CV)
 null = 0,
 #include "core-functions-c-v.h"
_RZ_ASG_FUNCTION_CODES

#undef RZ_ASG_FUNCTION_DECLARE


//#ifdef HIDE

RZ_GCALL_IMPLEMENT
<RZ_GCALL_CV(Init_or_Assign, Core_Class)>
{
 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, RZ_Assignment_Point& rap, RZ_ASG_Value_Holder& v2)
 {
  //tString str = t1.string_value();
  caon_ptr<RZ_ASG_Token> ft = rh.get_lead_function_token();
  if(ft)
  {
   CAON_PTR_DEBUG(RZ_ASG_Token ,ft)

//   caon_ptr<RZ_Type_Object> rto1 = v1.type_object();
//   CAON_PTR_DEBUG(RZ_Type_Object ,rto1)

   caon_ptr<RZ_Type_Object> rto2 = v2.type_object();
   CAON_PTR_DEBUG(RZ_Type_Object ,rto2)

   CAON_DEBUG_NOOP
//   rh.valuer().set_equal(rh, *ft, v1, v2);
  }
 }


 template<typename T1, typename T2>
 static void run(RZ_ASG_Result_Holder& rh, T1& t1,
                 T2& t2)
 {

 }
};


//#endif //HIDE

_RZNS(GRun)



#endif //GRAPH_CALL_C_V__H
