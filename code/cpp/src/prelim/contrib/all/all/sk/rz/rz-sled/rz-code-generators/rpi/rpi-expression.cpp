
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "rpi-expression.h"

#include "rpi-stage-form.h"

#include "rzns.h"

USING_RZNS(RZ_Core)
USING_RZNS(GVal)

RPI_Expression::RPI_Expression(RPI_Stage_Form& form)
  :  form_(form)
{

}

caon_ptr<RPI_Stage_Form> RPI_Expression::ptr_to_form()
{
 return &form_;
}


void RPI_Expression::write_as_statement(QTextStream* qts, caon_ptr<RPI_Stage_Form> prior)
{
 if(assignment_token_.isEmpty())
 {
  form_.write_statement_entry_qts();

  write(qts, prior);

  form_.write_statement_leave_qts(qts);

 }
 else
 {
  RPI_Assignment_Info& rai = form_.assignment_info();
  if(rai.vkind() == RPI_Assignment_Value_Kinds::Literal)
  {
   form_.write_assignment_initialization_via_token("parse-literal", prior);
  }
  else if(rai.vkind() == RPI_Assignment_Value_Kinds::Type_Default)
  {
   form_.write_assignment_initialization_via_token("type-default", prior);
  }
  else if(rai.vkind() == RPI_Assignment_Value_Kinds::Expression)
  {
   form_.write_assignment_initialization_via_expression(qts, prior);
  }
 }
}

QString RPI_Expression::encode_assignment_token()
{
 if(assignment_token_ == "\\=")
 {
  return "sng";
 }
 else if(assignment_token_ == "\\\\=")
 {
  return "dbl";
 }
 else
 {
  // // others? ...
  return "?";
 }
}

void RPI_Expression::mark_form_as_assignment_expression()
{
 form_.mark_as_assignment_expression();
}

void RPI_Expression::write(QTextStream* qts, caon_ptr<RPI_Stage_Form> prior)
{
 form_.write_unmediated(qts, prior);
}
