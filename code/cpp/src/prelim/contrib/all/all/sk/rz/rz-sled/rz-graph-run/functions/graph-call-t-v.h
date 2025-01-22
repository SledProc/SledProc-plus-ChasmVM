
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef GRAPH_CALL_T_V__H
#define GRAPH_CALL_T_V__H

#include "rz-graph-valuer/valuer/rz-asg-valuer.h"

#include "rzns.h"

RZNS_(GRun)

#define RZ_ASG_FUNCTION_DECLARE(str, name, arity, status) name,


RZ_ASG_FUNCTION_CODES_(RZ_Graph_Call_TV)
 null = 0,
 #include "core-functions-t-v.h"
_RZ_ASG_FUNCTION_CODES



#undef RZ_ASG_FUNCTION_DECLARE

_RZNS(GRun)


#endif //GRAPH_CALL_T_V__H
