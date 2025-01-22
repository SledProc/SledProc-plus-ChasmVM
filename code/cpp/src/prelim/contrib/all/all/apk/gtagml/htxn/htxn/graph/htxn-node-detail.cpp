
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "htxn-node-detail.h"

USING_KANS(HTXN)

HTXN_Node_Detail::HTXN_Node_Detail(u4 e, u4 l)
 :  Flags(0), enter(e), leave(l), node_ref(nullptr), order(0)
{

}

HTXN_Node_Detail::HTXN_Node_Detail()
 :  Flags(0), enter(0), leave(0), node_ref(nullptr), order(0)
{

}

void HTXN_Node_Detail::incorporate_wrap_mode_indicator(Wrap_Mode_Indicator_Codes wmic)
{
 switch(wmic)
 {
 case Wrap_Mode_Indicator_Codes::Normal:
  break;
 case Wrap_Mode_Indicator_Codes::Left_With_Space:
  flags.wmi_with_space = true;
  // fallthrough ...
  [[clang::fallthrough]];
 case Wrap_Mode_Indicator_Codes::Left:
  flags.wmi_left = true;
  break;
 case Wrap_Mode_Indicator_Codes::None_With_Space:
  // fallthrough ...
  flags.wmi_with_space = true;
  [[clang::fallthrough]];
 case Wrap_Mode_Indicator_Codes::None:
  flags.wmi_none = true;
  break;
 }
}


QVector<u4>* HTXN_Node_Detail::get_refs_from_split() const
{
 QPair<Glyph_Layer_8b*, QVector<u4>>* pr = 
   static_cast<QPair<Glyph_Layer_8b*, QVector<u4>>*>(node_ref);
 return &pr->second;
}

QString HTXN_Node_Detail::get_pre_space() const
{
 if(flags.pre_line_gap)
   return "\n";
 if(flags.pre_line_double_gap)
   return "\n\n";
 if(flags.pre_space_gap)
   return " ";
 return {};
}

QString HTXN_Node_Detail::get_post_space() const
{
 if(flags.post_line_gap)
   return "\n";
 if(flags.post_line_double_gap)
   return "\n\n";
 if(flags.post_space_gap)
   return " ";
 return {};
}


QVector<u4>* HTXN_Node_Detail::get_refs() const
{
 return (flags.split_node_ref)? get_refs_from_split() : nullptr;
}

void HTXN_Node_Detail::note_whitespace_code(u2 wsc)
{
 if(wsc == 0)
   return;
 QPair<Space_Codes, Space_Codes> pr = parse_whitespace_code(wsc);
 note_space_code(pr.first);
 note_space_code(pr.second);
}

QPair<HTXN_Node_Detail::Space_Codes, HTXN_Node_Detail::Space_Codes>
  HTXN_Node_Detail::parse_whitespace_code(u2 wsc)
{
 static QMap<u2, QPair<Space_Codes, Space_Codes>> static_map {{
   {2020, {Pre_Line_Double_Gap, Post_Line_Double_Gap}},
   {2021, {Pre_Line_Double_Gap, Post_Line_Double_Gap}},
   {2120, {Pre_Line_Double_Gap, Post_Line_Double_Gap}},
   {2121, {Pre_Line_Double_Gap, Post_Line_Double_Gap}},

   {2010, {Pre_Line_Double_Gap, Post_Line_Gap}},
   {2011, {Pre_Line_Double_Gap, Post_Line_Gap}},
   {2110, {Pre_Line_Double_Gap, Post_Line_Gap}},
   {2111, {Pre_Line_Double_Gap, Post_Line_Gap}},

   {2001, {Pre_Line_Double_Gap, Post_Space_Gap}},
   {2101, {Pre_Line_Double_Gap, Post_Space_Gap}},

   {1020, {Pre_Line_Gap, Post_Line_Double_Gap}},
   {1021, {Pre_Line_Gap, Post_Line_Double_Gap}},
   {1120, {Pre_Line_Gap, Post_Line_Double_Gap}},
   {1121, {Pre_Line_Gap, Post_Line_Double_Gap}},

   {1010, {Pre_Line_Gap, Post_Line_Gap}},
   {1011, {Pre_Line_Gap, Post_Line_Gap}},
   {1110, {Pre_Line_Gap, Post_Line_Gap}},
   {1111, {Pre_Line_Gap, Post_Line_Gap}},

   {1001, {Pre_Line_Gap, Post_Space_Gap}},
   {1101, {Pre_Line_Gap, Post_Space_Gap}},

   {120, {Pre_Space_Gap, Post_Line_Double_Gap}},
   {121, {Pre_Space_Gap, Post_Line_Double_Gap}},

   {110, {Pre_Space_Gap, Post_Line_Gap}},
   {111, {Pre_Space_Gap, Post_Line_Gap}},

   {101, {Pre_Space_Gap, Post_Space_Gap}},
  }};

 return static_map.value(wsc, {Space_Codes::N_A, Space_Codes::N_A});
}

void HTXN_Node_Detail::note_space_code(Space_Codes sc)
{
 switch (sc)
 {
 case Pre_Line_Gap: flags.pre_line_gap = true; break;
 case Post_Line_Gap: flags.post_line_gap = true; break;
 case Pre_Space_Gap: flags.pre_space_gap = true; break;
 case Post_Space_Gap: flags.post_space_gap = true; break;
 case Pre_Line_Double_Gap: flags.pre_line_double_gap = true; break;
 case Post_Line_Double_Gap: flags.post_line_double_gap = true; break;
 default: break;
 }
}


void HTXN_Node_Detail::add_node_ref(u4 nc)
{
 if(node_ref)
 {
  if(flags.split_node_ref)
  {
   QVector<u4>* refs = get_refs_from_split();
   refs->push_back(nc);
  }
  else
  {
   QPair<void*, QVector<u4>>* pr = new
     QPair<void*, QVector<u4>>({node_ref, {nc}});

//   QPair<Glyph_Layer_8b*, QVector<u4>>* pr = new
//     QPair<Glyph_Layer_8b*, QVector<u4>>(
//     {static_cast<Glyph_Layer_8b*>(node_ref), {nc}});
   node_ref = pr;
   flags.split_node_ref = true;
  }
 }
 else
 {
  node_ref = new 
    QPair<void*, QVector<u4>>({nullptr, {nc}});
  flags.split_node_ref = true;
 }
}

Glyph_Layer_8b* HTXN_Node_Detail::get_layer_from_package(void* pkg) const
{
 if(flags.has_ref_package)
   return static_cast<Ref_Package*>(pkg)->gl;
 if(flags.has_ref_package_no_layer)
   return nullptr;
 return static_cast<Glyph_Layer_8b*>(pkg);
}

Glyph_Layer_8b* HTXN_Node_Detail::get_layer_from_split() const
{
 void* pkg = static_cast<QPair<void*, QVector<u4>>*>(node_ref)->first;
 return get_layer_from_package(pkg);
}

Glyph_Layer_8b* HTXN_Node_Detail::get_layer() const
{
 if(flags.split_node_ref)
   return get_layer_from_split();
 return get_layer_from_package(node_ref);
}

HTXN_Node_Detail::Ref_Package* HTXN_Node_Detail::set_layer_from_package(Glyph_Layer_8b* gl, void* pkg)
{
 if(flags.has_ref_package)
 {
  static_cast<Ref_Package*>(pkg)->gl = gl;
  return nullptr;
 }
 if(flags.has_ref_package_no_layer)
 {
  Ref_Package_No_Layer* old = static_cast<Ref_Package_No_Layer*>(pkg);
  Ref_Package* result = new Ref_Package{gl, old->cross_code, old->enter_code};
  flags.has_ref_package = true;
  flags.has_ref_package_no_layer = false;
  return result;
 }
}

void HTXN_Node_Detail::set_layer(Glyph_Layer_8b* gl)
{
 if(flags.split_node_ref)
 {
  void*& pkg = static_cast<QPair<void*, QVector<u4>>*>(node_ref)->first;
  if(flags.has_ref_package)
    set_layer_from_package(gl, pkg);
  else if(flags.has_ref_package_no_layer)
  {
   Ref_Package_No_Layer* old = static_cast<Ref_Package_No_Layer*>(pkg);
   pkg = set_layer_from_package(gl, old);
   delete old;
  }
  else
    pkg = gl;
 }
 else if(flags.has_ref_package)
   set_layer_from_package(gl, node_ref);
 else if(flags.has_ref_package_no_layer)
 {
  Ref_Package_No_Layer* old = static_cast<Ref_Package_No_Layer*>(node_ref);
  node_ref = set_layer_from_package(gl, old);
  delete old;
 }
 else
   node_ref = gl;
}

void HTXN_Node_Detail::prepare_read()
{
 if(flags.split_node_ref)
 {
  node_ref = new QPair<void*, QVector<u4>>;
 }
}


// u8 HTXN_Node_Detail::get_encoding()
// {
// u8 result = (enter << 32) | leave ;
// if(this.region)
//   result 
// }

// u8 HTXN_Node_Detail::get_raw_encoding()
// {
// u8 result = (u8) *this;
// return result;
// }

