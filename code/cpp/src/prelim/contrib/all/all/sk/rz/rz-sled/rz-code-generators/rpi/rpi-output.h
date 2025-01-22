
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RPI_OUTPUT__H
#define RPI_OUTPUT__H

#include "accessors.h"
#include "flags.h"

#include <QString>
#include <QMap>

#include <QTextStream>

#include "rzns.h"

#include "relae-graph/relae-caon-ptr.h"

#include "rz-function-def/rz-function-def-syntax.h"

#include "phr-graph-core/kernel/graph/pgb-ir-build.h"



RZNS_(GBuild)
 class RZ_Lisp_Graph_Visitor;
 class RZ_Graph_Visitor_Phaon;
_RZNS(GBuild)

USING_RZNS(GBuild)


RZNS_(PhrGraphCore)
 class PGB_IR_Build;
_RZNS(PhrGraphCore)

USING_RZNS(PhrGraphCore)

RZNS_(GVal)

class RPI_Block;



class RPI_Output
{
 QList<PGB_IR_Build::Text_With_Purpose> step_forms_;

 RZ_Graph_Visitor_Phaon& visitor_phaon_;
 RZ_Lisp_Graph_Visitor& visitor();

 caon_ptr<RPI_Block> top_level_block_;

 void init_function_def_syntax();

public:

 RPI_Output(RZ_Graph_Visitor_Phaon& visitor_phaon);

 ACCESSORS__RGET(QList<PGB_IR_Build::Text_With_Purpose> ,step_forms)

 void write(QTextStream* qts);

 void init_top_level_block(PGB_IR_Build& pgb);

 void build_phaon_graph(PGB_IR_Build& pgb);

};

_RZNS(GVal)

#endif //RPI_OUTPUT__H
