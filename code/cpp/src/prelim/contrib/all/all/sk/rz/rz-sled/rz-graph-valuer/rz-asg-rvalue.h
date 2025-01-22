
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_ASG_RVALUE__H
#define RZ_ASG_RVALUE__H

#include "accessors.h"


#include <stack>

#include <map>

#include <QTextStream>

#include "rzns.h"

#include "flags.h"

#include "accessors.h"

#include "rz-graph-token/rz-asg-value-holder.h"

#include <stack>

//RZNS_(GBuild)

//class RZ_ASG_Value_Holder;

//_RZNS(GBuild)

USING_RZNS(GBuild)

RZNS_(GVal)


class RZ_ASG_Rvalue
{

 RZ_ASG_Value_Holder& vh_;

public:

 ACCESSORS__GET(RZ_ASG_Value_Holder& ,vh)

 RZ_ASG_Rvalue(RZ_ASG_Value_Holder& vh);

};

_RZNS(GVal)



#endif //RZ_ASG_RVALUE__H
