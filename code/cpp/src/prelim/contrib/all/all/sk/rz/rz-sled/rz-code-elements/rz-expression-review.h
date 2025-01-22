
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_EXPRESSION_REVIEW__H
#define RZ_EXPRESSION_REVIEW__H

#include <QDebug>
#include <QStringList>

#include "flags.h"
#include "accessors.h"

#include "relae-graph/relae-caon-ptr.h"

#include "rzns.h"

RZNS_(RZ_Core)

class ChasmRZ_Node;

_RZNS(RZ_Core)

USING_RZNS(RZ_Core)


RZNS_(GVal)


class RZ_Expression_Review
{

private:

 QString text_;
 caon_ptr<ChasmRZ_Node> node_;

public:

 ACCESSORS(caon_ptr<ChasmRZ_Node> ,node)
 ACCESSORS(QString ,text)

 RZ_Expression_Review();

 void add_text(QString str);

};

_RZNS(GVal)

#endif //RZ_EXPRESSION_REVIEW__H

