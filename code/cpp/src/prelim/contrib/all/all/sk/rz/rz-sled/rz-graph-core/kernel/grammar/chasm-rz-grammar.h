
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef CHASM_RZ_GRAMMAR__H
#define CHASM_RZ_GRAMMAR__H

#include "relae-graph/relae-grammar.h"

#include "kernel/chasm-rz-dominion.h"

#include "rzns.h"


RZNS_(RZ_Core)

class ChasmRZ_Graph;
class ChasmRZ_Graph_Build;
class ChasmRZ_Parser;

class ChasmRZ_Grammar : public Relae_Grammar<ChasmRZ_Graph, ChasmRZ_Parser>
{
public:

 ChasmRZ_Grammar();



 void init(ChasmRZ_Parser& p, ChasmRZ_Graph& g,
           ChasmRZ_Graph_Build& graph_build);


};

_RZNS(RZ_Core)

#endif //CHASM_RZ_GRAMMAR__H
