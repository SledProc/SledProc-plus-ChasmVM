
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef CHASM_RZ_PRE_NORMAL_ASG__H
#define CHASM_RZ_PRE_NORMAL_ASG__H

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

class ChasmRZ_Pre_Normal_ASG : public ChasmRZ_ASG_Output
{
public:

#ifdef NO_CAON
 ChasmRZ_Pre_Normal_ASG(ChasmRZ_Document* document);
#else
 ChasmRZ_Pre_Normal_ASG(caon_ptr<ChasmRZ_Document> document);
#endif

 virtual void report_token(QTextStream& qts,
  const ChasmRZ_Token& token);

 virtual void report_tuple_info_entry(QTextStream& qts,
  const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> rce_ptr = nullptr);

 virtual void report_tuple_info_leave(QTextStream& qts,
  const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> rce_ptr = nullptr);

 virtual void report_call_entry(QTextStream& qts,
  const ChasmRZ_Call_Entry& rce);

 virtual void report_call_leave(QTextStream& qts,
  const ChasmRZ_Call_Entry& rce);


};

_RZNS(RZ_Core)

#endif //CHASM_RZ_PRE_NORMAL_ASG__H
