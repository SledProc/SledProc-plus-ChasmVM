
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_LISP_GRAPH_VISITOR__H
#define RZ_LISP_GRAPH_VISITOR__H

#include "accessors.h"

#include "flags.h"

#include "rz-graph-sre/rz-read-table-state.h"


#include "rz-graph-core/kernel/chasm-rz-dominion.h"
#include "rz-graph-core/kernel/query/chasm-rz-query.h"
#include "rz-graph-core/kernel/frame/chasm-rz-frame.h"

#include "relae-graph/relae-caon-ptr.h"

#include "rz-graph-token/types/run-types.h"

#include "rz-graph-token/token/token-kinds.h"

#include "rz-graph-valuer/scope/rz-asg-lexical-scope.h"

#include "rz-asg-visitor-run-state.h"

#include <stack>

#include <map>

#include "rzns.h"


RZNS_(RZ_Core)

class ChasmRZ_Graph;
class ChasmRZ_Node;
class ChasmRZ_Call_Entry;
class ChasmRZ_Casement_Call_Entry;
class RZ_Cpp_Project;
class RZ_SChasmRZ_Token;

_RZNS(RZ_Core)

USING_RZNS(RZ_Core)


RZNS_(GRun)

class RZ_Graph_Run_Token;

_RZNS(GRun)


RZNS_(GBuild)


class RZ_ASG_Core_Casement_Function;
class RZ_ASG_Token;


_RZNS(GBuild)

USING_RZNS(GBuild)


RZNS_(GEmbed)

class RZ_Graph_Run_Embedder;
class RZ_Graph_Embed_Token;
class RZ_Graph_Cpp_Token;
class RZ_Graph_Embed_Run_Valuer;

_RZNS(GEmbed)

USING_RZNS(GEmbed)


RZNS_(GVal)

class RZ_ASG_Valuer;
class RZ_Dynamo_Output;

_RZNS(GVal)

USING_RZNS(GVal)

//RZNS_(RZClasp)

// class RZ_Clasp_Source_Element;

//_RZNS(RZClasp)

//USING_RZNS(RZClasp)



RZNS_(GRun)

class RZ_ASG_Runner;

_RZNS(GRun)

USING_RZNS(GRun)

RZNS_(GBuild)

class RZ_ASG_Visitor_Run_Plugin;


class RZ_ASG_Visitor
{
public:

 typedef ChasmRZ_Node tNode;

 enum class Next_Node_Premise
 {
  N_A, Normal, Expression, Function_Def_Entry,
  Token_To_Block_Entry, Block_Entry, Block_Continue
 };

private:

 caon_ptr<ChasmRZ_Graph> graph_;

 const ChasmRZ_Query& Qy;

 ChasmRZ_Frame& Cf;
 ChasmRZ_Frame& Sf;

 caon_ptr<ChasmRZ_Node> current_block_node_;
 caon_ptr<ChasmRZ_Node> data_continue_node_;


 caon_ptr<RZ_ASG_Valuer> valuer_;
 caon_ptr<RZ_Dynamo_Output> dynamo_output_;

 RZ_ASG_Lexical_Scope& current_lexical_scope_;

 std::stack<caon_ptr<ChasmRZ_Node>> entry_nodes_;

 caon_ptr<RZ_Graph_Embed_Run_Valuer> run_valuer_;

 caon_ptr<RZ_ASG_Runner> asg_runner_;



 std::map<QString, caon_ptr<RZ_ASG_Core_Casement_Function>> core_function_map_;
 void reset_call_entry_object(RZ_Read_Table_State state_on_unwind,
                              RZ_Read_Table_Post_Advance_State post_state_on_unwind,
                              RZ_Read_Table_State state_on_no_unwind,
                              RZ_Read_Table_Post_Advance_State post_state_on_no_unwind);

 RZ_ASG_Visitor_Run_State run_state_;
 caon_ptr<RZ_ASG_Visitor_Run_Plugin> current_run_plugin_;

 void normalize_nested_data(tNode& previous_node,
   tNode& data_node, const ChasmRZ_Connectors& qtok);


 typedef QPair<caon_ptr<RZ_ASG_Lexical_Scope>, QString> Current_Lex_Iterator_type;
 caon_ptr<Current_Lex_Iterator_type> current_lex_iterator_;

 QMap<caon_ptr<ChasmRZ_Call_Entry>, caon_ptr<Current_Lex_Iterator_type>> lex_iterators_;

 void check_token_node_type(caon_ptr<RZ_ASG_Token> tok, caon_ptr<tNode> token_node);

 caon_ptr<ChasmRZ_Node> find_block_entry_as_sequence();

 QString document_directory_;

 void insert_core_function(QString name,
  caon_ptr<RZ_ASG_Core_Casement_Function>);


public:

 RZ_ASG_Visitor(caon_ptr<ChasmRZ_Graph> graph = nullptr);

 ACCESSORS(caon_ptr<ChasmRZ_Graph> ,graph)
 ACCESSORS(caon_ptr<RZ_Graph_Embed_Run_Valuer> ,run_valuer)
 ACCESSORS__RGET(RZ_ASG_Visitor_Run_State ,run_state)

 ACCESSORS(caon_ptr<RZ_ASG_Valuer> ,valuer)

 ACCESSORS(caon_ptr<RZ_Dynamo_Output> ,dynamo_output)

 ACCESSORS(caon_ptr<Current_Lex_Iterator_type> ,current_lex_iterator)

 ACCESSORS(QString ,document_directory)

 caon_ptr<ChasmRZ_Block_Entry> current_node_as_block_entry();

 caon_ptr<tNode> graph_root_node();

 caon_ptr<RZ_ASG_Lexical_Scope> current_lexical_scope()
 {
  return &current_lexical_scope_;
 }

 void deactivate();
 void activate();

 int run_core_pairs(int generation);

 caon_ptr<tNode> check_raw_asg(caon_ptr<tNode> node);

 void add_initial_output_text(QString text);

 bool quasi_statements();

 void write_core_pairs(int generation, QString& text);
 void run_core_pair(RZ_ASG_Valuer_Core_Pair& pr);

 void check_anticipate(int generation, RZ_ASG_Result_Holder& rh,
   tNode& start_node);

 void deactivate_run_state_plugin();

 void find_block_entry();

 caon_ptr<tNode> nested_block_entry_from_node(caon_ptr<tNode> node);
 caon_ptr<tNode> call_entry_from_node(caon_ptr<tNode> node);

 QString get_field_index_key(caon_ptr<ChasmRZ_Node> n, QString sym);


 caon_ptr<tNode> find_run_block_entry(caon_ptr<tNode> node, caon_ptr<tNode>* call_entry_node = nullptr);
 caon_ptr<tNode> find_run_call_entry(caon_ptr<tNode> node);

 void check_redirect(RZ_ASG_Result_Holder& rh, caon_ptr<tNode> n);

 void prepare_rz_path_handlers_output(QString handlers);

 // maybe instead of QString& explanation should use the connector directly ...
 caon_ptr<tNode> find_do_map_block_continue_node(caon_ptr<tNode> node, caon_ptr<tNode>& call_entry_node_or_parent_node,
   QString& explanation);

 caon_ptr<tNode> get_call_sequence_node(caon_ptr<tNode> node);

 caon_ptr<RZ_ASG_Block_Info> get_block_info_from_function_node(caon_ptr<ChasmRZ_Node> node);

 void take_project(caon_ptr<RZ_Cpp_Project> project);

 QString get_mapkey_string(caon_ptr<ChasmRZ_Node> node);

 bool current_call_entry_is_function_expression();
 bool check_embed_noop(caon_ptr<tNode> n);

 caon_ptr<tNode> find_data_leave(tNode& n);


 void find_run_entry(RZ_Read_Table_State state_not_found, RZ_Read_Table_Post_Advance_State post_state_not_found);
 void find_next_token(RZ_Read_Table_State state_not_found, RZ_Read_Table_Post_Advance_State post_state_not_found);
 void find_continue_token(RZ_Read_Table_State state_not_found, RZ_Read_Table_Post_Advance_State post_state_not_found);

 caon_ptr<tNode> check_type_indicator(caon_ptr<tNode> node);


 caon_ptr<tNode> find_next_token(caon_ptr<tNode> start_node, RZ_SRE_Result_State& result_state,
   bool expression_flag, bool non_sequence = false);


 caon_ptr<tNode> find_expression_continue(caon_ptr<tNode> start_node, RZ_Read_Table_State& result_state,
   RZ_Read_Table_Advance_Token_State& token_state, bool expression_flag);
 void valuer_redirect(RZ_ASG_Result_Holder& rh,
  caon_ptr<RZ_ASG_Core_Casement_Function> ccf, caon_ptr<tNode> start_node);

 QString type_expression_from_node(caon_ptr<tNode> node);

 void normalize(tNode& start_node);

 void hyper_normalize();
 void hyper_normalize(tNode& start_node);

 caon_ptr<tNode> embed_redirect_node();
 caon_ptr<tNode> find_block_continue(caon_ptr<tNode> node);

 caon_ptr<tNode> normalize_run_call(int depth, int pos, tNode& pre_entry_node, tNode& start_node,
  const ChasmRZ_Connectors& qtok, caon_ptr<ChasmRZ_Call_Entry> current_call_entry = nullptr,
  caon_ptr<ChasmRZ_Call_Entry> carried_call_entry = nullptr,
  caon_ptr<tNode>* node_to_change = nullptr);

 caon_ptr<tNode> normalize_run_casement(int depth, int pos, tNode& pre_entry_node, tNode& start_node,
  const ChasmRZ_Connectors& qtok, caon_ptr<ChasmRZ_Call_Entry> current_call_entry = nullptr,
  caon_ptr<ChasmRZ_Call_Entry> carried_call_entry = nullptr,
  caon_ptr<tNode>* node_to_change = nullptr);





 caon_ptr<tNode> hyper_normalize_run_call(caon_ptr<ChasmRZ_Call_Entry> rce, tNode& start_node,
  const ChasmRZ_Connectors& qtok);





 caon_ptr<tNode> check_normalize_infix_run_call(caon_ptr<ChasmRZ_Call_Entry> rce, caon_ptr<ChasmRZ_Call_Entry> crce, int depth, tNode& pre_entry_node,
   tNode& start_node, const ChasmRZ_Connectors& qtok);

 caon_ptr<tNode> find_effective_call_entry(caon_ptr<tNode> sn);

 caon_ptr<tNode> check_normalize_infix_run_call(caon_ptr<ChasmRZ_Call_Entry> rce, caon_ptr<ChasmRZ_Call_Entry> crce, int depth, tNode& pre_entry_node,
   tNode& start_node,
   tNode& current_node, const ChasmRZ_Connectors& qtok);

 QString identify_function(QString name, caon_ptr<RZ_ASG_Core_Casement_Function>& ccf);

 void normalize_block(tNode& pre_entry_node, tNode& start_node,
  const ChasmRZ_Connectors& qtok,
  caon_ptr<tNode>* node_to_change = nullptr);

 void hyper_normalize_block(tNode& start_node,
  const ChasmRZ_Connectors& qtok);

 caon_ptr<tNode> get_data_entry(caon_ptr<tNode> node);

 void pop_embed_branch_node();

 void check_cross_do(caon_ptr<ChasmRZ_Node> n);

 void normalize_nested_run_call(caon_ptr<ChasmRZ_Call_Entry> carried_rce, int depth, tNode& function_node);

 caon_ptr<RZ_ASG_Core_Casement_Function> find_core_function(QString name);

 void swap_function_nodes(tNode&  pre_entry_node, tNode&  head_node,
   tNode& previous_node, tNode&  function_node, const ChasmRZ_Connectors& qtok);

 caon_ptr<tNode> normalize_nested_run_call_continuation(int depth, int pos, caon_ptr<tNode> previous_node,
   caon_ptr<tNode> function_node);

 caon_ptr<tNode> normalize_nested_block(caon_ptr<tNode> previous_node,
   caon_ptr<tNode> function_node, const ChasmRZ_Connectors& qtok);

 caon_ptr<tNode> find_call_entry(const tNode& node, caon_ptr<ChasmRZ_Call_Entry>& rce);
 caon_ptr<tNode> find_data_entry(const tNode& node, caon_ptr<ChasmRZ_Call_Entry>& rce);


 void check_function_symbol(caon_ptr<tNode> function_node);
 void note_block_entry(caon_ptr<tNode> function_node);
 void set_token_type_object(caon_ptr<RZ_ASG_Token> token, RZ_Run_Types::Enum);

 void anticipate(std::function<void(RZ_Dynamo_Output&)> fn);
 void anticipate(tNode& start_node);

 void anticipate_block(tNode& start_node);


 caon_ptr<tNode> anticipate_run_call(tNode& start_node);
 void anticipate_nested_run_calls(tNode& start_node, RZ_ASG_Result_Holder& rh);

 void check_asg_runner();

 void find_next_node_from_entry(tNode& node);

 caon_ptr<RZ_ASG_Core_Casement_Function> normalize_continue_run_call(caon_ptr<tNode> pass_node,
  caon_ptr<RZ_ASG_Token> tok, caon_ptr<tNode> function_node);
 void pre_interpret(caon_ptr<tNode> start_node);
 void init_core_functions();

 void normalize();

 caon_ptr<ChasmRZ_Node> get_cross_sequence_node(caon_ptr<ChasmRZ_Node> node);

 caon_ptr<tNode> embed_redirect_node(caon_ptr<tNode> node);
 caon_ptr<RZ_Graph_Embed_Token> current_embed_rename_token(caon_ptr<tNode> node);

 caon_ptr<tNode> nested_block_entry_from_prior_node(caon_ptr<tNode> node);
 caon_ptr<RZ_ASG_Block_Info> current_block_info(caon_ptr<tNode> node);

 caon_ptr<tNode> entry_from_call_entry(caon_ptr<tNode> node);

 caon_ptr<tNode> data_entry_from_call_entry(caon_ptr<tNode> node);

 caon_ptr<ChasmRZ_Node> get_code_statement_node_from_statement_entry_node(
   caon_ptr<ChasmRZ_Node> statement_entry_node);

 caon_ptr<ChasmRZ_Node> get_expression_review_node_from_entry_node(caon_ptr<ChasmRZ_Node> entry_node);

 caon_ptr<ChasmRZ_Node> get_assignment_annotation_node_from_statement_entry_node(
    caon_ptr<ChasmRZ_Node> statement_entry_node);

 caon_ptr<tNode> find_block_continue_node(caon_ptr<tNode> node, caon_ptr<tNode>& block_entry_node);

 caon_ptr<tNode> find_block_entry_node(caon_ptr<tNode> node, caon_ptr<RZ_ASG_Block_Info>& rbi);

 QString call_entry_label(caon_ptr<ChasmRZ_Call_Entry> rce);
 QString call_entry_label(caon_ptr<ChasmRZ_Node> n);

 Next_Node_Premise get_next_node(caon_ptr<ChasmRZ_Node> start_node, caon_ptr<ChasmRZ_Node>& result);
 Next_Node_Premise get_cross_node(caon_ptr<ChasmRZ_Node> start_node, caon_ptr<ChasmRZ_Node>& result);

};

_RZNS(GBuild)




#endif //RZ_LISP_GRAPH_VISITOR__H
