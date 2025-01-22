
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "rz-asg-rvalue.h"

#include "rzns.h"

USING_RZNS(GVal)

RZ_ASG_Rvalue::RZ_ASG_Rvalue(RZ_ASG_Value_Holder& vh)
 : vh_(vh)
{
}

