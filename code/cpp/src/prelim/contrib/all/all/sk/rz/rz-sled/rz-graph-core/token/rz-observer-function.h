
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_OBSERVER_FUNCTION__H
#define RZ_OBSERVER_FUNCTION__H

#include "accessors.h"
#include "flags.h"


#include "code/chasm-rz-code-representation.h"

#include <QString>

#include <functional>

#include "chasm-rz-token.h"

#include "relae-graph/relae-caon-ptr.h"

#include "rzns.h"

RZNS_CLASS_DECLARE(GBuild ,RZ_ASG_Token)
USING_RZNS(GBuild)


RZNS_(RZ_Core)


class RZ_Observer_Function
{
 QString name_;
 caon_ptr<RZ_ASG_Token> asg_token_;

public:

 ACCESSORS(QString ,name)
 ACCESSORS__GET(caon_ptr<RZ_ASG_Token> ,asg_token)

 RZ_Observer_Function(QString name);

 QString token_label()
 {
  return "# %1"_qt.arg(name_);
 }

 void init_asg_token();

};

_RZNS(RZ_Core)

#endif //RZ_OBSERVER_FUNCTION__H
