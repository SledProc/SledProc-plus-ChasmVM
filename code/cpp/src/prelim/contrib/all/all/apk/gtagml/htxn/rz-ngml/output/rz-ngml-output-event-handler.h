
#ifndef RZ_NGML_OUTPUT_EVENT_HANDLER__H
#define RZ_NGML_OUTPUT_EVENT_HANDLER__H

#include <QTextStream>

#include "relae-graph/relae-node-ptr.h"
#include "kernel/rz-ngml-dominion.h"
#include "kernel/query/rz-ngml-query.h"
#include "kernel/output/rz-ngml-connection-descriptor.h"
#include "accessors.h"
#include "rzns.h"

RZNS_(NGML)

class NGML_Graph;
class NGML_Document;
class NGML_Node;
class NGML_Tag_Command;
class NGML_Root;
class NGML_Annotation_Tile;
class NGML_Output_Bundle;
class NGML_Command_Callback;
class NGML_HTXN_Node;

class NGML_Output_Event_Handler
{
protected:

 typedef NGML_Node tNode;

 NGML_Output_Event_Handler();

public:

 virtual caon_ptr<NGML_Command_Callback> check_command_callback(const NGML_Output_Bundle& b);
 virtual caon_ptr<NGML_Command_Callback> check_command_callback(caon_ptr<NGML_Tag_Command> ntc);


 virtual caon_ptr<NGML_Tag_Command> generate_tag_command_entry(const NGML_Output_Bundle& b);
 virtual void generate_tag_command_entry(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc);

 virtual void check_update_index(const NGML_Output_Bundle& b, NGML_Tile& tile);
 virtual void check_update_index_after_whitespace(const NGML_Output_Bundle& b,
   NGML_Tile& tile);
 virtual void check_update_index_after_whitespace(const NGML_Output_Bundle& b,
   NGML_Tag_Command& ntc);


 virtual void generate_tag_command_leave(const NGML_Output_Bundle& b);
 virtual void generate_tag_command_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc);

 virtual void generate_tag_command_entry_connector(const NGML_Output_Bundle& b);

 virtual void generate_tag_command_auto_leave(const NGML_Output_Bundle& b);
 virtual void generate_tag_command_auto_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc);

 virtual void generate_tag_body_leave(const NGML_Output_Bundle& b);
 virtual void generate_tag_body_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc);

 virtual void generate_tile(const NGML_Output_Bundle& b);
 virtual void generate_tile(const NGML_Output_Bundle& b, caon_ptr<NGML_Tile> tile);
 virtual void generate_tile(const NGML_Output_Bundle& b, caon_ptr<NGML_Attribute_Tile> tile);
 virtual void generate_tile(const NGML_Output_Bundle& b, caon_ptr<NGML_Paralex_Tile> tile);
 virtual void generate_tile(const NGML_Output_Bundle& b, caon_ptr<NGML_Annotation_Tile> tile);

 virtual void generate_tile_via_htxn(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn);
 virtual void generate_attribute_tile_via_htxn(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn);
 virtual void generate_parelex_tile_via_htxn(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn);
 virtual void generate_annotation_tile_via_htxn(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn);


 virtual void handle_annotation(caon_ptr<NGML_Tile> tile, caon_ptr<NGML_Annotation_Tile> atile);
 virtual void handle_annotation(caon_ptr<NGML_Tile> tile, caon_ptr<tNode> node);


 virtual void check_generate_whitespace(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn);

 virtual void check_generate_whitespace(const NGML_Output_Bundle& b, caon_ptr<NGML_Tile> tile);
 virtual void check_generate_whitespace(const NGML_Output_Bundle& b, caon_ptr<NGML_Paralex_Tile> tile);
 virtual void check_generate_whitespace(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc);

 virtual void tag_command_annotation(caon_ptr<NGML_Tile> nt, caon_ptr<tNode> node);
 virtual void tag_command_annotation(caon_ptr<NGML_Tile> nt, caon_ptr<NGML_Annotation_Tile> tile);

 virtual void generate_root(const NGML_Output_Bundle& b, caon_ptr<NGML_Root> nr);



};

_RZNS(NGML)


#endif
