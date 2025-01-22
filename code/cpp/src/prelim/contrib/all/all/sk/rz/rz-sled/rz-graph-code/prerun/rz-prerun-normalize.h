
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_PRERUN_NORMALIZE__H
#define RZ_PRERUN_NORMALIZE__H

#include "accessors.h"
#include "flags.h"

#include "relae-graph/relae-node-ptr.h"

#include "rz-graph-core/output/chasm-rz-asg-output.h"

#include "rz-graph-core/kernel/graph/chasm-rz-graph.h"

#include <QString>
#include <QTextStream>
#include <QMap>

#include <functional>

#include "rzns.h"

RZNS_(GBuild)

class RZ_ASG_Visitor;

_RZNS(GBuild)

USING_RZNS(GBuild)

RZNS_(RZ_Core)


class ChasmRZ_Document;
class ChasmRZ_Node;
class ChasmRZ_Graph;


class RZ_Prerun_Normalize
{
 ChasmRZ_Graph& graph_;

public:

 RZ_Prerun_Normalize(ChasmRZ_Graph& graph);

 caon_ptr<RZ_ASG_Visitor> scan();
 void scan(RZ_ASG_Visitor& visitor);

};

_RZNS(RZ_Core)

#endif //RZ_PRERUN_NORMALIZE__H
