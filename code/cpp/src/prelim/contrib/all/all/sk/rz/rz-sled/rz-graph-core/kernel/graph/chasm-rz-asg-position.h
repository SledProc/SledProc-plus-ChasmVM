
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef CHASM_RZ_ASG_POSITION__H
#define CHASM_RZ_ASG_POSITION__H

#include "accessors.h"
#include "flags.h"


#include "kernel/graph/chasm-rz-node.h"
#include "kernel/query/chasm-rz-query.h"

#include "tuple/chasm-rz-tuple-info.h"

#include "code/chasm-rz-function-def-kinds.h"

#include <QList>
#include <QStack>
#include <QMap>
#include <QVector>

#include "rzns.h"

RZNS_(RZ_Core)


class ChasmRZ_Token;
class ChasmRZ_Graph_Build;
class ChasmRZ_Sentence;
class ChasmRZ_Sentence_Part;
class ChasmRZ_Tile_Sentence_Handshake;
class ChasmRZ_Paralexeme;
class ChasmRZ_Connectors;

class ChasmRZ_ASG_Position
{
public:
 flags_(1)

  bool awaiting_function_def_note:1;
  bool pending_equalizer_value:1;
  bool possible_pending_equalizer_nested_entry:1;
  bool currently_implied_call_entry:1;
  bool if_block_pending_follow:1;
  bool elsif_block_pending_follow:1;
  bool active_type_indicator_node:1;
  bool holding_retval_node:1;
 _flags

private:

 enum class Position_States {
  Root, Active_Run_Casement_Node, Active_Run_Casement_Chief,

  Active_Run_Token, Active_Run_Chief,
  End_Of_Logical_Scope, Data_Entry,
  Run_Cross_Sequence, Run_Pchasm_rz_Entry, Cross_Run_Chief,
  Block_Entry, Cross_Block_Entry, Active_Closed_Do_Entry,



 };

 Position_States position_state_;
 ChasmRZ_Graph_Build* graph_build_;
 caon_ptr<ChasmRZ_Token>  active_run_token_;

 int current_depth_;

 QStack<caon_ptr<ChasmRZ_Node>> block_chiefs_;
 QStack<caon_ptr<ChasmRZ_Node>> chiefs_;
 QStack<caon_ptr<ChasmRZ_Node>> over_chiefs_;
 QStack<caon_ptr<ChasmRZ_Node>> block_chiefs_ifs_;

 caon_ptr<ChasmRZ_Node> current_tag_entry_node_;
 caon_ptr<ChasmRZ_Node> current_function_def_entry_node_;

 caon_ptr<ChasmRZ_Node> last_pchasm_rz_entry_node_;
 caon_ptr<ChasmRZ_Node> last_statement_entry_node_;
 caon_ptr<ChasmRZ_Node> last_do_map_block_statement_entry_node_;

 caon_ptr<ChasmRZ_Node> type_indicator_node_;

 caon_ptr<ChasmRZ_Node> current_closed_do_entry_node_;
 caon_ptr<ChasmRZ_Node> held_assignment_annotation_node_;

 caon_ptr<ChasmRZ_Call_Entry> current_closed_do_entry();

 void check_append_chief(caon_ptr<ChasmRZ_Node> new_chief);
 void check_pop_chief();

 void check_hold_closed_do_entry();

 caon_ptr<ChasmRZ_Node> pop_chief();

 void check_if_block_non_continue(caon_ptr<ChasmRZ_Token> token);
 void reset_if_block_pending_follow();


 ChasmRZ_Frame& Cf;
 ChasmRZ_Frame& Sf;

 const ChasmRZ_Query& Qy;

 caon_ptr<ChasmRZ_Node> current_node_;

 caon_ptr<ChasmRZ_Node> check_insert_function_def_entry_node(const ChasmRZ_Tuple_Info& tuple_info);

 caon_ptr<ChasmRZ_Node> insert_entry_node(const ChasmRZ_Connectors& connector, bool is_statement_entry,
  QString prefix = QString());

 caon_ptr<ChasmRZ_Node> insert_casement_entry_node(const ChasmRZ_Connectors& connector, bool is_statement_entry,
  QString prefix = QString());

 caon_ptr<ChasmRZ_Node> insert_block_entry_node(const ChasmRZ_Connectors& connector);
 caon_ptr<ChasmRZ_Node> held_equalizer_node_;
 caon_ptr<ChasmRZ_Node> held_mapkey_node_;
 caon_ptr<ChasmRZ_Node> held_do_mapkey_node_;

 caon_ptr<ChasmRZ_Node> held_retval_node_;
 caon_ptr<ChasmRZ_Node> held_retval_follow_node_;
 caon_ptr<ChasmRZ_Node> current_do_map_block_entry_node_;
 caon_ptr<ChasmRZ_Node> current_block_map_entry_node_;

 caon_ptr<ChasmRZ_Node> last_do_map_inner_block_first_entry_node_;

 caon_ptr<ChasmRZ_Node> last_function_definition_arrow_node_;

 void read_chiefs();
 void read_block_chiefs();
 void read_over_chiefs();

 void add_block_entry_node(caon_ptr<ChasmRZ_Node> block_entry_node);
 void finalize_overall_if_block();


 enum class Equalizer_Contexts {
  N_A, Number, String, Float, Ratio, Pointer, Function,
 };

 QPair<Equalizer_Contexts, QString> parse_equalizer_context(QString key);

 Equalizer_Contexts held_equalizer_context_;

public:

 ChasmRZ_ASG_Position(ChasmRZ_Graph_Build* graph_build);

 ACCESSORS(caon_ptr<ChasmRZ_Node> ,current_node)

 ChasmRZ_Node& get_current_chief();

 caon_ptr<ChasmRZ_Node> check_implied_lambda_tuple(ChasmRZ_Function_Def_Kinds kind);

 bool current_node_is_symbol_declaration();

 void add_equalizer_token_node(caon_ptr<ChasmRZ_Node> token_node);

 void add_string_plex_node(caon_ptr<ChasmRZ_Node> tinfo_node,
  caon_ptr<ChasmRZ_Node> string_plex_node);

 void add_block_map_entry();
 void add_block_map_leave();

 bool awaiting_statement_call_entry();

 void add_token_node(caon_ptr<ChasmRZ_Node> token_node);
 void add_residual_node(caon_ptr<ChasmRZ_Node> node);

 void add_arrow_node(caon_ptr<ChasmRZ_Node> token_node, ChasmRZ_Function_Def_Kinds kind);

 void hold_assignment_annotation_node(caon_ptr<ChasmRZ_Node> aa_node);

 void check_add_implied_my();

 caon_ptr<ChasmRZ_Node> current_entry_is_backquoted();
 caon_ptr<ChasmRZ_Node> check_add_function_def_entry_node_with_label();

 void add_arrow_token_node(caon_ptr<ChasmRZ_Node> token_node);

 void add_type_indicator(caon_ptr<ChasmRZ_Node> token_node);

 void complete_function_declaration(caon_ptr<ChasmRZ_Node> arrow_node,
                                    caon_ptr<ChasmRZ_Node> proxy_body_node);


 void check_add_retval_nodes(caon_ptr<ChasmRZ_Node> node);

 caon_ptr<ChasmRZ_Node> check_add_retval_nodes();

 void add_data_entry(caon_ptr<ChasmRZ_Node> tuple_info_node);
 void add_data_leave(caon_ptr<ChasmRZ_Node> tuple_info_node);

 caon_ptr<ChasmRZ_Tuple_Info> current_tuple_info();


 void add_raw_asg_token(caon_ptr<ChasmRZ_Node> token_node);

 void check_cancel_implied_call_entry();
 void check_add_implied_call_entry();

 void add_new_node_as_implied_call_entry();

 void add_casement_entry(caon_ptr<ChasmRZ_Node> entry_node, QString prefix = {});
 void add_casement_sequence(caon_ptr<ChasmRZ_Node> node);
 void add_casement_cross(caon_ptr<ChasmRZ_Node> entry_node);

 void add_call_entry(bool is_statement_entry, QString prefix = QString());
 void leave_expression();

 void close_statement();
 void leave_lexical_scope(int length, QString suffix);


 ChasmRZ_Tuple_Info::Tuple_Indicators data_chief_indicator();

 void hold_mapkey_node(caon_ptr<ChasmRZ_Node> node);
 void check_held_mapkey_node(caon_ptr<ChasmRZ_Node> new_node);
 void hold_do_mapkey_node(caon_ptr<ChasmRZ_Node> node);


 void hold_retval_node(caon_ptr<ChasmRZ_Node> node);

};


_RZNS(RZ_Core)

#endif //CHASM_RZ_ASG_POSITION__H

