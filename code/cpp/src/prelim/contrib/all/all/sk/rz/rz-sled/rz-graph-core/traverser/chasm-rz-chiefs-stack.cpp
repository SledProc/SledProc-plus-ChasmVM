
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "chasm-rz-chiefs-stack.h"


#include "textio.h"

USING_RZNS(RZ_Core)


USING_KANS(TextIO)

ChasmRZ_Chiefs_Stack::ChasmRZ_Chiefs_Stack()
{

}



ChasmRZ_Node& ChasmRZ_Chiefs_Stack::get_current_chief()
{
 return *chiefs_.top();
}


caon_ptr<ChasmRZ_Node> ChasmRZ_Chiefs_Stack::check_pop_chief()
{
 if(chiefs_.isEmpty())
 {
  return nullptr;
 }
 return pop_chief();
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Chiefs_Stack::check_pop_block_chief()
{
 if(block_chiefs_.isEmpty())
 {
  return nullptr;
 }
 return pop_block_chief();
}


caon_ptr<ChasmRZ_Node> ChasmRZ_Chiefs_Stack::pop_chief()
{
 return chiefs_.pop();
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Chiefs_Stack::pop_block_chief()
{
 return block_chiefs_.pop();
}


void ChasmRZ_Chiefs_Stack::push_chief(caon_ptr<ChasmRZ_Node> node)
{
 chiefs_.push(node);
}

void ChasmRZ_Chiefs_Stack::push_block_chief(caon_ptr<ChasmRZ_Node> node)
{
 block_chiefs_.push(node);
}


