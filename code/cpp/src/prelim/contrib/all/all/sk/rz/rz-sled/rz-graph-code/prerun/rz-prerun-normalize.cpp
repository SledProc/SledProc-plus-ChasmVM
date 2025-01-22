
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "rz-prerun-normalize.h"

#include "rz-graph-visit/rz-asg-visitor.h"

#include "rz-graph-core/kernel/graph/chasm-rz-graph.h"

USING_RZNS(RZ_Core)
USING_RZNS(GBuild)


RZ_Prerun_Normalize::RZ_Prerun_Normalize(ChasmRZ_Graph& graph)
 : graph_(graph)
{

}

caon_ptr<RZ_ASG_Visitor> RZ_Prerun_Normalize::scan()
{
 caon_ptr<RZ_ASG_Visitor> result = new RZ_ASG_Visitor(&graph_);
 scan(*result);
 return result;
}

void RZ_Prerun_Normalize::scan(RZ_ASG_Visitor& v)
{
 v.set_graph(&graph_);
 v.normalize();
 v.hyper_normalize();
}

