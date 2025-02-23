
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rz-prerun-setup.h"


#include "rz-graph-core/kernel/graph/chasm-rz-graph.h"

#include "rz-graph-core/kernel/document/chasm-rz-document.h"

#include "rz-graph-core/kernel/chasm-rz-root.h"
#include "rz-graph-core/kernel/graph/chasm-rz-node.h"

#include "textio.h"

USING_RZNS(RZ_Core)
USING_RZNS(GBuild)

USING_KANS(TextIO)


RZ_Prerun_Setup::RZ_Prerun_Setup(caon_ptr<ChasmRZ_Document> document)
 : ChasmRZ_ASG_Output(document), traverser_(*document->graph())
{

}


void RZ_Prerun_Setup::write_report(QTextStream& qts)
{
 caon_ptr<ChasmRZ_Node> node = get_root_node();

 if(!node)
 {
  qts << "No root node found; perhaps empty source file\n\n";
  return;
 }

 CAON_PTR_DEBUG(ChasmRZ_Node ,node)

 caon_ptr<ChasmRZ_Root> root = node->chasm_rz_root();

 CAON_PTR_DEBUG(ChasmRZ_Root ,root)

 if(root)
 {
  qts << "Root local path: " << root->document_path() << "\n\n";
 }

 node = traverser_.find_root_entry(node);
 {
  CAON_PTR_DEBUG(ChasmRZ_Node ,node)
  CAON_DEBUG_NOOP
 }

 if(!node)
 {
  qts << "Source file appears to have no valid content\n\n";
  return;
 }


}



void RZ_Prerun_Setup::report_token(QTextStream& qts,
 const ChasmRZ_Token& token)
{

}


void RZ_Prerun_Setup::report_tuple_info_entry(QTextStream& qts,
 const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> rce_ptr)
{

}

void RZ_Prerun_Setup::report_tuple_info_leave(QTextStream& qts,
 const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> rce_ptr)
{

}

void RZ_Prerun_Setup::report_call_entry(QTextStream& qts,
 const ChasmRZ_Call_Entry& rce)
{

}

void RZ_Prerun_Setup::report_call_leave(QTextStream& qts,
 const ChasmRZ_Call_Entry& rce)
{

}
