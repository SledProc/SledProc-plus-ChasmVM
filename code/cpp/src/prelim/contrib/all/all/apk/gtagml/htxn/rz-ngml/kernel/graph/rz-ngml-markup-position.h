
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef RZ_NGML_MARKUP_POSITION__H
#define RZ_NGML_MARKUP_POSITION__H

#include "accessors.h"
#include "flags.h"


#include "kernel/graph/rz-ngml-node.h"

#include "kernel/query/rz-ngml-query.h"

#include "kernel/frame/rz-ngml-frame.h"

#include <QList>
#include <QStack>
#include <QMap>
#include <QVector>

#include "rzns.h"

RZNS_(NGML)

class NGML_Node;

class NGML_Markup_Position
{
 int comment_semis_;
 int comment_tildes_;

 enum Position_States
 {
  Root, Held_Empty, Tag_Command_Entry, Tile_Sequence,
   Tag_Body_Leave, Tag_Command_Leave,
   Active_Khif_Connector, Khif_Tag_Command_Leave, 
   Annotation_Close,
    Tag_Body_Leave_Awaiting_Attributes, 
   Awaiting_Optional, Awaiting_Mandatory
 };

 enum Khif_Connectors
 {
  Khif_N_A, Khif_Entry, Khif_Cross, Khif_Continue,
  Khif_Attribute, Khif_Main_Tile, Khif_Space
 };

 Khif_Connectors parse_khif_connector(QString symbol)
 {
  static QMap<QString, Khif_Connectors> static_map {{
   {"e", Khif_Entry},
   {"m", Khif_Main_Tile},
   {"@", Khif_Attribute},
   {"c", Khif_Continue},
   {"x", Khif_Cross},
   {"s", Khif_Space},
  }};

  return static_map.value(symbol, Khif_Connectors::Khif_N_A);
 }

 Position_States position_state_;
 Position_States held_position_state_;

 typedef NGML_Node tNode;

 QStack<caon_ptr<tNode>> tag_commands_;

 caon_ptr<tNode> current_node_;
 caon_ptr<tNode> current_attribute_node_;

 NGML_Frame fr_;
 const NGML_Query& qry_;

 QString current_connectors_;

 Khif_Connectors current_khif_connector_;

public:

 ACCESSORS(int ,comment_semis)
 ACCESSORS(int ,comment_tildes)

 ACCESSORS(caon_ptr<tNode> ,current_node)

 NGML_Markup_Position(caon_ptr<tNode> current_node);

 void prepare_attribute_sequence();

 void await_mandatory_or_optional(caon_ptr<tNode> node);
 void await_optional(caon_ptr<tNode> node);
 void await_mandatory(caon_ptr<tNode> node);

 void merge_multi_parent_inherited(caon_ptr<tNode> parent,
   caon_ptr<tNode> node);
 void merge_multi_parent_sequence(caon_ptr<tNode> prior,
   caon_ptr<tNode> node);

 void merge_multi_parent_inherited(NGML_Tag_Command& parent,
   NGML_Tag_Command& ntc);
 void merge_multi_parent_sequence(NGML_Tag_Command& parent,
   NGML_Tag_Command& ntc);

// void merge_multi_parent_inherited(NGML_Tag_Command& parent,
//   NGML_Tile& ntc);
// void merge_multi_parent_sequence(NGML_Tag_Command& parent,
//   NGML_Tile& ntc);

 void merge_multi_parent_inherited(NGML_Tile& parent,
   NGML_Tag_Command& ntc);

 void attribute_sequence_leave();


 void enter_multiline_comment(int semis, int tildes);
 bool check_leave_multiline_comment(int semis, int tildes);
 void add_tile_node(caon_ptr<NGML_Node> node);
 void add_attribute_tile_node(caon_ptr<NGML_Node> node);
 void tag_command_entry(caon_ptr<tNode> node);
 void restore_current_node(caon_ptr<tNode> node);
 caon_ptr<tNode> tag_command_instruction(caon_ptr<tNode> node);
 void tag_body_leave();
 caon_ptr<tNode> check_tag_command_leave(QString tag_command);
 void rewind_tag_command_leave(QString tag_command);

 void tag_command_annotation(caon_ptr<tNode> node);

 QString current_tag_command_name();
 caon_ptr<NGML_Tag_Command> get_current_tag_command();

 void annotation_entry(caon_ptr<tNode> node, caon_ptr<tNode> anode);
 void close_annotation();

 caon_ptr<tNode> tag_command_leave();
 void confirm_tag_command_leave(caon_ptr<tNode> node);

 void load_khif_connectors(QString connectors);
 void khif_tag_command_leave();

 void attach_khif_tag_command_node(caon_ptr<NGML_Node> node);
 void add_khif_tile_node(caon_ptr<tNode> node);

 bool expecting_khif_attribute_tile()
 {
  return current_khif_connector_ == Khif_Connectors::Khif_Attribute;
 }


};


_RZNS(NGML)

#endif

