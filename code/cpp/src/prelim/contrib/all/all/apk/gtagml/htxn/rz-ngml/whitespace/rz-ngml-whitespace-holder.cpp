
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rz-ngml-whitespace-holder.h"

#include "rzns.h"

USING_RZNS(NGML)

NGML_Whitespace_Holder::NGML_Whitespace_Holder()
  : inherited_(nullptr)
{

}

void NGML_Whitespace_Holder::get_whitespace_counts_as_inherited(u1* result)
{
 if(inherited_)
   inherited_->ws().get_counts_as_inherited(result);
 else
 {
  result[2] = 0;
  result[3] = 0;
 }
}

void NGML_Whitespace_Holder::inherit_whitespace(NGML_Whitespace_Holder* wsh)
{
 inherited_ = wsh;
}


void NGML_Whitespace_Holder::attach_whitespace(QString whitespace)
{
 ws_ = NGML_Whitespace(whitespace);
}

