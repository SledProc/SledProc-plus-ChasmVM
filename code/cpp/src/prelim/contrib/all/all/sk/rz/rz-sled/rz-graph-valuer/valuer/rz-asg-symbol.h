
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_ASG_SYMBOL__H
#define RZ_ASG_SYMBOL__H

#include "accessors.h"
#include "flags.h"


#include "relae-graph/relae-caon-ptr.h"

#include <QString>

#include "rzns.h"

RZNS_(RZ_Core)

class ChasmRZ_Node;

_RZNS(RZ_Core)

USING_RZNS(RZ_Core)


RZNS_(GVal)

class RZ_ASG_Symbol
{
private:

 caon_ptr<ChasmRZ_Node> node_;

public:

 ACCESSORS(caon_ptr<ChasmRZ_Node> ,node)

 RZ_ASG_Symbol(caon_ptr<ChasmRZ_Node> n);

 template<typename T>
 friend void operator<<(T& t, RZ_ASG_Symbol& sym)
 {}

};

_RZNS(GVal)

#endif //RZ_ASGP_SYMBOL__H
