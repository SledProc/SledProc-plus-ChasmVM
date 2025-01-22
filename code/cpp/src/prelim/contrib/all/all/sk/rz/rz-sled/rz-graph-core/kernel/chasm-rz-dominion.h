
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef CHASM_RZ_DOMINION__H
#define CHASM_RZ_DOMINION__H


#include "relae-graph/relae-node-ptr.h"
#include "relae-graph/relae-caon-ptr.h"

#include "rz-graph-sre/rz-read-table-state.h"

#include "rzns.h"



#define DOMINION_HIDE_NO_NAMESPACE
#define DOMINION_TYPE DOMINION_TYPE_DECLARE
#include "dominion/types.h"
#undef DOMINION_TYPE
#undef DOMINION_HIDE_NO_NAMESPACE


RZNS_(RZClasp)
 class RZ_Clasp_Source_Element;
_RZNS(RZClasp)

USING_RZNS(RZClasp)

RZNS_(RZ_Core)

struct ChasmRZ_Galaxy;
class ChasmRZ_Node;
class ChasmRZ_Connectors;
class ChasmRZ_Connection;
class ChasmRZ_Annotated_Connectors;
class ChasmRZ_Frame;
class ChasmRZ_Document;
class ChasmRZ_Graph;
class ChasmRZ_Root;
class RZ_SRE_Token;
class RZ_SRE_State;

struct ChasmRZ_Dominion
{
 typedef ChasmRZ_Galaxy Galaxy_type;
 typedef ChasmRZ_Node Node_type;
 typedef ChasmRZ_Frame Frame_type;
 typedef ChasmRZ_Connectors Connectors_type;
 typedef ChasmRZ_Connection Connection_type;
 typedef ChasmRZ_Annotated_Connectors Annotated_Connectors_type;
 typedef ChasmRZ_Document Document_type;
 typedef ChasmRZ_Graph Graph_type;
 typedef ChasmRZ_Root Root_type;

 enum class Type_Codes { N_A,
  #define DOMINION_TYPE DOMINION_TYPE_ENUM
  #include "dominion/types.h"
  #undef DOMINION_TYPE
 };

 template<typename T>
 Type_Codes get_type_code()
 {
 }

};



struct ChasmRZ_Galaxy : Node_Ptr_Default_Galaxy<ChasmRZ_Dominion>
{
 typedef RZ_SRE_Token SRE_Token_type;
 typedef RZ_SRE_State SRE_State_type;
 typedef RZ_Clasp_Source_Element Source_Element_type;

};

enum class ChasmRZ_Connectors_Case_Labels
{
 #define DOMINION_CONNECTOR(name, label) \
  name,
 #include "rz-graph-core/kernel/dominion/connectors.h"
 #undef DOMINION_CONNECTOR
};

struct ChasmRZ_Connectors : node_connectors<ChasmRZ_Dominion>
{
  ChasmRZ_Connectors(ChasmRZ_Connectors_Case_Labels cl = ChasmRZ_Connectors_Case_Labels::N_A,
    QString label = QString())
   : node_connectors<ChasmRZ_Dominion>(label), case_label(cl),
     priority(0), order(0){}
  ChasmRZ_Connectors_Case_Labels case_label;
  int priority;
  int order;
  bool operator<(const ChasmRZ_Connectors& rhs) const
  {
   return order < rhs.order;
  }
  operator bool() const
  {
   return order > 0;
  }
  bool operator==(const ChasmRZ_Connectors& rhs) const
  {
   return order == rhs.order;
  }
};

struct ChasmRZ_Annotated_Connectors :
  node_annotated_connectors<ChasmRZ_Dominion>
{
 ChasmRZ_Annotated_Connectors(const ChasmRZ_Connectors& conn,
   caon_ptr<ChasmRZ_Connection> cion)
   : node_annotated_connectors<ChasmRZ_Dominion>{conn, cion} {}

};


_RZNS(RZ_Core)


#endif //CHASM_RZ_DOMINION__H
