
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef CHASM_RZ_GRAPH__H
#define CHASM_RZ_GRAPH__H

#include "relae-graph/relae-node-ptr.h"

#include "rz-graph-core/kernel/chasm-rz-dominion.h"

#include <QTextStream>

#include "rzns.h"

RZNS_(RZ_Core)


class ChasmRZ_Graph : public node_graph<ChasmRZ_Dominion>
{
 public:
  ChasmRZ_Graph(caon_ptr<ChasmRZ_Node> root_node = nullptr);

 void report(QTextStream& qts);
 void report_from_node(QTextStream& qts,
  const ChasmRZ_Node& node, int indent = 0);

};

_RZNS(RZ_Core)


#endif //CHASM_RZ_GRAPH__H
