
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "rpi-stage-element.h"
#include "rzns.h"


USING_RZNS(GVal)

RPI_Stage_Element::RPI_Stage_Element(RPI_Stage_Element_Kinds kind, QString text)
  :  kind_(kind), text_(text), form_(nullptr)
{

}

RPI_Stage_Element::RPI_Stage_Element()
  :  kind_(RPI_Stage_Element_Kinds::N_A), form_(nullptr)
{

}


RPI_Stage_Element::RPI_Stage_Element(caon_ptr<RPI_Stage_Form> form)
 :  kind_(RPI_Stage_Element_Kinds::Form), form_(form)
{

}
