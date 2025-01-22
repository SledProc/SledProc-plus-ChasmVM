
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_GRAPH_CORE_RUNNER__H
#define RZ_GRAPH_CORE_RUNNER__H


#include <QMap>

#include "rz-graph-token/rz-asg-value-holder.h"

#include "rz-graph-build/rz-asg-result-holder.h"


#include "functions/rz-asg-function-map.h"

#include "rz-asg-core-runner.h"

#include "rzns.h"


USING_RZNS(GBuild)


RZNS_(GRun)

struct RZ_ASG_Core_Runner
{
 template<RZ_ASG_Function_Family>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Function_Code function_code,
  RZ_ASG_Value_Holder& v1, RZ_ASG_Value_Holder& v2);

 template<RZ_ASG_Function_Family>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Function_Code function_code,
  RZ_ASG_Value_Holder& vh, RZ_ASG_Token& token);

 template<RZ_ASG_Function_Family>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Function_Code function_code,
  RZ_ASG_Token& token, RZ_ASG_Value_Holder& vh);


 template<RZ_ASG_Function_Family>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Function_Code function_code,
  RZ_ASG_Token& start_token, caon_ptr<tNode> pass_node);

 template<RZ_ASG_Function_Family>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Function_Code function_code,
  RZ_ASG_Token& start_token);

 template<RZ_ASG_Function_Family>
 static void run(RZ_ASG_Result_Holder& rh, RZ_ASG_Function_Code function_code,
  RZ_ASG_Token& lhs_token, RZ_ASG_Token& rhs_token);

};

_RZNS(GRun)


#endif //RZ_GRAPH_CORE_RUNNER__H
