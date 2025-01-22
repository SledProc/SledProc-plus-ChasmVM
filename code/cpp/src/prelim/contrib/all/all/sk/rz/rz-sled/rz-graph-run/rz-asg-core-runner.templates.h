
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_LISP_GRAPH_CORE_RUNNER__TEMPLATES__H
#define RZ_LISP_GRAPH_CORE_RUNNER__TEMPLATES__H

#include "rz-asg-core-runner.h"

#include "functions/rz-asg-run-router.h"

#include "rz-graph-core/kernel/chasm-rz-dominion.h"

USING_RZNS(GBuild)

RZNS_(GRun)

#define RZ_TEMP_CASE(num) \
 case num: \
  Cast_Schedule< \
    RZ_Graph_Call_S, \
    num, \
    RZ_Type_Families::Internal, \
    No_Cast_Needed, No_Cast_Needed>:: \
    Runner<Cast_Null_Marker, Raw_Token_Marker, \
     RZ_Type_Families::Internal>::run(rh, token, pass_node); break;

template<>
void RZ_ASG_Core_Runner::run<RZ_Graph_Call_S>(RZ_ASG_Result_Holder& rh,
  RZ_ASG_Function_Code function_code,
 RZ_ASG_Token& token, caon_ptr<tNode> pass_node)
{
 switch(function_code)
 {
  RZ_TEMP_CASES__FUNCTION_CODE
 }
}
#undef RZ_TEMP_CASE


#define RZ_TEMP_CASE(num) \
 case num: \
  Cast_Schedule< \
    RZ_Graph_Call_T, \
    num, \
    RZ_Type_Families::Internal, \
    No_Cast_Needed, No_Cast_Needed>:: \
    Runner<Cast_Null_Marker, Raw_Token_Marker, RZ_Type_Families::Internal>::run(rh, token); break;

template<>
void RZ_ASG_Core_Runner::run<RZ_Graph_Call_T>(RZ_ASG_Result_Holder& rh, RZ_ASG_Function_Code function_code,
 RZ_ASG_Token& token)
{
 switch(function_code)
 {
  RZ_TEMP_CASES__FUNCTION_CODE
 }
}
#undef RZ_TEMP_CASE


#define RZ_TEMP_CASE(num) \
 case num: \
  Cast_Schedule< \
    RZ_Graph_Call_Tc, \
    num, \
    RZ_Type_Families::Internal, \
    No_Cast_Needed, Cast_Needed>:: \
    Runner<Cast_Null_Marker, RHS_Cast_Marker, \
    RZ_Type_Families::Internal>::run(rh, \
     caon_ptr<RZ_ASG_Token>(&token), caon_ptr<RZ_ASG_Value_Holder>(&vh)); break; \

template<>
void RZ_ASG_Core_Runner::run<RZ_Graph_Call_Tc>(RZ_ASG_Result_Holder& rh,
 RZ_ASG_Function_Code function_code,
 RZ_ASG_Token& token, RZ_ASG_Value_Holder& vh)
{
 caon_ptr<RZ_ASG_Value_Holder> pvh = &vh;
 switch(function_code)
 {
  RZ_TEMP_CASES__FUNCTION_CODE
 }
}
#undef RZ_TEMP_CASE



#define RZ_TEMP_CASE(num) \
 case num: \
  Cast_Schedule< \
    RZ_Graph_Call_Tv, \
    num, \
    RZ_Type_Families::Internal, \
    No_Cast_Needed, No_Cast_Needed>:: \
    Runner<Cast_Null_Marker, RHS_Value_Marker, \
    RZ_Type_Families::Internal>::run(rh, \
     token, vh); break; \


template<>
void RZ_ASG_Core_Runner::run<RZ_Graph_Call_Tv>(RZ_ASG_Result_Holder& rh,
 RZ_ASG_Function_Code function_code,
 RZ_ASG_Token& token, RZ_ASG_Value_Holder& vh)
{
 switch(function_code)
 {
  RZ_TEMP_CASES__FUNCTION_CODE
 }
}
#undef RZ_TEMP_CASE




#define RZ_TEMP_CASE(num) \
 case num: \
  Cast_Schedule< \
    RZ_Graph_Call_TV, \
    num, \
    RZ_Type_Families::Internal, \
    No_Cast_Needed, No_Cast_Needed>:: \
    Runner<Cast_Null_Marker, RHS_Value_Marker, RZ_Type_Families::Internal>::run(rh, token, vh); break;

template<>
void RZ_ASG_Core_Runner::run<RZ_Graph_Call_TV>(RZ_ASG_Result_Holder& rh,
 RZ_ASG_Function_Code function_code,
 RZ_ASG_Token& token, RZ_ASG_Value_Holder& vh)
{
 switch(function_code)
 {
  RZ_TEMP_CASES__FUNCTION_CODE
 }
}
#undef RZ_TEMP_CASE





#define RZ_TEMP_CASE(num) \
 case num: \
  Cast_Schedule< \
    RZ_Graph_Call_VV, \
    num, \
    RZ_Type_Families::Internal, \
    No_Cast_Needed, No_Cast_Needed>:: \
    Runner<LHS_Value_Marker, RHS_Value_Marker, RZ_Type_Families::Internal>::run(rh, v1, v2); break;

template<>
void RZ_ASG_Core_Runner::run<RZ_Graph_Call_VV>(RZ_ASG_Result_Holder& rh,
 RZ_ASG_Function_Code function_code,
 RZ_ASG_Value_Holder& v1, RZ_ASG_Value_Holder& v2)
{
 switch(function_code)
 {
  RZ_TEMP_CASES__FUNCTION_CODE
 }
}
#undef RZ_TEMP_CASE






#define RZ_TEMP_CASE(num) \
 case num: \
  Cast_Schedule< \
    RZ_Graph_Call_TC, \
    num, \
    RZ_Type_Families::Internal, \
    No_Cast_Needed, Cast_Needed>:: \
    Runner<Cast_Null_Marker, RHS_Cast_Marker, \
     RZ_Type_Families::Internal>::run(rh, \
     caon_ptr<RZ_ASG_Token>(&lhs_token), \
     caon_ptr<RZ_ASG_Value_Holder>(&rhs_value)); break;

template<>
void RZ_ASG_Core_Runner::run<RZ_Graph_Call_TC>(RZ_ASG_Result_Holder& rh,
 RZ_ASG_Function_Code function_code,
 RZ_ASG_Token& lhs_token, RZ_ASG_Value_Holder& rhs_value)
{
 switch(function_code)
 {
  RZ_TEMP_CASES__FUNCTION_CODE
 }
}
#undef RZ_TEMP_CASE




#define RZ_TEMP_CASE(num) \
case num: \
 Cast_Schedule< \
   RZ_Graph_Call_C, \
   num, \
   RZ_Type_Families::Internal, \
   Cast_Needed, No_Cast_Needed>:: \
   Runner<Cast_Null_Marker, LHS_Cast_Marker, \
    RZ_Type_Families::Internal>::run(rh, \
     caon_ptr<RZ_ASG_Value_Holder>(&v1), \
     caon_ptr<RZ_ASG_Value_Holder>(&v2)); break;

template<>
void RZ_ASG_Core_Runner::run<RZ_Graph_Call_C>(RZ_ASG_Result_Holder& rh, RZ_ASG_Function_Code function_code,
 RZ_ASG_Value_Holder& v1, RZ_ASG_Value_Holder& v2)
{
 switch(function_code)
 {
  RZ_TEMP_CASES__FUNCTION_CODE
 }
}
#undef RZ_TEMP_CASE



#define RZ_TEMP_CASE(num) \
case num: \
 Cast_Schedule< \
   RZ_Graph_Call_CT, \
   num, \
   RZ_Type_Families::Internal, \
   Cast_Needed, No_Cast_Needed>:: \
   Runner<Cast_Null_Marker, LHS_Cast_Marker, \
    RZ_Type_Families::Internal>::run(rh, \
     caon_ptr<RZ_ASG_Value_Holder>(&lhs_value), \
     caon_ptr<RZ_ASG_Token>(&rhs_token)); break;

template<>
void RZ_ASG_Core_Runner::run<RZ_Graph_Call_CT>(RZ_ASG_Result_Holder& rh, RZ_ASG_Function_Code function_code,
 RZ_ASG_Value_Holder& lhs_value, RZ_ASG_Token& rhs_token)
{
 switch(function_code)
 {
  RZ_TEMP_CASES__FUNCTION_CODE
 }
}
#undef RZ_TEMP_CASE




#define RZ_TEMP_CASE(num) \
case num: \
 Cast_Schedule< \
   RZ_Graph_Call_CC, \
   num, \
   RZ_Type_Families::Internal, \
   Cast_Needed, Cast_Needed>:: \
   Runner<Cast_Null_Marker, LHS_Cast_Marker, \
    RZ_Type_Families::Internal>::run(rh, \
     caon_ptr<RZ_ASG_Value_Holder>(&v1), \
     caon_ptr<RZ_ASG_Value_Holder>(&v2)); break;

template<>
void RZ_ASG_Core_Runner::run<RZ_Graph_Call_CC>(RZ_ASG_Result_Holder& rh, RZ_ASG_Function_Code function_code,
 RZ_ASG_Value_Holder& v1, RZ_ASG_Value_Holder& v2)
{
 switch(function_code)
 {
  RZ_TEMP_CASES__FUNCTION_CODE
 }
}
#undef RZ_TEMP_CASE

_RZNS(GRun)


#endif //RZ_LISP_GRAPH_CORE_RUNNER__TEMPLATES__H

