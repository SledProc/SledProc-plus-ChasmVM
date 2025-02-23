
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rz-prerun-check.h"


#include "rz-graph-core/kernel/graph/chasm-rz-graph.h"

#include "textio.h"

USING_RZNS(RZ_Core)
USING_RZNS(GBuild)

USING_KANS(TextIO)


RZ_Prerun_Check::RZ_Prerun_Check(caon_ptr<ChasmRZ_Document> document)
 : ChasmRZ_ASG_Output(document)
{

}


void RZ_Prerun_Check::write_report(QTextStream& qts)
{
 caon_ptr<ChasmRZ_Node> node = get_root_node();
}



void RZ_Prerun_Check::report_token(QTextStream& qts,
 const ChasmRZ_Token& token)
{

}


void RZ_Prerun_Check::report_tuple_info_entry(QTextStream& qts,
 const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> rce_ptr)
{

}

void RZ_Prerun_Check::report_tuple_info_leave(QTextStream& qts,
 const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> rce_ptr)
{

}

void RZ_Prerun_Check::report_call_entry(QTextStream& qts,
 const ChasmRZ_Call_Entry& rce)
{

}

void RZ_Prerun_Check::report_call_leave(QTextStream& qts,
 const ChasmRZ_Call_Entry& rce)
{

}
