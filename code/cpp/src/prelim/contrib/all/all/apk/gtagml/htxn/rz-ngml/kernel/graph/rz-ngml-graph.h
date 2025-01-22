
#ifndef RZ_NGML_GRAPH__H
#define RZ_NGML_GRAPH__H

#include "relae-graph/relae-node-ptr.h"

#include "kernel/rz-ngml-dominion.h"

#include "rzns.h"

RZNS_(NGML)


class NGML_Graph : public node_graph<NGML_Dominion>
{
 public:
  NGML_Graph(caon_ptr<NGML_Node> root_node = caon_ptr<NGML_Node>(nullptr) );

};

_RZNS(NGML)


#endif
