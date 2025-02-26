
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

#include "rz-graph-core/token/rz-observer-function.h"


#include "textio.h"

USING_RZNS(RZ_Core)
USING_RZNS(GBuild)

USING_KANS(TextIO)


RZ_Prerun_Setup::RZ_Prerun_Setup(caon_ptr<ChasmRZ_Document> document)
 : ChasmRZ_ASG_Output(document)
{

}

caon_ptr<ChasmRZ_Node> RZ_Prerun_Setup::get_root_node(QTextStream& qts)
{
 caon_ptr<ChasmRZ_Node> result = ChasmRZ_ASG_Output::get_root_node();

 if(!result)
 {
  qts << "No root node found; perhaps empty source file\n\n";
  return nullptr;
 }

 CAON_PTR_DEBUG(ChasmRZ_Node ,result)

 caon_ptr<ChasmRZ_Root> root = result->chasm_rz_root();

 CAON_PTR_DEBUG(ChasmRZ_Root ,root)

 if(result)
 {
  qts << "Root local path: " << root->document_path() << "\n\n";
 }

 return result;
}


//void RZ_Prerun_Setup::write_report(QTextStream& qts)
//{


void RZ_Prerun_Setup::output_from_individual_node(QTextStream& qts,
  const ChasmRZ_Node& node, caon_ptr<ChasmRZ_Call_Entry>& rce,
  u2 indent)
{
 if(caon_ptr<ChasmRZ_Root> root = node.chasm_rz_root())
 {
  caon_ptr<ChasmRZ_Node> next_node = traverser_.find_root_entry(&node);

  CAON_PTR_DEBUG(ChasmRZ_Node ,next_node)
  CAON_DEBUG_NOOP

  if(!next_node)
  {
   qts << "Source file appears to have no valid content\n\n";
   return;
  }

  caon_ptr<ChasmRZ_Block_Entry> rbe = new ChasmRZ_Block_Entry(node_factory_.make_block_entry_id());
  caon_ptr<ChasmRZ_Node> rbe_node = node_factory_.make_new_node(rbe);

  &node <<Tf/Qy.Run_Block_Entry>> rbe_node;

  add_observer_function(rbe_node, next_node, Qy.Run_Call_Entry);
 }

}



void RZ_Prerun_Setup::add_observer_function(caon_ptr<ChasmRZ_Node> start_node,
   caon_ptr<ChasmRZ_Node> next_node, const ChasmRZ_Connectors& connector)
{
 caon_ptr<ChasmRZ_Call_Entry> rce = new ChasmRZ_Call_Entry(node_factory_.make_call_entry_id());
 caon_ptr<ChasmRZ_Node> rce_node = node_factory_.make_new_node(rce);

 caon_ptr<RZ_Observer_Function> rof = new RZ_Observer_Function("lex-decl");
 caon_ptr<ChasmRZ_Node> rof_node = node_factory_.make_new_node(rof);

 start_node <<Tf/connector>> rce_node;
 rce_node <<Tf/Qy.Run_Call_Entry>> rof_node;
 rof_node <<Tf/Qy.Run_Call_Sequence>> next_node;

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
