
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_ASG_BLOCK_INFO__H
#define RZ_ASG_BLOCK_INFO__H

#include "accessors.h"
#include "flags.h"

#include "relae-graph/relae-caon-ptr.h"
#include "rz-asg-value-holder.h"


#include <QString>
#include <QMap>
#include <QList>

#include "rzns.h"

RZNS_(RZ_Core)

class ChasmRZ_Node;

_RZNS(RZ_Core)

USING_RZNS(RZ_Core)
USING_RZNS(GBuild)

RZNS_(GBuild)

class RZ_ASG_Token;

_RZNS(GBuild)



RZNS_(GVal)

class RZ_ASG_Block_Info
{
 QMap<caon_ptr<ChasmRZ_Node>, RZ_ASG_Value_Holder> assignments_;
 QList<caon_ptr<ChasmRZ_Node>> assignment_order_;

 caon_ptr<ChasmRZ_Node> block_entry_node_;
 caon_ptr<ChasmRZ_Node> continue_node_;

 QList<RZ_ASG_Value_Holder> returns_assignments_;

 QMap<QString, int> assignment_counts_;

 int iterator_count_;

 int allocation_count_;

 caon_ptr<RZ_ASG_Block_Info> child_block_info_;
 caon_ptr<RZ_ASG_Block_Info> parent_block_info_;

 static int increment_allocation_count()
 {
  static int count = 0;
  return ++count;
 }

public:

 RZ_ASG_Block_Info(caon_ptr<ChasmRZ_Node> block_entry_node, caon_ptr<RZ_ASG_Block_Info> parent_block_info);

 ACCESSORS(caon_ptr<ChasmRZ_Node> ,block_entry_node)
 ACCESSORS(caon_ptr<ChasmRZ_Node> ,continue_node)

 ACCESSORS__RGET(QList<caon_ptr<ChasmRZ_Node>> ,assignment_order)
 ACCESSORS__RGET(QList<RZ_ASG_Value_Holder> ,returns_assignments)
 ACCESSORS__GET(int ,allocation_count)

 ACCESSORS(caon_ptr<RZ_ASG_Block_Info> ,child_block_info)
 ACCESSORS(caon_ptr<RZ_ASG_Block_Info> ,parent_block_info)

 void add_assignment(caon_ptr<ChasmRZ_Node> key, const RZ_ASG_Value_Holder& vh);

 void add_returns_assignment(const RZ_ASG_Value_Holder& vh);

 void add_assignment_count(QString sym);

 QString symbol_string(QString sym);

 bool check_final_iterator();
 bool needs_else();

 caon_ptr<RZ_ASG_Block_Info> has_returns_assignments();

 void reset_iterator();

};

_RZNS(GVal)

#endif //RZ_ASG_BLOCK_INFO__H
