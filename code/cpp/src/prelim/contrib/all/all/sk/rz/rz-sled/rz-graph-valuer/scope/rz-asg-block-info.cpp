
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include <QString>
#include <QMap>

#include <QStringList>

#include "rz-asg-block-info.h"

#include "rz-graph-token/token/rz-asg-token.h"

#include "rz-graph-core/kernel/graph/chasm-rz-node.h"
#include "rz-graph-core/code/chasm-rz-block-entry.h"


USING_RZNS(GVal)

RZ_ASG_Block_Info::RZ_ASG_Block_Info(caon_ptr<ChasmRZ_Node> block_entry_node, caon_ptr<RZ_ASG_Block_Info> parent_block_info)
 : block_entry_node_(block_entry_node), continue_node_(nullptr), iterator_count_(0),
   allocation_count_(increment_allocation_count()),
   child_block_info_(nullptr), parent_block_info_(parent_block_info)
{

}



void RZ_ASG_Block_Info::add_assignment_count(QString sym)
{
 ++assignment_counts_[sym];
}


QString RZ_ASG_Block_Info::symbol_string(QString sym)
{
 int c = assignment_counts_.value(sym, 0);
 if(c > 0)
 {
  sym += QString("'%1").arg(c);
 }
 return sym;
}

void RZ_ASG_Block_Info::reset_iterator()
{
 iterator_count_ = 0;
}

caon_ptr<RZ_ASG_Block_Info> RZ_ASG_Block_Info::has_returns_assignments()
{
 if(returns_assignments_.isEmpty())
 {
  if(child_block_info_)
  {
   return child_block_info_->has_returns_assignments();
  }
  return nullptr;
 }
 return this;
}

bool RZ_ASG_Block_Info::needs_else()
{
 if(block_entry_node_)
 {
  if(caon_ptr<ChasmRZ_Block_Entry> rbe = block_entry_node_->chasm_rz_block_entry())
  {
   return (rbe->flags.if_block) && !(rbe->flags.has_else_block);
  }
 }
 return false;
}


bool RZ_ASG_Block_Info::check_final_iterator()
{
 ++iterator_count_;
 int total = assignments_.size() + returns_assignments_.size();
 return iterator_count_ == total;
}

void RZ_ASG_Block_Info::add_assignment(caon_ptr<ChasmRZ_Node> key, const RZ_ASG_Value_Holder& vh)
{
 assignments_[key] = vh;
 assignment_order_.push_back(key);
}

void RZ_ASG_Block_Info::add_returns_assignment(const RZ_ASG_Value_Holder& vh)
{
 returns_assignments_.push_back(vh);
}
