
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef CHASM_RZ_QUERY__H
#define CHASM_RZ_QUERY__H

#include "rz-graph-core/kernel/chasm-rz-dominion.h"

#include "relae-graph/relae-node-ptr.h"

#include "rzns.h"

RZNS_(RZ_Core)


class ChasmRZ_Query : public node_query<ChasmRZ_Dominion>
{
 ChasmRZ_Query();

 public:
  #define DOMINION_CONNECTOR(name, label) \
   ChasmRZ_Connectors name;
  #include "rz-graph-core/kernel/dominion/connectors.h"
  #undef DOMINION_CONNECTOR

 static const ChasmRZ_Query& instance();
};

_RZNS(RZ_Core)

#endif //CHASM_RZ_QUERY__H
