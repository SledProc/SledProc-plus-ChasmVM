
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef RZ_NGML_GRAMMAR__H
#define RZ_NGML_GRAMMAR__H

#include "relae-graph/relae-grammar.h"

#include "rzns.h"

RZNS_(NGML)

class NGML_Graph;
class NGML_Parser;
class NGML_Graph_Build;

class NGML_Grammar : public Relae_Grammar<NGML_Graph, NGML_Parser>
{
public:

 NGML_Grammar();

 void init(NGML_Parser& p, NGML_Graph& g,
           NGML_Graph_Build& graph_build);


};

_RZNS(NGML)

#endif // RZ_NGML_GRAMMAR__H
