
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RPI_EXPRESSION__H
#define RPI_EXPRESSION__H

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
 class RZ_Graph_Visitor_Phaon;
_RZNS(GBuild)

USING_RZNS(GBuild)

RZNS_(GVal)

class RPI_Stage_Form;
class RZ_Lisp_Graph_Lexical_Scope;
class RZ_Function_Def_Syntax;
class RZ_Lisp_Graph_Block_Info;
class RPI_Block;


class RPI_Expression
{
 RPI_Stage_Form& form_;

 QString assignment_token_;


public:

 RPI_Expression(RPI_Stage_Form& form);

 ACCESSORS(QString ,assignment_token)


 void write(QTextStream* qts, caon_ptr<RPI_Stage_Form> prior);
 void write_as_statement(QTextStream* qts, caon_ptr<RPI_Stage_Form> prior);

 caon_ptr<RPI_Stage_Form> ptr_to_form();

 QString encode_assignment_token();

 void mark_form_as_assignment_expression();


};

_RZNS(GVal)

#endif //RPI_EXPRESSION__H
