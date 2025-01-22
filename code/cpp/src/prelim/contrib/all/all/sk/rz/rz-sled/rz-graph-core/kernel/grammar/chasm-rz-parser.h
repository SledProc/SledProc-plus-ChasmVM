
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef CHASM_RZ_PARSER__H
#define CHASM_RZ_PARSER__H

#include "relae-graph/relae-parser.h"

#include "kernel/chasm-rz-dominion.h"

#include "rzns.h"
RZNS_(RZ_Core)

class ChasmRZ_Graph;

class ChasmRZ_Parser : public Relae_Parser<ChasmRZ_Galaxy>
{
 QString raw_text_;

public:

 ACCESSORS(QString ,raw_text)

 ChasmRZ_Parser(caon_ptr<ChasmRZ_Graph> g);

};

_RZNS(RZ_Core)

#endif //CHASM_RZ_PARSER__H
