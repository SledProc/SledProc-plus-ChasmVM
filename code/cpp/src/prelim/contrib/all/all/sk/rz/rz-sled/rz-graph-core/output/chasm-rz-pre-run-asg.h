
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef CHASM_RZ_PChasmRZ_RUN_ASG__H
#define CHASM_RZ_PChasmRZ_RUN_ASG__H

#include "accessors.h"
#include "flags.h"

#include "relae-graph/relae-node-ptr.h"

#include "chasm-rz-asg-output.h"

#include <QString>
#include <QTextStream>
#include <QMap>

#include <functional>

#include "rzns.h"
RZNS_(RZ_Core)


class ChasmRZ_Document;
class ChasmRZ_Node;

class ChasmRZ_Pchasm_rz_Run_ASG : public ChasmRZ_ASG_Output
{
public:

 ChasmRZ_Pchasm_rz_Run_ASG(caon_ptr<ChasmRZ_Document> document);

 virtual void output_from_node(QTextStream& qts,
  const ChasmRZ_Node& node, int indent = 0);

 virtual void report_call_entry(QTextStream& qts,
  const ChasmRZ_Call_Entry& rce) override;

 virtual void report_call_leave(QTextStream& qts,
  const ChasmRZ_Call_Entry& rce) override;



};

_RZNS(RZ_Core)

#endif //CHASM_RZ_PChasmRZ_RUN_ASG__H
