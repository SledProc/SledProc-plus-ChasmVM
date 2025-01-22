
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef NGML_HTXN_NODE__H
#define NGML_HTXN_NODE__H 

#include "global-types.h"

#include "accessors.h"

#include "whitespace/rz-ngml-whitespace.h"

#include "kans.h"
KANS_CLASS_DECLARE(HTXN ,HTXN_Node_Detail)
KANS_CLASS_DECLARE(HTXN ,HTXN_Document_8b)
USING_KANS(HTXN)


#include "rzns.h"

RZNS_(NGML)

class NGML_HTXN_Node 
{
 u4 detail_code_;
 u4 layer_code_;

 QPair<u4, u4> range_;

 u4 ref_order_;

 NGML_Whitespace ws_;

public:

 NGML_HTXN_Node(u4 detail_code = 0);

 ACCESSORS(u4 ,detail_code)
 ACCESSORS(u4 ,layer_code)

 ACCESSORS(MACRO_PASTE(QPair<u4, u4>) ,range)

 ACCESSORS(NGML_Whitespace ,ws)


 HTXN_Node_Detail* get_node_detail(HTXN_Document_8b* doc);

 u4 get_range_enter();
 u4 get_range_leave();

 u4 get_ref_enter();
 u4 get_ref_leave();

 void set_range_enter(u4 enter);
 void set_range_leave(u4 leave);

 void set_ref_positions(u4 pos, u4 order, 
   u4 leave);

 template<typename T>
 void write_whitespace(T& t)
 {
  t << ws_.to_string();
 }


};

_RZNS(NGML)


#endif //  NGML_HTXN_NODE__H

