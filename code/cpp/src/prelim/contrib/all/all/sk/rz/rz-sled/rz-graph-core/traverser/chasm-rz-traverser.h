
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef CHASM_RZ_TRAVERSER__H
#define CHASM_RZ_TRAVERSER__H

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


RZNS_(RZ_Core)


class ChasmRZ_Document;
class ChasmRZ_Node;
class ChasmRZ_Graph;


class ChasmRZ_Traverser
{
 ChasmRZ_Graph& graph_;

 ChasmRZ_Frame& Cf;
 ChasmRZ_Frame& Sf;
 ChasmRZ_Frame& Tf;
 const ChasmRZ_Query& Qy;


public:

 ChasmRZ_Traverser(ChasmRZ_Graph& graph);

 caon_ptr<ChasmRZ_Node> find_root_entry(caon_ptr<ChasmRZ_Node> root_node);

 void scan();

 // std::function<void(RZ_Dynamo_Output&)> fn
};

_RZNS(RZ_Core)

#endif //CHASM_RZ_TRAVERSER__H
