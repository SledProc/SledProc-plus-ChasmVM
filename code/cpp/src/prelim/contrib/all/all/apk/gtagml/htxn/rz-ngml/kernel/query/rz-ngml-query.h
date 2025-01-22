
#ifndef RZ_NGML_QUERY__H
#define RZ_NGML_QUERY__H

#include "kernel/rz-ngml-dominion.h"

#include "relae-graph/relae-node-ptr.h"

#include "rzns.h"

RZNS_(NGML)


class NGML_Query : public node_query<NGML_Dominion>
{
 NGML_Query();

 public:
  #define DOMINION_CONNECTOR(name, label) \
   NGML_Connectors name;
  #include "kernel/dominion/connectors.h"
  #undef DOMINION_CONNECTOR

 static const NGML_Query& instance();
};

_RZNS(NGML)

#endif
