
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef CHASM_RZ_CASEMENT_CALL_ENTRY__H
#define CHASM_RZ_CASEMENT_CALL_ENTRY__H

#include "accessors.h"
#include "flags.h"

#include "relae-graph/relae-caon-ptr.h"

#include <QString>
#include <QMap>

#include "rzns.h"

RZNS_(RZ_Core)

class ChasmRZ_Node;
class ChasmRZ_Connectors;

class ChasmRZ_Casement_Call_Entry
{

public:

 flags_(3)
  bool is_statement_entry:1;
  bool is_function_expression_entry:1;
  bool is_data_branch_entry:1;
  bool is_tuple_info_entry:1;
  bool is_empty_tuple:1;
  bool is_do_lambda:1;
  bool was_added_as_implied:1;
  bool has_label:1;
  bool was_added_as_pchasm_rz_ghost_block:1;
  bool may_precede_if_block:1;
  bool is_backquoted:1;
  bool is_unquoted:1;
  bool no_normalize:1;
  bool no_anticipate:1;
  bool has_do_map:1;
  bool may_precede_elsif_block:1;

  // // no others for now
  bool equalizer_context_number:1;
  bool equalizer_context_string:1;

  bool is_do_closed_with_follow:1;
  bool follows_closed_do:1;
  bool is_deferred:1;


 _flags




private:

 quint32 call_depth_ : 24;
 quint16 line_number_;
 quint16 file_id_;


 int call_id_;

 caon_ptr<ChasmRZ_Node> ref_node_;
 caon_ptr<ChasmRZ_Node> parent_entry_node_;


 caon_ptr<ChasmRZ_Node> self_node_;
 caon_ptr<ChasmRZ_Node> block_entry_node_;
 caon_ptr<ChasmRZ_Node> block_chief_node_;
 caon_ptr<ChasmRZ_Node> block_continue_node_;

 QString prefix_;

 // //   For debug only
 QString debug_text_hint_;

public:

 ChasmRZ_Casement_Call_Entry(int call_id, QString prefix = QString());

 ACCESSORS(caon_ptr<ChasmRZ_Node> ,ref_node)
 ACCESSORS(caon_ptr<ChasmRZ_Node> ,parent_entry_node)

 ACCESSORS(caon_ptr<ChasmRZ_Node> ,self_node)
 ACCESSORS(caon_ptr<ChasmRZ_Node> ,block_entry_node)
 ACCESSORS(caon_ptr<ChasmRZ_Node> ,block_chief_node)
 ACCESSORS(caon_ptr<ChasmRZ_Node> ,block_continue_node)

 ACCESSORS(QString ,prefix)
 ACCESSORS(QString ,debug_text_hint)

 ACCESSORS(quint16 ,call_depth)
 ACCESSORS(quint16 ,line_number)
 ACCESSORS(quint16 ,file_id)
 ACCESSORS__GET(int ,call_id)

 caon_ptr<ChasmRZ_Node> get_parent_entry_node_unwind();
 caon_ptr<ChasmRZ_Node> get_next_entry_node(caon_ptr<ChasmRZ_Casement_Call_Entry>& next_call_entry, int& unwind_count);
 caon_ptr<ChasmRZ_Casement_Call_Entry> unwind();
 caon_ptr<ChasmRZ_Casement_Call_Entry> unwind(int& unwind_count);


 caon_ptr<ChasmRZ_Node> get_next_entry_node();
 caon_ptr<ChasmRZ_Node> get_node();

 void debug_check_entry(const ChasmRZ_Connectors& conn);

};

_RZNS(RZ_Core)

#endif //CHASM_RZ_CASEMENT_CALL_ENTRY__H
