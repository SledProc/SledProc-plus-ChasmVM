
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "chasm-rz-query.h"

#include "rzns.h"


USING_RZNS(RZ_Core)


ChasmRZ_Query::ChasmRZ_Query()
 : node_query<ChasmRZ_Dominion>()
  #define DOMINION_CONNECTOR(name, label) \
    ,name(ChasmRZ_Connectors(ChasmRZ_Connectors_Case_Labels::name, label))
  #include "kernel/dominion/connectors.h"
  #undef DOMINION_CONNECTOR
{
 int order = 0;
 #define DOMINION_CONNECTOR(name, label) \
  name.order = order; \
  ++order;
 #include "kernel/dominion/connectors.h"
 #undef DOMINION_CONNECTOR
}

const ChasmRZ_Query& ChasmRZ_Query::instance()
{
 static ChasmRZ_Query* the_instance = nullptr;
 if(!the_instance)
  the_instance = new ChasmRZ_Query;
 return *the_instance;
}
