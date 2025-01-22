
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RPI_ASSIGNMENT_INFO__H
#define RPI_ASSIGNMENT_INFO__H

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


enum class RPI_Assignment_Value_Kinds
{
  N_A, Scoped_Symbol, Literal, Expression,
  Type, Type_Default, Ctor
};

enum class RPI_Assignment_Initialization_Kinds
{
  N_A, Init, Reinit,
};



class RPI_Assignment_Info
{
 RPI_Assignment_Value_Kinds vkind_;
 RPI_Assignment_Initialization_Kinds ikind_;
 QString text_;

public:

 RPI_Assignment_Info(RPI_Assignment_Value_Kinds vkind,
    RPI_Assignment_Initialization_Kinds ikind, QString text);
 RPI_Assignment_Info();

 ACCESSORS(RPI_Assignment_Value_Kinds ,vkind)
 ACCESSORS(RPI_Assignment_Initialization_Kinds ,ikind)
 ACCESSORS(QString ,text)

 bool has_text()
 {
  return !text_.isEmpty();
 }

 QString encode_ikind();

};

_RZNS(GVal)

#endif //RPI_ASSIGNMENT_INFO__H
