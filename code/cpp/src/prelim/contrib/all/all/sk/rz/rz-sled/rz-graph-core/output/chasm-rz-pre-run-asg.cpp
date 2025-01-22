
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "chasm-rz-pre-run-asg.h"

#include "rzns.h"

USING_RZNS(RZ_Core)

ChasmRZ_Pchasm_rz_Run_ASG::ChasmRZ_Pchasm_rz_Run_ASG(caon_ptr<ChasmRZ_Document> document)
 : ChasmRZ_ASG_Output(document)
{

}


void ChasmRZ_Pchasm_rz_Run_ASG::output_from_node(QTextStream& qts,
 const ChasmRZ_Node& node, int indent)
{
 qts << "OK";
}


void ChasmRZ_Pchasm_rz_Run_ASG::report_call_entry(QTextStream& qts,
 const ChasmRZ_Call_Entry& rce)
{
}

void ChasmRZ_Pchasm_rz_Run_ASG::report_call_leave(QTextStream& qts,
 const ChasmRZ_Call_Entry& rce)
{
}
