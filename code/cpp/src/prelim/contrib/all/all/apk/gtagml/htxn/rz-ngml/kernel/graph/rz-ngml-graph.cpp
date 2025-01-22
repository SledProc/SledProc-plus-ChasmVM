
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rz-ngml-graph.h"
#include "rz-ngml-node.h"

#include "rzns.h"

USING_RZNS(NGML)

NGML_Graph::NGML_Graph(caon_ptr<NGML_Node> root_node)
 : node_graph<NGML_Dominion>(root_node)
{

}
