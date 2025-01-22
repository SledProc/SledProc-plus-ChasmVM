
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef CHASM_RZ_CONNECTION__H
#define CHASM_RZ_CONNECTION__H

#include "relae-graph/relae-node-ptr.h"

#include "kernel/chasm-rz-dominion.h"

#include <QTextStream>

#include "rzns.h"
#include "accessors.h"

RZNS_(RZ_Core)

class Run_Call_Entry;
class ChasmRZ_Node;

class ChasmRZ_Connection
{
 caon_ptr<ChasmRZ_Node> chasm_rz_node_;

public:

 ACCESSORS(caon_ptr<ChasmRZ_Node> ,chasm_rz_node)

 ChasmRZ_Connection(caon_ptr<ChasmRZ_Node> chasm_rz_node);



};


_RZNS(RZ_Core)

#endif //CHASM_RZ_CONNECTION__H

