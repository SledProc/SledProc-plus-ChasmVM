
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef CHASM_RZ_NODE_PROXY__H
#define CHASM_RZ_NODE_PROXY__H

#include "relae-graph/relae-node-ptr.h"

#include "kernel/chasm-rz-dominion.h"

#include "accessors.h"

#include <functional>

#include "rzns.h"

RZNS_(RZ_Core)

class ChasmRZ_Node;

class ChasmRZ_Node_Proxy
{
 caon_ptr<ChasmRZ_Node> node_;

public:

 ChasmRZ_Node_Proxy(caon_ptr<ChasmRZ_Node> node);
 ACCESSORS(caon_ptr<ChasmRZ_Node> ,node)

};

_RZNS(RZ_Core)


#endif //CHASM_RZ_NODE_PROXY__H
