
#ifndef RZ_NGML_NODE__H
#define RZ_NGML_NODE__H

#include "relae-graph/relae-node-ptr.h"

#include "kernel/rz-ngml-dominion.h"

#include "kernel/frame/rz-ngml-frame.h"

#include "rzns.h"

RZNS_(NGML)

class NGML_Whitespace_Holder;

class NGML_Node : public node_ptr<NGML_Dominion>
{
public:
 #define DOMINION_TYPE DOMINION_NODE_CONSTRUCTOR
 #include "kernel/dominion/types.h"
 #undef DOMINION_TYPE


 QString debug_connectors() const;

 NGML_Whitespace_Holder* wsh()
 {
  return reinterpret_cast<NGML_Whitespace_Holder*>(vertex_.raw_pointer());
 }

 template<typename T>
 caon_ptr<T> as()
 {
  #ifdef NO_CAON
  //?return reinterpret_cast<T*>(vertex_);
  return caon_ptr<T> (reinterpret_cast<T*>(vertex_));
  #else //NO_CAON
  return vertex_.template caon_cast<T>();
  #endif

 }


};

_RZNS(NGML)


#endif
