
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "htxn-infoset-range.h"

USING_KANS(HTXN)

HTXN_Infoset_Range::HTXN_Infoset_Range(QString cl, u4 enter, u4 leave, u4 layer_id)
  :  classifier_(cl), enter_(enter), leave_(leave),
     layer_id_(layer_id), input_data_(nullptr), node_detail_(nullptr)
{

}

HTXN_Infoset_Range::HTXN_Infoset_Range(u4 enter, u4 leave, u4 layer_id)
 :  enter_(enter), leave_(leave), layer_id_(layer_id),
    input_data_(nullptr), node_detail_(nullptr)
{

}

void HTXN_Infoset_Range::add_connection(QString* label, HTXN_Infoset_Range* range)
{
 connections_[label] = range;
}

 
