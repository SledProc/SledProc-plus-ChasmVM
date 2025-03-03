
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rpi-file.h"

#include "rpi-block.h"

#include "rz-graph-visit/rz-graph-visitor-phaon.h"

#include "rz-graph-core/kernel/graph/chasm-rz-node.h"




USING_RZNS(GVal)


RPI_File::RPI_File(QString path, caon_ptr<RPI_Block> implicit_block)
  : path_(path), implicit_block_(implicit_block)
{

}

PGB_IR_Build& RPI_File::pgb()
{
 return implicit_block_->pgb_;
}

_PGB_IR_Build RPI_File::pgb(QList<PGB_IR_Build::Text_With_Purpose>& tps)
{
 return pgb()(tps);
}

void RPI_File::scan_top_level(RZ_Graph_Visitor_Phaon& visitor_phaon)
{
 caon_ptr<ChasmRZ_Node> rn = visitor_phaon.visitor().graph_root_node();

 scan(visitor_phaon, *rn);

}

void RPI_File::scan(RZ_Graph_Visitor_Phaon& visitor_phaon, ChasmRZ_Node& start_node)
{
 caon_ptr<RZ_ASG_Block_Info> rbi;
 if(caon_ptr<ChasmRZ_Node> ben = visitor_phaon.visitor().find_block_entry_node(&start_node, rbi))
 {
  CAON_PTR_DEBUG(ChasmRZ_Node ,ben)

  caon_ptr<ChasmRZ_Node> cen = visitor_phaon.visitor().find_run_call_entry(ben);

  // // does this need to be more fine-grained?

  caon_ptr<ChasmRZ_Block_Entry> rbe = ben->chasm_rz_block_entry();
  CAON_PTR_DEBUG(ChasmRZ_Block_Entry ,rbe)

//  if(rbe)
//  {
//   CAON_PTR_DEBUG(ChasmRZ_Block_Entry ,rbe)
//   lexical_scope_ = rbe->lexical_scope();
//  }

  CAON_PTR_DEBUG(ChasmRZ_Node ,cen)

  CAON_DEBUG_NOOP
 }
}


void RPI_File::write_top_level(QList<PGB_IR_Build::Text_With_Purpose>& tps, QTextStream* qts)
{
 pgb(tps).make_file_node(path_);



}
