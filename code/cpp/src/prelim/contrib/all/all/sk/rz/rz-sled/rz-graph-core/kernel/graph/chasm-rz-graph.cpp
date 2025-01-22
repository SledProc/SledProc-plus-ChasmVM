
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "chasm-rz-graph.h"
#include "chasm-rz-node.h"

#include "kernel/chasm-rz-root.h"

#include "token/chasm-rz-token.h"

#include "tuple/chasm-rz-tuple-info.h"
#include "code/chasm-rz-call-entry.h"

#include "code/chasm-rz-block-entry.h"
#include "kernel/graph/chasm-rz-connection.h"

#include "rzns.h"

USING_RZNS(RZ_Core)

ChasmRZ_Graph::ChasmRZ_Graph(caon_ptr<ChasmRZ_Node> root_node)
 : node_graph<ChasmRZ_Dominion>(root_node)
{

}

void ChasmRZ_Graph::report(QTextStream& qts)
{
 if(caon_ptr<ChasmRZ_Root> root = root_node_->chasm_rz_root())
 {
  qts << "Root node: " << root->document_path();
 }
 report_from_node(qts, *root_node_);
}

void ChasmRZ_Graph::report_from_node(QTextStream& qts,
 const ChasmRZ_Node& node, int indent)
{
 QString padding(indent, ' ');
 qts << "\n" << padding;
 if(caon_ptr<ChasmRZ_Token> token = node.chasm_rz_token())
 {
  qts << "[token= " << token->string_summary() << "]";
 }
 else if(caon_ptr<ChasmRZ_Call_Entry> rce = node.chasm_rz_call_entry())
 {
  CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce)
  QString extra;
  if(!rce->prefix().isEmpty())
  {
   extra = QString(" (%1) ").arg(rce->prefix());
  }
  if(rce->flags.no_normalize)
  {
   extra += "nn";
  }
  if(rce->flags.no_anticipate)
  {
   extra += "na";
  }
  qts << QString("<call %1%2>").arg(rce->call_id()).arg(extra);
 }
 else if(caon_ptr<ChasmRZ_Block_Entry> rbe = node.chasm_rz_block_entry())
 {
  qts << QString("<block %1:%2>").arg(rbe->block_id()).arg(rbe->nested_depth());
 }
 else if(caon_ptr<ChasmRZ_Tuple_Info> rti = node.chasm_rz_tuple_info())
 {
  if(rti->is_entry())
   qts << QString("<data %1 ...>").arg(rti->data_id());
  else
   qts << "<... data>";
 }
 else
 {
  qts << "<<node/" << node.label() << ">>";
 }
  //Run_Data_Entry
 node.each_connection([this, node, &qts, &padding, &indent]
  (const ChasmRZ_Connectors& connector, const ChasmRZ_Node& target, const ChasmRZ_Connection* connection)
 {
  // //   For debugging...
  QString label = node.label();

//  // //  Somehow, this connector is causing trouble
//  if(connector.case_label == ChasmRZ_Connectors_Case_Labels::Run_Fundef_Map_Key_Sequence)
//  {
//   //?return;
//  }

  // //  This connector is tangential ...
  if(connector.case_label == ChasmRZ_Connectors_Case_Labels::Assignment_Annotation)
  {
   qts << "\n" << padding << " (Has connection: " << connector.label() << ")\n";
   return;
  }

  // //  This connector loops ...
  if(connector.case_label == ChasmRZ_Connectors_Case_Labels::Parent_Block_Map)
  {
   qts << "\n\n" << padding << "Has connection: " << connector.label() << "\n";
   return;
  }

  qts << "\n\n" << padding << "For connection: " << connector.label() << "\n"
      << padding << "==== ";

  if(connection)
  {
   qts << "\n\n" << padding << "Annotated: \n";

   if(caon_ptr<ChasmRZ_Node> cn = connection->chasm_rz_node())
   {
    qts << " [[" << cn->label() << "]]\n";
   }

   qts << padding << "---- ";

  }

  report_from_node(qts, target, indent + 1);
  qts << "\n" << padding << "....";
 });

}
