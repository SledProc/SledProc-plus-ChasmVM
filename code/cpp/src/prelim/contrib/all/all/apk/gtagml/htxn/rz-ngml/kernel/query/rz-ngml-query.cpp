
#include "rz-ngml-query.h"

#include "rzns.h"

USING_RZNS(NGML)

NGML_Query::NGML_Query()
 : node_query<NGML_Dominion>()
  #define DOMINION_CONNECTOR(name, label) \
    ,name(NGML_Connectors(#label))
  #include "kernel/dominion/connectors.h"
  #undef DOMINION_CONNECTOR
{


}

const NGML_Query& NGML_Query::instance()
{
 static NGML_Query* the_instance = nullptr;
 if(!the_instance)
  the_instance = new NGML_Query;
 return *the_instance;
}
