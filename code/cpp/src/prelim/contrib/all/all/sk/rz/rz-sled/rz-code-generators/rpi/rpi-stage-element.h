
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RPI_STAGE_ENEMENT__H
#define RPI_STAGE_ENEMENT__H

#include "accessors.h"
#include "flags.h"

#include <QString>
#include <QMap>

#include <QTextStream>

#include "rzns.h"

#include "relae-graph/relae-caon-ptr.h"

#include "rz-function-def/rz-function-def-syntax.h"



RZNS_(GBuild)
 class RZ_Lisp_Graph_Visitor;
 class RZ_Graph_Visitor_Phaon;
_RZNS(GBuild)

USING_RZNS(GBuild)


RZNS_(PhrGraphCore)
 class PGB_IR_Build;
_RZNS(PhrGraphCore)

USING_RZNS(PhrGraphCore)

RZNS_(GVal)

class RPI_Stage_Form;

enum class RPI_Stage_Element_Kinds
{
 N_A, FGround_Symbol, S1_FGround_Symbol, Token, Form, S1_Symbol,
 Type_Default_Marker,

 Instruction_Symbol, Literal, String_Literal, Kernel_Type_Symbol,
 Raw_Symbol, Bridge_Symbol, Field_Index_Key,
};

class RPI_Stage_Element
{
 caon_ptr<RPI_Stage_Form> form_;
 RPI_Stage_Element_Kinds kind_;
 QString text_;

public:

 RPI_Stage_Element(RPI_Stage_Element_Kinds kind, QString text);
 RPI_Stage_Element(caon_ptr<RPI_Stage_Form> form);
 RPI_Stage_Element();

 ACCESSORS(RPI_Stage_Element_Kinds ,kind)
 ACCESSORS(QString ,text)
 ACCESSORS(caon_ptr<RPI_Stage_Form> ,form)
};

_RZNS(GVal)

#endif //RPI_STAGE_ENEMENT__H
