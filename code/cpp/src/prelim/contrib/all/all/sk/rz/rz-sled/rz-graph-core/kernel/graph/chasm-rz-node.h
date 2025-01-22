
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef CHASM_RZ_NODE__H
#define CHASM_RZ_NODE__H

#include "relae-graph/relae-node-ptr.h"

#include "kernel/chasm-rz-dominion.h"

#include "kernel/frame/chasm-rz-frame.h"

#include "accessors.h"

#include <functional>

#include <QVector>

#include "rzns.h"


RZNS_(GBuild)

 class RZ_ASG_Token;

_RZNS(GBuild)


RZNS_(RZ_Core)

USING_RZNS(GBuild)

class ChasmRZ_Node : public node_ptr<ChasmRZ_Dominion>
{

 QVector<caon_ptr<ChasmRZ_Node>> hyponodes_;
 caon_ptr<ChasmRZ_Node> parent_;

public:

 #define DOMINION_TYPE DOMINION_NODE_CONSTRUCTOR
 #include "kernel/dominion/types.h"
 #undef DOMINION_TYPE

 ACCESSORS__RGET(QVector<caon_ptr<ChasmRZ_Node>> ,hyponodes)
 ACCESSORS(caon_ptr<ChasmRZ_Node> ,parent)

 caon_ptr<RZ_ASG_Token> asg_token();

 void debug_connections();

 void each_connection(std::function<void(const ChasmRZ_Connectors& connector,
  const ChasmRZ_Node&, const ChasmRZ_Connection* connection)> fn)  const;

 void swap_relation(const ChasmRZ_Connectors& connector,
  caon_ptr<ChasmRZ_Node> n1, caon_ptr<ChasmRZ_Node> n2);

 void delete_relation(const ChasmRZ_Connectors& connector,
  caon_ptr<ChasmRZ_Node> n1);

 void add_hyponode(caon_ptr<ChasmRZ_Node> n);


};

_RZNS(RZ_Core)


#endif //CHASM_RZ_NODE__H
