
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_PRERUN_TOKENS__H
#define RZ_PRERUN_TOKENS__H

#include "accessors.h"
#include "flags.h"

#include "relae-graph/relae-node-ptr.h"

#include "rz-graph-core/output/chasm-rz-asg-output.h"

#include <QString>
#include <QTextStream>
#include <QMap>

#include <functional>

#include "rzns.h"
RZNS_(RZ_Core)


class ChasmRZ_Document;
class ChasmRZ_Node;


class RZ_Prerun_Tokens : public ChasmRZ_ASG_Output
{

public:

 RZ_Prerun_Tokens(caon_ptr<ChasmRZ_Document> document);

 virtual void init_token(ChasmRZ_Token& token) Q_DECL_OVERRIDE;

 virtual void init_token(const ChasmRZ_Node& node, caon_ptr<ChasmRZ_Node>& rat_node) Q_DECL_OVERRIDE;


 virtual void report_token(QTextStream& qts,
  const ChasmRZ_Token& token) Q_DECL_OVERRIDE;

 virtual void report_tuple_info_entry(QTextStream& qts,
  const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> rce_ptr = nullptr) Q_DECL_OVERRIDE;

 virtual void report_tuple_info_leave(QTextStream& qts,
  const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> rce_ptr = nullptr) Q_DECL_OVERRIDE;

 virtual void report_call_entry(QTextStream& qts,
  const ChasmRZ_Call_Entry& rce) Q_DECL_OVERRIDE;

 virtual void report_call_leave(QTextStream& qts,
  const ChasmRZ_Call_Entry& rce) Q_DECL_OVERRIDE;


};

_RZNS(RZ_Core)

#endif //RZ_PRERUN_TOKENS__H
