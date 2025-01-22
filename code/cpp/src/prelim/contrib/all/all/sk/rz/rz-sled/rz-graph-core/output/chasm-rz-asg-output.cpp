
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "chasm-rz-asg-output.h"

#include "kernel/document/chasm-rz-document.h"

#include "kernel/graph/chasm-rz-graph.h"
#include "kernel/graph/chasm-rz-node.h"

#include "kernel/chasm-rz-root.h"

#include "token/chasm-rz-token.h"

#include "tuple/chasm-rz-tuple-info.h"


#include "token/chasm-rz-token.h"

#include "rzns.h"


USING_RZNS(RZ_Core)


ChasmRZ_ASG_Output::ChasmRZ_ASG_Output(caon_ptr<ChasmRZ_Document> document)
 : document_(document), Cf(ChasmRZ_Frame::instance("casement")),
    Sf(ChasmRZ_Frame::instance("semantic")),
    Qy(ChasmRZ_Query::instance())
{

}


QString ChasmRZ_ASG_Output::get_asg_out(const ChasmRZ_Token& token)
{
 return token.get_asg_out();
}


void ChasmRZ_ASG_Output::output(QString path)
{
 document_->write_report(path, *this);
}

void ChasmRZ_ASG_Output::write_report(QTextStream& qts)
{
 caon_ptr<ChasmRZ_Node> node = document_->graph()->root_node();
 output_from_node(qts, *node);
}


void ChasmRZ_ASG_Output::output_from_node(QTextStream& qts,
 const ChasmRZ_Node& node, int indent, caon_ptr<ChasmRZ_Call_Entry> rce_ptr)
{
 CAON_PTR_DEBUG(ChasmRZ_Call_Entry ,rce_ptr)

 QString padding(indent, ' ');

 caon_ptr<ChasmRZ_Call_Entry> rce = nullptr;
 caon_ptr<ChasmRZ_Block_Entry> rbe = nullptr;
 caon_ptr<ChasmRZ_Tuple_Info> rde = nullptr;

 if(caon_ptr<ChasmRZ_Token> token = node.chasm_rz_token())
 {
  init_token(*token);
  report_token(qts, *token);
 }

 else if(rce = node.chasm_rz_call_entry())
 {
 }
 else if(rbe = node.chasm_rz_block_entry())
 {

 }
 else if(rde = node.chasm_rz_tuple_info())
 {

 }

#ifdef NO_CAON
  caon_ptr<ChasmRZ_Node> pnode = caon_ptr<ChasmRZ_Node>( &node );
#else
 caon_ptr<ChasmRZ_Node> pnode = &node;
#endif

 if(caon_ptr<ChasmRZ_Node> next_node = Qy.Run_Call_Sequence(pnode))
 {
  qts << ' ';
  output_from_node(qts, *next_node, indent + 1);
 }

 if(caon_ptr<ChasmRZ_Node> next_node = Qy.Run_Call_Entry(pnode))
 {
  CAON_PTR_DEBUG(ChasmRZ_Node ,next_node)
  if(rce)
  {
   qts << "\n" << padding;
   qts << '(';
   report_call_entry(qts, *rce);
   output_from_node(qts, *next_node, indent, rce);
   report_call_leave(qts, *rce);
   qts << ')';

   if(caon_ptr<ChasmRZ_Node> cross_node = Qy.Run_Cross_Sequence(pnode))
   {
    qts << ' ';
    output_from_node(qts, *cross_node, indent);
   }
  }
  else
  {
   output_from_node(qts, *next_node, indent + 1);
  }

  // //?  for do map ...
  caon_ptr<ChasmRZ_Node> do_map_continue_node = nullptr;
  caon_ptr<ChasmRZ_Node> temp_next_node = next_node;
  while(true)
  {
   // //  check if these nodes are being visited multiple times?
   do_map_continue_node = Qy.Run_Nested_Do_Map_Block_Entry(temp_next_node);
   if(do_map_continue_node)
   {
    CAON_PTR_DEBUG(ChasmRZ_Node ,do_map_continue_node)
    qts << ' ';
    output_from_node(qts, *do_map_continue_node, indent);
    temp_next_node = do_map_continue_node;
   }
   else
   {
    break;
   }
  }


 }

 if(caon_ptr<ChasmRZ_Node> next_node = Qy.Run_Block_Entry(pnode))
 {
  qts << "\n" << padding;
  qts << "(block ";
  output_from_node(qts, *next_node, indent + 1);
  if(caon_ptr<ChasmRZ_Node> cross_node = Qy.Run_Cross_Sequence(next_node))
  {
   output_from_node(qts, *cross_node, indent + 1);
  }
  qts << ')';
 }

 if(caon_ptr<ChasmRZ_Node> next_node = Qy.Run_Data_Entry(pnode))
 {
  CAON_PTR_DEBUG(ChasmRZ_Node ,next_node)
  next_node->debug_connections();
  if(caon_ptr<ChasmRZ_Tuple_Info> rti = next_node->chasm_rz_tuple_info())
  {
   qts << "\n" << padding << ' ';
   report_tuple_info_entry(qts, *rti, rce);
   if(next_node = Qy.Run_Data_Entry(next_node))
    output_from_node(qts, *next_node, indent + 1);
   report_tuple_info_leave(qts, *rti, rce);
  }
  if(caon_ptr<ChasmRZ_Node> cross_node = Qy.Run_Cross_Sequence(pnode))
  {
   qts << ' ';
   output_from_node(qts, *cross_node, indent);
  }
 }

 if(caon_ptr<ChasmRZ_Node> next_node = Qy.Run_Function_Def_Entry(pnode))
 {
  CAON_PTR_DEBUG(ChasmRZ_Node ,next_node)
  next_node->debug_connections();

  // Function Def Entry followed by Call Entry then
  //  Cross Sequence (not Block Entry)
  // probably implies type expression

  if(caon_ptr<ChasmRZ_Node> call_entry_node = Qy.Run_Call_Entry(next_node))
  {
   if(caon_ptr<ChasmRZ_Node> entry_node = Qy.Run_Cross_Sequence(call_entry_node))
   {
    CAON_PTR_DEBUG(ChasmRZ_Node ,entry_node)
    output_from_node(qts, *entry_node);
   }
  }
 }
}


