
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef CHASM_RZ_FRAME__H
#define CHASM_RZ_FRAME__H

#include "chasm-rz-frame.h"

#include "relae-graph/relae-node-ptr.h"

#include "rz-graph-core/kernel/chasm-rz-dominion.h"

#include "rzns.h"

RZNS_(RZ_Core)


class ChasmRZ_Frame : public node_frame<ChasmRZ_Dominion>
{
 ChasmRZ_Frame(QString label = {});

 QString label_;

 public:

 static ChasmRZ_Frame& instance(QString label);

};

_RZNS(RZ_Core)

#endif //CHASM_RZ_FRAME__H
