
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "ngml-htxn-node.h"

#include "htxn/htxn-document-8b.h"

USING_RZNS(NGML)

NGML_HTXN_Node::NGML_HTXN_Node(u4 detail_code) 
  :  detail_code_(detail_code), 
     layer_code_(0), range_({0,0}),
     ref_order_(0)
{

}

HTXN_Node_Detail* NGML_HTXN_Node::get_node_detail(HTXN_Document_8b* doc)
{
 if(detail_code_ == 0)
   return nullptr;
 return doc->get_node_detail(detail_code_);
}


void NGML_HTXN_Node::set_range_enter(u4 enter)
{
 range_.first = enter;
}

void NGML_HTXN_Node::set_range_leave(u4 leave)
{
 range_.second = leave;
}

void NGML_HTXN_Node::set_ref_positions(u4 pos, u4 order, 
  u4 leave)
{
 ref_order_ = order;
 range_ = {pos, leave};
}

u4 NGML_HTXN_Node::get_ref_enter()
{
 if(ref_order_ == 0)
   return 0;
 return range_.first;
}

u4 NGML_HTXN_Node::get_ref_leave()
{
 if(ref_order_ == 0)
   return 0;
 return range_.second;
}


u4 NGML_HTXN_Node::get_range_enter()
{
 if(ref_order_ == 0)
   return range_.first;
 return 0;
}

u4 NGML_HTXN_Node::get_range_leave()
{
 if(ref_order_ == 0)
   return range_.second;
 return 0;
}


