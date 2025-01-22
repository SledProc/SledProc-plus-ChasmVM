
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RPI_STAGE_FORM__H
#define RPI_STAGE_FORM__H

#include "accessors.h"
#include "flags.h"

#include <QString>
#include <QMap>
#include <QVector>
#include <QPair>

#include <QTextStream>

#include "rzns.h"

#include "relae-graph/relae-caon-ptr.h"

#include "rpi-stage-element.h"
#include "rpi-assignment-info.h"

#include "phr-graph-core/kernel/graph/pgb-ir-build.h"

#include "multistep-token.h"

RZNS_(GBuild)
 class RZ_Lisp_Graph_Visitor;
 class RZ_Lisp_Graph_Visitor_Kauvir;
_RZNS(GBuild)

USING_RZNS(GBuild)

RZNS_(GVal)

class RPI_Block;
class RPI_Type_Declaration;
class RPI_Expression;
class RPI_Stage_Form_Annotation;
class RZ_Code_Statement;
class RZ_Expression_Review;

class RPI_Stage_Form
{
public:

 flags_(3)
  bool is_block_entry_statment:1;
  bool is_nested_block_entry_statment:1;
  bool write_type_declaration:1;
  bool is_inferred_block_entry_statment:1;
  bool is_statement:1;
  bool has_preceder_token:1;
  bool has_s1_token:1;

  bool prior_sibling_is_fground_token:1;
  bool prior_sibling_is_token:1;
  bool prior_sibling_is_expression:1;
  bool prior_sibling_is_block:1;

  bool write_push_expression:1;
  bool is_assignment_expression:1;

  bool is_fn_no_block:1;
  bool is_fn:1;
  bool is_deferred:1;
  bool has_instruction_token:1;
  bool infer_write_s0_statement:1;
 // bool has_instruction_token:1;

 _flags

private:


 PGB_IR_Build& pgb_;

 QList<PGB_IR_Build::Text_With_Purpose> step_forms_;

 QString fn_;

 QVector<RPI_Stage_Element> inner_elements_;

 typedef QVector<RPI_Stage_Element> inner_elements_type;

 caon_ptr<RPI_Stage_Form> parent_;

 caon_ptr<RPI_Block> plene_block_;

 caon_ptr<RPI_Block> first_nested_block_;

 caon_ptr<RPI_Stage_Form_Annotation> annotation_;

 int parent_lambda_position_;

 // // do we need this?
 int implicit_added_depth_;

 int nesting_level_;

 QString raw_text_;

 QString chief_;

 caon_ptr<RPI_Type_Declaration> type_declaration_;
 caon_ptr<RPI_Expression> expression_;

 RPI_Assignment_Info assignment_info_;

 caon_ptr<RZ_Code_Statement> code_statement_;
 caon_ptr<RZ_Expression_Review> expression_review_;

 QMap<caon_ptr<RPI_Stage_Form>, QPair<QString, int>> wrapped_inner_elements_;

 int hdcode_;

 QString s1_assignment_check_;

 QString prior_description_;

// void mark_unsurrounded_nested();
 void add_string_token(QString tok);

 RPI_Assignment_Info* get_parent_assignmnt_info();

 RPI_Stage_Element_Kinds last_element_kind();

 QString last_element_form_instruction();
 bool last_element_form_instruction(QString arg)
 {
  return last_element_form_instruction() == arg;
 }

 void init_prior_description();

public:

 RPI_Stage_Form(PGB_IR_Build& pgb, caon_ptr<RPI_Stage_Form> parent = nullptr);

 RPI_Stage_Form(PGB_IR_Build& pgb, caon_ptr<RPI_Block> block);

 ACCESSORS(QString ,fn)

 ACCESSORS__RGET(inner_elements_type ,inner_elements)
 ACCESSORS(caon_ptr<RPI_Stage_Form> ,parent)
 ACCESSORS(int ,parent_lambda_position)
 ACCESSORS(int ,implicit_added_depth)
 ACCESSORS(QString ,raw_text)
 ACCESSORS__RGET(RPI_Assignment_Info ,assignment_info)
 ACCESSORS(int ,nesting_level)
 ACCESSORS(caon_ptr<RZ_Code_Statement> ,code_statement)
 ACCESSORS(caon_ptr<RZ_Expression_Review> ,expression_review)
 ACCESSORS(QString ,s1_assignment_check)
 ACCESSORS(caon_ptr<RPI_Type_Declaration> ,type_declaration)

 ACCESSORS__RGET(QList<PGB_IR_Build::Text_With_Purpose> ,step_forms)

// bool s1_assignment_preempts_s0();

 QString instruction();

 bool instruction(QString sym)
 {
  return instruction() == sym;
 }

 bool is_effective_block_entry_statment();
 bool is_non_block_expression();
 bool is_deferred();

 void check_write_block_signature(QList<PGB_IR_Build::Text_With_Purpose>& step_forms);

 caon_ptr<RPI_Type_Declaration> type_declaration_on_block_entry();

 void set_prior_sibling_flags(caon_ptr<RPI_Stage_Form> prior);

 void write(QTextStream* qts, caon_ptr<RPI_Stage_Form> prior);
 void write_unmediated(QTextStream* qts, caon_ptr<RPI_Stage_Form> prior);
 void write_as_statement(QTextStream* qts, caon_ptr<RPI_Stage_Form> prior);

 void write_signature_type_declaration(QTextStream* qts);
 void write_type_declaration(QTextStream* qts);

 void write_statement_entry_qts();


 void write_fdef_entry();

 void write_assignment_initialization_via_token(QString key, caon_ptr<RPI_Stage_Form> prior);

 void write_assignment_initialization_via_expression(QTextStream* qts, caon_ptr<RPI_Stage_Form> prior);



 void add_expression_wrapper(caon_ptr<RPI_Stage_Form> form, QString text, int hdcode);

 QString get_s1_assignment_check();

 void init_type_declaration(QString cmd);
 void init_expression();

 void init_assignment_expression(QString tok);
 void init_formula_expression(QString tok);

 void write_statement_entry_qts(QTextStream* qts);
 void write_statement_leave_qts(QTextStream* qts);

 void write_statement_entry(QString node_name,
   QString prior_node_name, QString copy = QString());

 void write_cond_statement_entry(QString node_name,
   QString prior_node_name, QString alt_prior_node_name, QString copy = QString());


 void check_write_first_nested_is_assignment_leave(QTextStream* qts);

 void mark_deferred(int hdcode);
 void mark_as_assignment_expression();
 void mark_as_fn_no_block();
// void mark_as_s1_assignment_preempts_s0();
// void mark_as_parent_s1_assignment_preempts_s0();

 void mark_as_fn();

 QString get_assignment_target();

 void add_s1_fn_element(QString fn, QString obj);


// void check_init_annotation();
// void check_init_annotation(QString fn);
// void check_init_annotation_flags();

 void init_inferred_s0_statement();

 void add_expression(caon_ptr<RPI_Stage_Form> form);

 void add_nested_block(caon_ptr<RPI_Block> block);

 void write_checked_unmediated(QTextStream* qts, caon_ptr<RPI_Stage_Form> prior);

// void mark_parent_implict_end_form_before_nested_written();
// void mark_child_implict_end_form_before_nested_written();

//? bool implict_end_form_before_nested_written();

 void set_assignment_info(RPI_Assignment_Info& assignment_info);
 void set_assignment_info(RPI_Assignment_Info&& assignment_info)
 {
  set_assignment_info(assignment_info);
 }

 void add_assignment_initialization_element(RPI_Stage_Element_Kinds kind, QString text);
 void add_literal_element(RPI_Stage_Element_Kinds kind, QString text);
 void add_carrier_element(RPI_Stage_Element_Kinds kind, QString text);
 void add_kauvir_element(RPI_Stage_Element_Kinds kind, QString text);
 void add_fn_element(RPI_Stage_Element_Kinds kind, QString text);
 void add_bridge_element(RPI_Stage_Element_Kinds kind, QString text);
 void add_instruction_element(RPI_Stage_Element_Kinds kind, QString text);
 void add_argument_element(RPI_Stage_Element_Kinds kind, QString text);
 void add_insert_element(RPI_Stage_Element_Kinds kind, QString text);

};

_RZNS(GVal)

#endif //RPI_STAGE_FORM__H
