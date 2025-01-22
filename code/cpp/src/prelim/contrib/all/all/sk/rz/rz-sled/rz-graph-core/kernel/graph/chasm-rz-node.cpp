
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "chasm-rz-node.h"

#include "token/chasm-rz-token.h"

#include "rzns.h"

USING_RZNS(RZ_Core)

void ChasmRZ_Node::each_connection(std::function<void(const ChasmRZ_Connectors& connector,
 const ChasmRZ_Node&, const ChasmRZ_Connection* connection)> fn) const
{
 targets_iterator_type it(targets_);
 while(it.hasNext())
 {
  it.next();
  const ChasmRZ_Connectors& connector = *it.key();
  const ChasmRZ_Node& target = *it.value();
  fn(connector, target, nullptr);
 }

 annotated_targets_iterator_type ait(annotated_targets_);
 while(ait.hasNext())
 {
  ait.next();
  const ChasmRZ_Connectors& connector = *ait.key();
  const ChasmRZ_Connection* connection = ait.value().first.raw_pointer();
  const ChasmRZ_Node& target = *ait.value().second;
  fn(connector, target, connection);

 }

}


caon_ptr<RZ_ASG_Token> ChasmRZ_Node::asg_token()
{
 if(chasm_rz_token())
 {
  return chasm_rz_token()->asg_token();
 }
 return nullptr;
}

void ChasmRZ_Node::debug_connections()
{
 targets_iterator_type it(targets_);
 while(it.hasNext())
 {
  it.next();
  CAON_EVALUATE_DEBUG(ChasmRZ_Connectors ,key ,it.key())
  CAON_EVALUATE_DEBUG(ChasmRZ_Node ,value ,it.value())

 }
}

void ChasmRZ_Node::add_hyponode(caon_ptr<ChasmRZ_Node> n)
{
 hyponodes_.push_back(n);
}


void ChasmRZ_Node::swap_relation(const ChasmRZ_Connectors& connector,
 caon_ptr<ChasmRZ_Node> n1, caon_ptr<ChasmRZ_Node> n2)
{
 CAON_PTR_DEBUG(ChasmRZ_Node ,n1)
 CAON_PTR_DEBUG(ChasmRZ_Node ,n2)


 #ifdef NO_CAON
   ChasmRZ_Connectors* pc = const_cast<ChasmRZ_Connectors*>( &connector );
   targets_.remove(pc, n1);
   targets_.insert(pc, n2);
 #else
  targets_.remove(&connector, n1);
  targets_.insert(&connector, n2);
 #endif //NO_CAON

}


void ChasmRZ_Node::delete_relation(const ChasmRZ_Connectors& connector,
 caon_ptr<ChasmRZ_Node> n1)
{
 CAON_PTR_DEBUG(ChasmRZ_Node ,n1)
  #ifdef NO_CAON
    ChasmRZ_Connectors* pc = const_cast<ChasmRZ_Connectors*>( &connector );
    targets_.remove(pc, n1);
  #else
   targets_.remove(&connector, n1);
  #endif //NO_CAON
}

