
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_COMPILER_FUNCTION__H
#define RZ_COMPILER_FUNCTION__H

#include "accessors.h"
#include "flags.h"


#include "code/chasm-rz-code-representation.h"

#include <QString>

#include <functional>

#include "chasm-rz-token.h"

#include "relae-graph/relae-caon-ptr.h"

#include "rzns.h"


RZNS_(RZ_Core)


class RZ_Compiler_Function
{
 QString name_;

public:

 ACCESSORS(QString ,name)

 RZ_Compiler_Function(QString name);

 QString token_label()
 {
  return "# %1"_qt.arg(name_);
 }

};

_RZNS(RZ_Core)

#endif //RZ_COMPILER_FUNCTION__H
