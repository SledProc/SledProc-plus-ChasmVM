

//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_PHAON_PRECYCLE__H
#define RZ_PHAON_PRECYCLE__H

#include "accessors.h"
#include "flags.h"

#include <QString>
#include <QMap>


#include "rz-asg-scope-token.h"

#include "accessors.h"

#include "relae-graph/relae-caon-ptr.h"

#include "kans.h"

KANS_CLASS_DECLARE(Phaon ,PHR_Type_Object)

USING_KANS(Phaon)


#include "rzns.h"
RZNS_(GVal)

class RZ_Phaon_Precycle : QList<QPair<caon_ptr<PHR_Type_Object>, QString>>
{
public:
 RZ_Phaon_Precycle();

void add_type(caon_ptr<PHR_Type_Object> pto)
{
 push_back({pto, QString()});
}

};

_RZNS(GVal)

#endif // RZ_PHAON_USER_TYPE__H
