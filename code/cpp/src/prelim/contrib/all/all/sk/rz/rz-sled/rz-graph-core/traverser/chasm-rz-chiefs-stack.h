
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef CHASM_RZ_CHIEFS_STACK__H
#define CHASM_RZ_CHIEFS_STACK__H


#include "accessors.h"
#include "flags.h"

#include "global-types.h"

#include "relae-graph/relae-node-ptr.h"

#include <QStack>

#include <functional>

#include "rzns.h"


RZNS_(RZ_Core)


class ChasmRZ_Document;
class ChasmRZ_Node;
class ChasmRZ_Graph;
class ChasmRZ_Frame;
class ChasmRZ_Query;


class ChasmRZ_Chiefs_Stack
{
 QStack<caon_ptr<ChasmRZ_Node>> block_chiefs_;
 QStack<caon_ptr<ChasmRZ_Node>> chiefs_;

public:

 ChasmRZ_Chiefs_Stack();

 ACCESSORS__RGET(QStack<caon_ptr<ChasmRZ_Node>> ,block_chiefs)
 ACCESSORS__RGET(QStack<caon_ptr<ChasmRZ_Node>> ,chiefs)

 bool chiefs_empty()
 {
  return chiefs_.isEmpty();
 }

 bool block_chiefs_empty()
 {
  return block_chiefs_.isEmpty();
 }

 s4 chiefs_size()
 {
  return chiefs_.size();
 }

 s4 block_chiefs_size()
 {
  return block_chiefs_.size();
 }

 ChasmRZ_Node& get_current_chief();

 caon_ptr<ChasmRZ_Node> chiefs_top()
 {
  return chiefs_.top();
 }

 caon_ptr<ChasmRZ_Node> block_chiefs_top()
 {
  return block_chiefs_.top();
 }

 void push_chief(caon_ptr<ChasmRZ_Node> node);
 void push_block_chief(caon_ptr<ChasmRZ_Node> node);

 caon_ptr<ChasmRZ_Node> check_pop_chief();
 caon_ptr<ChasmRZ_Node> check_pop_block_chief();

 caon_ptr<ChasmRZ_Node> pop_chief();
 caon_ptr<ChasmRZ_Node> pop_block_chief();

};

_RZNS(RZ_Core)

#endif //CHASM_RZ_CHIEFS_STACK__H
