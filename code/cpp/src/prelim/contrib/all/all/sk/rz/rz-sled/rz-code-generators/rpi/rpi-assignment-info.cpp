
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "rpi-assignment-info.h"
#include "rzns.h"


USING_RZNS(GVal)

RPI_Assignment_Info::RPI_Assignment_Info(RPI_Assignment_Value_Kinds vkind,
  RPI_Assignment_Initialization_Kinds ikind, QString text)
  :  vkind_(vkind),  ikind_(ikind), text_(text)
{

}


QString RPI_Assignment_Info::encode_ikind()
{
 switch (ikind_)
 {
 case RPI_Assignment_Initialization_Kinds::Init:
  return "\\=";
 case RPI_Assignment_Initialization_Kinds::Reinit:
  return "\\\\=";
 default:
  return "?";
 }
}

RPI_Assignment_Info::RPI_Assignment_Info()
  :  vkind_(RPI_Assignment_Value_Kinds::N_A),
     ikind_(RPI_Assignment_Initialization_Kinds::N_A)
{

}

