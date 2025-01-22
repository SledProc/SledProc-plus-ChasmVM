
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef CHASM_RZ_ASG_OUTPUT__H
#define CHASM_RZ_ASG_OUTPUT__H

#include "accessors.h"
#include "flags.h"

#include "relae-graph/relae-node-ptr.h"

#include "rz-graph-core/kernel/query/chasm-rz-query.h"
#include "rz-graph-core/kernel/frame/chasm-rz-frame.h"

#include <QString>
#include <QTextStream>
#include <QMap>

#include <functional>

#include "rzns.h"
RZNS_(RZ_Core)


class ChasmRZ_Document;
class ChasmRZ_Node;

class ChasmRZ_ASG_Output
{
protected:

 caon_ptr<ChasmRZ_Document> document_;

 const ChasmRZ_Frame& Cf;
 const ChasmRZ_Frame& Sf;
 const ChasmRZ_Query& Qy;

 virtual void output_from_node(QTextStream& qts,
  const ChasmRZ_Node& node, int indent = 0,
  caon_ptr<ChasmRZ_Call_Entry> rce_ptr = nullptr);

 virtual QString get_asg_out(const ChasmRZ_Token& token);

 virtual void report_call_entry(QTextStream& qts,
  const ChasmRZ_Call_Entry& rce) = 0;

 virtual void report_call_leave(QTextStream& qts,
  const ChasmRZ_Call_Entry& rce) = 0;

 virtual void report_tuple_info_entry(QTextStream& qts,
  const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> = nullptr) = 0;

 virtual void report_tuple_info_leave(QTextStream& qts,
  const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> = nullptr) = 0;

 virtual void report_token(QTextStream& qts,
  const ChasmRZ_Token& token) = 0;

 virtual void init_token(ChasmRZ_Token& token) {}

public:

 ChasmRZ_ASG_Output(caon_ptr<ChasmRZ_Document> document);

 virtual void output(QString path);
 virtual void write_report(QTextStream& qts);

};

_RZNS(RZ_Core)

#endif //CHASM_RZ_ASG_OUTPUT__H
