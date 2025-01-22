
#ifndef RZ_NGML_DOMINION__H
#define RZ_NGML_DOMINION__H


#include "relae-graph/relae-node-ptr.h"
#include "relae-graph/relae-caon-ptr.h"

#include "rzns.h"



#define DOMINION_TYPE DOMINION_TYPE_DECLARE
#include "dominion/types.h"
#undef DOMINION_TYPE

RZNS_(NGML)

struct NGML_Galaxy;
class NGML_Node;
class NGML_Connectors;
class NGML_Connection;
class NGML_Annotated_Connectors;
class NGML_Frame;
class NGML_Root;
class NGML_Graph;
class NGML_Document;


struct NGML_Dominion
{
 typedef NGML_Galaxy Galaxy_type;
 typedef NGML_Node Node_type;
 typedef NGML_Frame Frame_type;
 typedef NGML_Connectors Connectors_type;
 typedef NGML_Connection Connection_type;
 typedef NGML_Annotated_Connectors Annotated_Connectors_type;;
 typedef NGML_Graph Graph_type;
 typedef NGML_Document Document_type;
 typedef NGML_Root Root_type;

 enum class Type_Codes {
  #define DOMINION_TYPE DOMINION_TYPE_ENUM
  #include "dominion/types.h"
  #undef DOMINION_TYPE
 };

 template<typename T>
 Type_Codes get_type_code()
 {
 }
};

struct NGML_Galaxy : Node_Ptr_Default_Galaxy<NGML_Dominion>
{
};

struct NGML_Connectors : node_connectors<NGML_Dominion> {
  NGML_Connectors(QString label) : node_connectors<NGML_Dominion>(label){}
};

_RZNS(NGML)


#endif
