
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rz-ngml-output-event-handler.h"

#include "kernel/document/rz-ngml-document.h"
#include "kernel/graph/rz-ngml-node.h"
#include "tile/rz-ngml-token.h"
#include "kernel/output/rz-ngml-output-bundle.h"
#include "tile/rz-ngml-tile.h"
#include "tile/rz-ngml-attribute-tile.h"
#include "tag-command/rz-ngml-tag-command.h"
#include "tile/rz-ngml-paralex-tile.h"

#include "annotation/rz-ngml-annotation-tile.h"

#include "ngml-htxn/ngml-htxn-node.h"

#include <QFile>
#include <QFileInfo>

#include "rzns.h"

USING_RZNS(NGML)


NGML_Output_Event_Handler::NGML_Output_Event_Handler()
{

}

caon_ptr<NGML_Command_Callback>
 NGML_Output_Event_Handler::check_command_callback(const NGML_Output_Bundle& b)
{
 if(caon_ptr<NGML_Tag_Command> ntc = b.node->ngml_tag_command())
 {
  return check_command_callback(ntc);
 }
}

caon_ptr<NGML_Command_Callback>
 NGML_Output_Event_Handler::check_command_callback(caon_ptr<NGML_Tag_Command> ntc)
{
 CAON_PTR_DEBUG(NGML_Tag_Command ,ntc)
 return caon_ptr<NGML_Command_Callback>( nullptr );
}

void NGML_Output_Event_Handler::generate_root(const NGML_Output_Bundle& b, caon_ptr<NGML_Root> nr)
{
}

caon_ptr<NGML_Tag_Command> NGML_Output_Event_Handler::generate_tag_command_entry(const NGML_Output_Bundle& b)
{
 caon_ptr<NGML_Command_Callback> cb = check_command_callback(b);
 if(caon_ptr<NGML_Tag_Command> ntc = b.node->ngml_tag_command())
 {
  CAON_PTR_DEBUG(NGML_Tag_Command ,ntc)
  generate_tag_command_entry(b.with_callback(cb), ntc);

   // // ? wha ?
  if(ntc->flags.is_closed)
  {
   return ntc;
  }
 }
 return caon_ptr<NGML_Tag_Command>( nullptr );
}

void NGML_Output_Event_Handler::generate_tag_command_entry(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{
}

void NGML_Output_Event_Handler::generate_tag_command_leave(const NGML_Output_Bundle& b)
{
 if(caon_ptr<NGML_Tag_Command> ntc = b.node->ngml_tag_command())
 {
  generate_tag_command_leave(b, ntc);
  check_generate_whitespace(b, ntc);
 }
}


void NGML_Output_Event_Handler::tag_command_annotation(caon_ptr<NGML_Tile> nt, caon_ptr<tNode> node)
{
 if(caon_ptr<NGML_Annotation_Tile> tile = node->ngml_annotation_tile())
  tag_command_annotation(nt, tile);
}

void NGML_Output_Event_Handler::tag_command_annotation(caon_ptr<NGML_Tile> nt,
  caon_ptr<NGML_Annotation_Tile> tile)
{

}

void NGML_Output_Event_Handler::generate_tag_command_auto_leave(const NGML_Output_Bundle& b)
{
 if(caon_ptr<NGML_Tag_Command> ntc = b.node->ngml_tag_command())
 {
  generate_tag_command_auto_leave(b, ntc);
  check_generate_whitespace(b, ntc);
 }
}

void NGML_Output_Event_Handler::generate_tag_command_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{
}

void NGML_Output_Event_Handler::generate_tag_command_auto_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{
 generate_tag_command_leave(b, ntc);
}


void NGML_Output_Event_Handler::generate_tag_body_leave(const NGML_Output_Bundle& b)
{
 if(caon_ptr<NGML_Tag_Command> ntc = b.node->ngml_tag_command())
 {
  generate_tag_body_leave(b, ntc);
 }
}

void NGML_Output_Event_Handler::generate_tag_command_entry_connector(const NGML_Output_Bundle& b)
{

}

void NGML_Output_Event_Handler::generate_tag_body_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{
}


void NGML_Output_Event_Handler::generate_tile(const NGML_Output_Bundle& b)
{
 if(caon_ptr<NGML_Tile> tile = b.node->ngml_tile())
 {
  generate_tile(b, tile);
 }
 else if(caon_ptr<NGML_Attribute_Tile> tile = b.node->ngml_attribute_tile())
 {
  generate_tile(b, tile);
 }
 else if(caon_ptr<NGML_Paralex_Tile> tile = b.node->ngml_paralex_tile())
 {
  generate_tile(b, tile);
 }
}

void NGML_Output_Event_Handler::check_update_index(const NGML_Output_Bundle& b,
  NGML_Tile& tile)
{
 b.index += tile.raw_text().length();
}

void NGML_Output_Event_Handler::generate_tile_via_htxn(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn)
{

}

void NGML_Output_Event_Handler::generate_attribute_tile_via_htxn(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn)
{

}

void NGML_Output_Event_Handler::generate_parelex_tile_via_htxn(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn)
{

}

void NGML_Output_Event_Handler::generate_annotation_tile_via_htxn(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn)
{

}

void NGML_Output_Event_Handler::generate_tile(const NGML_Output_Bundle& b,
  caon_ptr<NGML_Tile> tile)
{
 CAON_PTR_DEBUG(NGML_Tile ,tile)

 if(NGML_HTXN_Node* nhn = tile->ngml_htxn_node())
 {
  generate_tile_via_htxn(b, *nhn);
   // // here?
  check_generate_whitespace(b, *nhn);
  return;
 }

 QString rt = tile->raw_text();

 if(!rt.startsWith("*:"))
 {
  b.qts << tile->raw_text();
  check_update_index(b, *tile);
 }
 check_generate_whitespace(b, tile);
}

void NGML_Output_Event_Handler::generate_tile(const NGML_Output_Bundle& b, caon_ptr<NGML_Attribute_Tile> tile)
{
 tile->write_html(b.qts);
}

void NGML_Output_Event_Handler::generate_tile(const NGML_Output_Bundle& b, caon_ptr<NGML_Annotation_Tile> tile)
{
}

void NGML_Output_Event_Handler::handle_annotation(caon_ptr<NGML_Tile> tile, caon_ptr<NGML_Annotation_Tile> atile)
{

}

void NGML_Output_Event_Handler::handle_annotation(caon_ptr<NGML_Tile> tile, caon_ptr<tNode> node)
{
 if(caon_ptr<NGML_Annotation_Tile> atile = node->ngml_annotation_tile())
 {
  handle_annotation(tile, atile);
 }
}

void NGML_Output_Event_Handler::generate_tile(const NGML_Output_Bundle& b, caon_ptr<NGML_Paralex_Tile> tile)
{
 tile->write_html(b.qts);

 check_generate_whitespace(b, tile);

}

void NGML_Output_Event_Handler::check_generate_whitespace(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn)
{
 nhn.write_whitespace(b.qts);
}


void NGML_Output_Event_Handler::check_generate_whitespace(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{
 CAON_PTR_DEBUG(NGML_Tag_Command ,ntc)
 ntc->write_whitespace(b.qts);
 //? check_update_index_after_whitespace(b, ntc);
}

void NGML_Output_Event_Handler::check_generate_whitespace(const NGML_Output_Bundle& b, caon_ptr<NGML_Paralex_Tile> tile)
{
 tile->write_whitespace(b.qts);
}

void NGML_Output_Event_Handler::check_generate_whitespace(const NGML_Output_Bundle& b, caon_ptr<NGML_Tile> tile)
{
 tile->write_whitespace(b.qts);
 check_update_index_after_whitespace(b, *tile);
}

void NGML_Output_Event_Handler::check_update_index_after_whitespace(const NGML_Output_Bundle& b,
  NGML_Tile& tile)
{
 b.index += tile.ws().get_length();
}

void NGML_Output_Event_Handler::check_update_index_after_whitespace(const NGML_Output_Bundle& b,
  NGML_Tag_Command& ntc)
{
 b.index += ntc.ws().get_length();
}

