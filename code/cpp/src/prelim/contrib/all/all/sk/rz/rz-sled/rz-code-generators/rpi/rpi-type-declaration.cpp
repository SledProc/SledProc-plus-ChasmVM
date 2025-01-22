
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "rpi-type-declaration.h"
#include "rpi-stage-form.h"


#include "rzns.h"

USING_RZNS(RZ_Core)
USING_RZNS(GVal)

RPI_Type_Declaration::RPI_Type_Declaration(RPI_Stage_Form& form)
  :  form_(form)
{

}

void RPI_Type_Declaration::write(QTextStream* qts)
{
// qts << "\n\n;;type decl \n";
 form_.write_checked_unmediated(qts, nullptr);
}

