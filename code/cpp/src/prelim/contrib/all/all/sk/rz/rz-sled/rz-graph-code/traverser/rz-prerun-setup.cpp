
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rz-prerun-setup.h"


#include "rz-graph-core/kernel/graph/chasm-rz-graph.h"

#include "rz-graph-core/kernel/document/chasm-rz-document.h"

#include "rz-graph-core/kernel/chasm-rz-root.h"
#include "rz-graph-core/kernel/graph/chasm-rz-node.h"

#include "rz-graph-core/code/chasm-rz-block-entry.h"
#include "rz-graph-core/code/chasm-rz-call-entry.h"

#include "rz-graph-core/token/rz-compiler-function.h"


#include "textio.h"

USING_RZNS(RZ_Core)
USING_RZNS(GBuild)

USING_KANS(TextIO)


RZ_Prerun_Setup::RZ_Prerun_Setup(caon_ptr<ChasmRZ_Document> document)
 : ChasmRZ_ASG_Output(document),
   node_factory_(ChasmRZ_Node_Factory::instance()),
   traverser_(*document->graph())
{

}


void RZ_Prerun_Setup::write_report(QTextStream& qts)
{
 caon_ptr<ChasmRZ_Node> root_node = get_root_node();

 if(!root_node)
 {
  qts << "No root node found; perhaps empty source file\n\n";
  return;
 }

 CAON_PTR_DEBUG(ChasmRZ_Node ,root_node)

 caon_ptr<ChasmRZ_Root> root = root_node->chasm_rz_root();

 CAON_PTR_DEBUG(ChasmRZ_Root ,root)

 if(root)
 {
  qts << "Root local path: " << root->document_path() << "\n\n";
 }

 caon_ptr<ChasmRZ_Node> next_node = traverser_.find_root_entry(root_node);
 CAON_PTR_DEBUG(ChasmRZ_Node ,next_node)
 CAON_DEBUG_NOOP

 if(!next_node)
 {
  qts << "Source file appears to have no valid content\n\n";
  return;
 }

 caon_ptr<ChasmRZ_Block_Entry> rbe = new ChasmRZ_Block_Entry(node_factory_.make_block_entry_id());
 caon_ptr<ChasmRZ_Node> rbe_node = node_factory_.make_new_node(rbe);

 caon_ptr<ChasmRZ_Call_Entry> rce = new ChasmRZ_Call_Entry(node_factory_.make_call_entry_id());
 caon_ptr<ChasmRZ_Node> rce_node = node_factory_.make_new_node(rce);

 caon_ptr<RZ_Compiler_Function> rcf = new RZ_Compiler_Function("lex-decl");
 caon_ptr<ChasmRZ_Node> rcf_node = node_factory_.make_new_node(rcf);

 root_node <<Tf/Qy.Run_Block_Entry>> rbe_node;
 rbe_node <<Tf/Qy.Run_Call_Entry>> rce_node;
 rce_node <<Tf/Qy.Run_Call_Entry>> rcf_node;
 rcf_node <<Tf/Qy.Run_Call_Sequence>> next_node;


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
