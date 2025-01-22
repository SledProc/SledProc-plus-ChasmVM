
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RPI_TYPE_DECLARATION__H
#define RPI_TYPE_DECLARATION__H

#include "accessors.h"
#include "flags.h"

#include <QString>
#include <QMap>

#include <QVector>

#include <QTextStream>

#include "rzns.h"

#include "relae-graph/relae-caon-ptr.h"

RZNS_(RZ_Core)
 class ChasmRZ_Node;
 class RE_Block_Entry;
_RZNS(RZ_Core)

USING_RZNS(RZ_Core)

RZNS_(GBuild)
 class RZ_Lisp_Graph_Visitor;
 class RZ_Lisp_Graph_Visitor_Dynamo;
_RZNS(GBuild)

USING_RZNS(GBuild)

RZNS_(GVal)

class RPI_Stage_Form;
class RZ_Lisp_Graph_Lexical_Scope;
class RZ_Function_Def_Syntax;
class RZ_Lisp_Graph_Block_Info;
class RPI_Block;


class RPI_Type_Declaration
{
 RPI_Stage_Form& form_;

 QString cmd_;

public:

 RPI_Type_Declaration(RPI_Stage_Form& form);

 ACCESSORS(QString ,cmd)

 void write(QTextStream* qts);

};

_RZNS(GVal)

#endif //RPI_TYPE_DECLARATION__H
