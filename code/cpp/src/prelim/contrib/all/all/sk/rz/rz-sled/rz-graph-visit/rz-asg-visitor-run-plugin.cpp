
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "rz-asg-visitor-run-plugin.h"
#include "rz-asg-visitor.h"

#include "rzns.h"

USING_RZNS(GBuild)


RZ_ASG_Visitor_Run_Plugin::RZ_ASG_Visitor_Run_Plugin(RZ_ASG_Visitor& main_visitor)
 : main_visitor_(main_visitor), run_state_(main_visitor.run_state())
{

}




