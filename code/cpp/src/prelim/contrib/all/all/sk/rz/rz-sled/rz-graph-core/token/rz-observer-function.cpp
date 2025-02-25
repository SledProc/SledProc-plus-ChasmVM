
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "rz-observer-function.h"


USING_RZNS(RZ_Core)

RZ_Observer_Function::RZ_Observer_Function(QString name)
 : name_(name), asg_token_(nullptr)
{

}

void RZ_Observer_Function::init_asg_token()
{
 asg_token_ = new RZ_ASG_Token("[%1]"_qt.arg(name_));
}





