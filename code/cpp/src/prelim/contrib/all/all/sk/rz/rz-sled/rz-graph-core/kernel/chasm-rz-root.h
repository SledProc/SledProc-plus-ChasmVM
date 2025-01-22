
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef CHASM_RZ_ROOT__H
#define CHASM_RZ_ROOT__H


#include "relae-graph/relae-node-ptr.h"

#include "chasm-rz-dominion.h"

#include "rzns.h"

RZNS_(RZ_Core)

class ChasmRZ_Document;

class ChasmRZ_Root
{
 ChasmRZ_Document* document_;

public:

 ChasmRZ_Root(ChasmRZ_Document* document);

 QString document_path();
};

_RZNS(RZ_Core)


#endif //CHASM_RZ_ROOT__H
