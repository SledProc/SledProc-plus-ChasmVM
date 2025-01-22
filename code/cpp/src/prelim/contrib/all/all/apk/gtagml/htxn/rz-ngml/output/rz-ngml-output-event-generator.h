
#ifndef RZ_NGML_OUTPUT_EVENT_GENERATOR__H
#define RZ_NGML_OUTPUT_EVENT_GENERATOR__H

#include "relae-graph/relae-node-ptr.h"
#include "kernel/rz-ngml-dominion.h"
#include "kernel/query/rz-ngml-query.h"
#include "output/rz-ngml-output-base.h"
#include "kernel/output/rz-ngml-connection-descriptor.h"

#include "accessors.h"

#include "kans.h"
KANS_CLASS_DECLARE(HTXN ,HTXN_Document_8b)
USING_KANS(HTXN)

#include "rzns.h"

RZNS_(NGML)

class NGML_Graph;
class NGML_Document;
class NGML_Node;
class NGML_Token;
class NGML_Root;
class NGML_Output_Base;
class NGML_Output_Event_Handler;
class NGML_Output_Bundle;

class NGML_Output_Event_Generator : NGML_Output_Base
{
 NGML_Output_Event_Handler& handler_;

 HTXN_Document_8b* htxn_document_;

protected:

 typedef NGML_Node tNode;

public:

 NGML_Output_Event_Generator(NGML_Output_Base& base, NGML_Output_Event_Handler& handler);

 ACCESSORS(HTXN_Document_8b* ,htxn_document)

 void generate(QTextStream& qts);

 void generate_tag_command_entry(const NGML_Output_Bundle& b);
 void generate_tag_command_leave(const NGML_Output_Bundle& b);

 void generate_attribute_tile(const NGML_Output_Bundle& b);
 void generate_tile(const NGML_Output_Bundle& b);
 void generate_tile_sequence(const NGML_Output_Bundle& b);
 void check_tile_follow(const NGML_Output_Bundle& b);

// void generate_tag_command_sequence(const const NGML_Output_Bundle& b);

// void check_generate_token(const const NGML_Output_Bundle& b);

 caon_ptr<tNode> check_tag_command_entry_connectors(const NGML_Output_Bundle& b,
  bool write_connector = false);
 caon_ptr<tNode> check_closed_tag_command_entry_connectors(const NGML_Output_Bundle& b);

 void check_tag_body_connectors(const NGML_Output_Bundle& b);
 void check_connectors_on_root(const NGML_Output_Bundle& b);


 void check_closed_tag_command_cross_connectors(const NGML_Output_Bundle& b);

 void check_closed_tag_command_tile_connectors(const NGML_Output_Bundle& b);

 void check_tag_command_tile_connectors(const NGML_Output_Bundle& b);
 void check_tag_command_cross_connectors(const NGML_Output_Bundle& b);

 // void check_cross_connectors(const const NGML_Output_Bundle& b);
// void check_call_connectors(const const NGML_Output_Bundle& b);

};

_RZNS(NGML)


#endif
