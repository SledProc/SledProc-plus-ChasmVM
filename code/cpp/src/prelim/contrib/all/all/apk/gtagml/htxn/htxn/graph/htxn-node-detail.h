
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef HTXN_NODE_DETAIL__H
#define HTXN_NODE_DETAIL__H

#include "accessors.h"

#include "global-types.h"

#include "flags.h"

#include "kans.h"

#include <QString>
#include <QList>
#include <QDate>

#include <functional>

#include "global-types.h"


class QTextStream;

KANS_(HTXN)

class Glyph_Layer_8b;

struct HTXN_Node_Detail
{ 
flags_(4)

 bool region:1;
 bool self_close:1;
 bool optional:1;
 bool key_with_ref:1;
 bool key_no_ref:1; 
 bool has_continue:1;
 bool split_node_ref:1;
 bool ref_preempts_wrap:1;

 bool sole_line:1;
 bool pre_line_gap:1;
 bool post_line_gap:1;
 bool pre_space_gap:1;
 bool post_space_gap:1;
 bool pre_line_double_gap:1;
 bool post_line_double_gap:1;

 bool region_main_preempts_wrap:1;

 bool wmi_left:1;
 bool wmi_none:1;
 bool wmi_with_space:1;

 bool needs_write_end:1;
 bool is_ghosted:1;

 bool has_nav_ref_enter:1;
 bool has_nav_ref_cross:1;

 bool has_ref_package:1;
 bool has_ref_package_no_layer:1;

 bool is_fiat:1;
 bool main_only_block_environment:1;
 bool block_environment_marked_main:1;

_flags

 struct Ref_Package
 {
  Glyph_Layer_8b* gl;
  u4 enter_code;  
  u4 cross_code;  
 };

 struct Ref_Package_No_Layer
 {
  u4 enter_code;  
  u4 cross_code;  
 };

 enum Space_Codes {
   N_A = 0, Pre_Line_Gap = 1, Post_Line_Gap = 2,
   Pre_Space_Gap = 3, Post_Space_Gap = 4,
   Pre_Line_Double_Gap = 5, Post_Line_Double_Gap = 6
 };

 enum class Wrap_Mode_Indicator_Codes {
  Normal, Left, Left_With_Space, None, None_With_Space, 
 };

 void incorporate_wrap_mode_indicator(Wrap_Mode_Indicator_Codes wmic);

 void* node_ref;

 u4 enter;
 u4 leave;

 u2 order;

// u8 get_encoding();
// u8 get_raw_encoding();

 HTXN_Node_Detail(u4 e, u4 l);

 HTXN_Node_Detail();

 QString get_pre_space() const;
 QString get_post_space() const;

 void note_space_code(Space_Codes sc);
 void note_whitespace_code(u2 wsc);
 static QPair<Space_Codes, Space_Codes> parse_whitespace_code(u2 wsc);

 void add_node_ref(u4 nc);
 QVector<u4>* get_refs() const;
 QVector<u4>* get_refs_from_split() const;
 Glyph_Layer_8b* get_layer_from_split() const;

 Glyph_Layer_8b* get_layer_from_package(void* pkg) const;
 Ref_Package* set_layer_from_package(Glyph_Layer_8b* gl, void* pkg);

 Glyph_Layer_8b* get_layer() const;
 void set_layer(Glyph_Layer_8b* gl);

 void prepare_read();

};

//QTextStream& operator<<(QTextStream& qts, const HTXN_Node_Detail rhs);

_KANS(HTXN)

#endif // HTXN_NODE_DETAIL__H
