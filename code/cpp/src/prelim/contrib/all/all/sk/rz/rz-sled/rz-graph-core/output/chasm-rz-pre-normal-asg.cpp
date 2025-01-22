
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "chasm-rz-pre-normal-asg.h"

#include "kernel/graph/chasm-rz-graph.h"
#include "kernel/graph/chasm-rz-node.h"

#include "kernel/chasm-rz-root.h"

#include "token/chasm-rz-token.h"

#include "tuple/chasm-rz-tuple-info.h"
#include "code/chasm-rz-call-entry.h"

#include "rzns.h"

USING_RZNS(RZ_Core)


#ifdef NO_CAON
 ChasmRZ_Pre_Normal_ASG::ChasmRZ_Pre_Normal_ASG(ChasmRZ_Document* document)
#else
 ChasmRZ_Pre_Normal_ASG::ChasmRZ_Pre_Normal_ASG(caon_ptr<ChasmRZ_Document> document)
#endif
 : ChasmRZ_ASG_Output(document)
{

}


void ChasmRZ_Pre_Normal_ASG::report_token(QTextStream& qts,
 const ChasmRZ_Token& token)
{
 if(token.flags.is_symbol_declaration)
 {
  qts << "(rz-decl " << token.raw_text() << ")";
 }
 else
  qts << token.get_asg_out();
}

void ChasmRZ_Pre_Normal_ASG::report_tuple_info_entry(QTextStream& qts,
 const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> rce_ptr)
{
 qts << '(';
 if(rce_ptr)
  qts << QString("<%1>").arg(rce_ptr->call_id());
 qts << rti.asg_out_with_id() << ' ';
}

void ChasmRZ_Pre_Normal_ASG::report_call_entry(QTextStream& qts,
 const ChasmRZ_Call_Entry& rce)
{
 qts << QString("<%1>").arg(rce.call_id());
}

void ChasmRZ_Pre_Normal_ASG::report_call_leave(QTextStream& qts,
 const ChasmRZ_Call_Entry& rce)
{
 qts << QString("</%1>").arg(rce.call_id());
}

void ChasmRZ_Pre_Normal_ASG::report_tuple_info_leave(QTextStream& qts,
 const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> rce_ptr)
{
 qts << ')';
}

