
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef RZ_NGML_TAG_COMMAND__H
#define RZ_NGML_TAG_COMMAND__H

#include <QString>

#include <functional>

#include "accessors.h"
#include "flags.h"

#include "whitespace/rz-ngml-whitespace-holder.h"

#include "global-types.h"

#include "rzns.h"

RZNS_(NGML)

class NGML_HTXN_Node;

class NGML_Tag_Command : public NGML_Whitespace_Holder
{
public:

 flags_(5)
  bool is_region:1;
  bool is_closed:1;
  bool is_html:1;
  bool is_self_closed:1;
  bool is_tag_assertion:1;
  bool is_tag_query:1;
  bool is_understood_auto_closed:1;
  bool has_main_tile:1;
  bool has_entry:1;
  bool has_attribute_tile:1;
 
  bool is_multi_parent:1;
  bool is_multi_optional:1;
  bool is_multi_mandatory:1;
 
  //bool is_multi_parent_semis:1;
  bool anticipate_semis:1;

  bool is_multi_parent_inherited:1;
//?  bool is_multi_parent_semis_inherited:1;
  bool anticipate_semis_inherited:1;


  bool multi_arg_layer:1;
  bool multi_main_layer:1;

  bool multi_arg_layer_inherited:1;
  bool multi_main_layer_inherited:1;

  bool autogen_multi_name:1;

  bool left_space_gap:1;
  bool right_space_gap:1;
  bool left_line_gap:1;
  bool right_line_gap:1;
  bool left_line_double_gap:1;
  bool right_line_double_gap:1;

  bool nonstandard_space:1;

  bool has_non_wrapped_space:1;
  bool is_non_wrapped:1;
  bool is_left_wrapped:1;

  bool is_main_layer:1;
  bool is_provisional_multi_parent:1;

  bool is_fiat:1;
  bool marked_main:1;

//?  bool is_provisional_multi_parent_semis:1;

 _flags

private:

 QString name_;
 QString argument_; 

 QString parent_tag_type_;

 u4 ref_position_;
 u4 ref_order_;

 NGML_HTXN_Node* ngml_htxn_node_;

 NGML_HTXN_Node* arg_ngml_htxn_node_;

 QList<NGML_HTXN_Node*> arg_ngml_htxn_nodes_;

public:

 ACCESSORS(QString ,name)
 ACCESSORS(QString ,parent_tag_type)
 ACCESSORS(QString ,argument)

 ACCESSORS(u4 ,ref_position)
 ACCESSORS(u4 ,ref_order)

 ACCESSORS(NGML_HTXN_Node* ,ngml_htxn_node)
 ACCESSORS(NGML_HTXN_Node* ,arg_ngml_htxn_node)

 NGML_Tag_Command(QString name, QString argument,
   QString parent_tag_type = QString());

 QString latex_name();

 void add_arg_ngml_htxn_node(NGML_HTXN_Node* nhn);

 void each_arg_ngml_htxn_node(std::function<void(NGML_HTXN_Node*)> fn);

 void normalize_whitespace();

 u2 get_whitespace_code();

};

_RZNS(NGML)

#endif
