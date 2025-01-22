
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_ASG_VALUER__H
#define RZ_ASG_VALUER__H

#include "accessors.h"


#include "rz-graph-core/kernel/graph/chasm-rz-graph.h"
#include "rz-graph-core/kernel/query/chasm-rz-query.h"
#include "rz-graph-core/kernel/frame/chasm-rz-frame.h"

#include "rz-graph-core/kernel/chasm-rz-dominion.h"

#include "rz-graph-token/types/run-types.h"
#include "rz-graph-token/types/rz-type-variety.h"

#include "relae-graph/relae-caon-ptr.h"

#include "rz-code-elements/rz-code-statement.h"


#include <stack>

#include <map>

#include <functional>

#include "rzns.h"
#include "accessors.h"

class RZ_GCALL_Token;

RZNS_(RZ_Core)

class ChasmRZ_Graph;
class ChasmRZ_Node;

_RZNS(RZ_Core)

USING_RZNS(RZ_Core)


RZNS_(GBuild)

class RZ_ASG_Core_Runner;
class RZ_Type_Object;
class RZ_ASG_Visitor;
class RZ_ASG_Value_Holder;
class RZ_String_Phrase;
class RZ_ASG_Token;

_RZNS(GBuild)

USING_RZNS(GBuild)


RZNS_(GEmbed)

class RZ_Graph_Run_Embedder;

_RZNS(GEmbed)


USING_RZNS(GBuild)
USING_RZNS(GEmbed)


RZNS_(GRun)
_RZNS(GRun)

USING_RZNS(GRun)


USING_RZNS(RZClasp)


#ifndef type_is
#define type_is(T) \
 tv_type_is(RZ_Run_Types::T, type_object(), type_variety_)
#endif

RZNS_(GVal)

class RZ_Opaque_Type_Symbol;
class RZ_ASG_Symbol;
class RZ_ASG_Rvalue;
class RZ_ASG_Scope_Token;
class RZ_ASG_Logical_Scope;

class RZ_Phaon_User_Type;

class RZ_Code_Statement;

class RZ_ASG_Block_Info;


enum class RZ_ASG_Basic_Type_Groups {
 N_A, Opaque_Call, Opaque_Symbol, Evaluated_Symbol,
 Core_Function, Tuple, Function_Def_Info
};

struct RZ_ASG_Valuer_Core_Pair
{
 int generation;
 RZ_ASG_Core_Casement_Function* ccf;
 caon_ptr<ChasmRZ_Node> fnode;
 caon_ptr<ChasmRZ_Node> lhs_node;
 caon_ptr<ChasmRZ_Node> left_new_node;
 caon_ptr<ChasmRZ_Node> rhs_node;
 caon_ptr<ChasmRZ_Node> right_new_node;
 caon_ptr<ChasmRZ_Node> arity_value_node;
 caon_ptr<ChasmRZ_Node>* holding_fnode;
};

class RZ_ASG_Valuer
{
 RZ_ASG_Visitor& rz_asg_visitor_;
 caon_ptr<RZ_Graph_Run_Embedder> embedder_;

 RZ_Type_Variety type_variety_;

 ChasmRZ_Frame& Cf;
 ChasmRZ_Frame& Sf;
 const ChasmRZ_Query& Qy;

 QMap<ChasmRZ_Dominion::Type_Codes, caon_ptr<RZ_Type_Object>> type_objects_by_code_;


 typedef ChasmRZ_Node tNode;

 caon_ptr<tNode> the_monotail_;

 caon_ptr<RZ_ASG_Logical_Scope> current_logical_scope_;

 caon_ptr<RZ_ASG_Lexical_Scope> current_lexical_scope_;
 caon_ptr<RZ_ASG_Lexical_Scope> root_lexical_scope_;

 caon_ptr<RZ_ASG_Block_Info> current_block_info_;

 int current_core_pair_nodes_generation_;

 QMap<QString, caon_ptr<RZ_Phaon_User_Type>> user_type_names_;

public:

 QList<QVector<caon_ptr<tNode>>> core_pair_nodes_;

 typedef  QList<QVector<caon_ptr<tNode>>> core_pairs_nodes_type;

 void init_type_objects();
 void check_type_objects();
 void add_type_object(ChasmRZ_Dominion::Type_Codes code, caon_ptr<RZ_Type_Object> rto);


 typedef std::function<caon_ptr<RZ_ASG_Core_Casement_Function>(QString)>
  rz_asg_core_function_finder_type;


 enum Core_Function_Redirect_Codes {
  Core_Function_Code_Not_Recognized
  #define RZ_ASG_FUNCTION_DECLARE(rz_name, code, arity, ftype) ,code
  #include "rz-graph-run/functions/core-functions-valuer-n.h"
  #include "rz-graph-run/functions/core-functions-valuer-n-s.h"
//?  #include "rz-graph-run/functions/core/core-functions-valuer-n-s-s.h"
//?  #include "rz-graph-run/functions/core/core-functions-valuer-n-t.h"
  #include "rz-graph-run/functions/core-functions-valuer-rn.h"
  #include "rz-graph-run/functions/core-functions-valuer-cb.h"
  #include "rz-graph-run/functions/core-functions-valuer-dc.h"
  #undef RZ_ASG_FUNCTION_DECLARE
 };

 QMap<QString, Core_Function_Redirect_Codes> core_function_redirect_code_map_;

 enum Callback_Codes {
  Callback_Code_Not_Recognized
  #define RZ_ASG_FUNCTION_DECLARE(rz_name, code, arity, ftype) ,cb_##code
  #include "rz-graph-run/functions/core-functions-valuer-cb.h"
  #undef RZ_ASG_FUNCTION_DECLARE

  #define RZ_ASG_FUNCTION_DECLARE(rz_name, code, arity, ftype) ,dc_##code
  #include "rz-graph-run/functions/core-functions-valuer-dc.h"
  #undef RZ_ASG_FUNCTION_DECLARE
 };

 enum Set_Equal_Modes
 {
  Normal, Their, Via_Type, Their_Via_Type
 };

 QMap<QString, Callback_Codes> callback_code_map_;

 void init_core_function_redirect_code_map();
 void init_callback_code_map();
 void init_callback_defer_code_map();

 void add_statement_element_node(caon_ptr<tNode> node, caon_ptr<RZ_Code_Statement> st);

 void check_identify_statement_kind(RZ_Code_Statement::Statement_Kinds sk,
   QString check, caon_ptr<ChasmRZ_Node> fn);

 void check_function_def_map_sequence(caon_ptr<tNode> node,
   RZ_Function_Def_Info& fdi);

 QMap<QString, Callback_Codes> callback_defer_code_map_;

 caon_ptr<tNode> core_pair_function_node_;


public:


 ACCESSORS__RGET(RZ_Type_Variety ,type_variety)
 ACCESSORS__RGET(core_pairs_nodes_type ,core_pair_nodes)

 ACCESSORS(caon_ptr<RZ_Graph_Run_Embedder> ,embedder)

 ACCESSORS(caon_ptr<RZ_ASG_Lexical_Scope> ,root_lexical_scope)
 ACCESSORS__GET(caon_ptr<RZ_ASG_Lexical_Scope> ,current_lexical_scope)
 ACCESSORS__GET(caon_ptr<RZ_ASG_Logical_Scope> ,current_logical_scope)

 ACCESSORS(caon_ptr<tNode> ,core_pair_function_node)

 RZ_ASG_Valuer(RZ_ASG_Visitor& rz_asg_visitor,
  caon_ptr<RZ_Graph_Run_Embedder> = nullptr);

 rz_asg_core_function_finder_type rz_asg_core_function_finder;

 caon_ptr<RZ_ASG_Valuer_Core_Pair> check_release_core_pair();

 QString get_cpp_type_name(QString name);

 RZ_ASG_Basic_Type_Groups parse_basic_type_group(const RZ_ASG_Scope_Token& st);

 void redirect_core_function(RZ_ASG_Result_Holder& rh,
  QString function_name, tNode& start_node);

 void check_node_type(caon_ptr<tNode>& node);
 void check_monotail(caon_ptr<tNode>& node);

 void type_numeric_token(RZ_ASG_Token& tok);
 void type_string_token(RZ_ASG_Token& tok);
 void type_match_literal_token(RZ_ASG_Token& tok);

 void check_assignments();

 void add_file_to_process(RZ_ASG_Result_Holder& rh, RZ_Opaque_Type_Symbol& ots);
 void add_file_to_process(QString file);

 void init_scan_block(RZ_ASG_Result_Holder& rh,
  RZ_Opaque_Type_Symbol& ots);
 void init_scan_block(RZ_ASG_Result_Holder& rh, RZ_Opaque_Call& opc);
 void init_scan_block(RZ_ASG_Result_Holder& rh,
  caon_ptr<tNode> pre_entry_node, caon_ptr<tNode> start_node, caon_ptr<tNode> block_entry_node);

 void mark_core_function_call_entry(int generation,
   RZ_ASG_Core_Casement_Function& ccf,
   caon_ptr<tNode> start_node, caon_ptr<tNode> lhs_node,
   caon_ptr<tNode> left_new_node,
   caon_ptr<tNode> rhs_node, caon_ptr<tNode> right_new_node,
   caon_ptr<tNode> arity_value_node);


 void init_if_block(RZ_ASG_Result_Holder& rh, RZ_Opaque_Type_Symbol& ots);

 void init_if_block(RZ_ASG_Result_Holder& rh, RZ_ASG_Core_Casement_Function& ccf);
 void init_if_block(RZ_ASG_Result_Holder& rh, RZ_Opaque_Call& opc);
 void init_if_block(RZ_ASG_Result_Holder& rh,
  caon_ptr<tNode> pre_entry_node, caon_ptr<tNode> start_node, caon_ptr<tNode> block_entry_node);

 void init_elsif_block(RZ_ASG_Result_Holder& rh, RZ_ASG_Core_Casement_Function& ccf);
 void init_elsif_block(RZ_ASG_Result_Holder& rh, RZ_Opaque_Call& opc);
 void init_elsif_block(RZ_ASG_Result_Holder& rh,
  caon_ptr<tNode> pre_entry_node, caon_ptr<tNode> start_node, caon_ptr<tNode> block_entry_node);

 void init_else_block(RZ_ASG_Result_Holder& rh,
  caon_ptr<tNode> pre_entry_node, caon_ptr<tNode> block_entry_node);

 void init_caserun_block(RZ_ASG_Result_Holder& rh,
  caon_ptr<tNode> start_node, caon_ptr<tNode> arg_node, caon_ptr<tNode> block_entry_node);

 QString rename_function_name(RZ_ASG_Token& tok);

 void init_opaque_call(tNode& pre_entry_node, tNode& entry_node);

 bool check_embed_noop(caon_ptr<tNode> start_node);

 caon_ptr<tNode> check_node_type_with_entry_premise(const ChasmRZ_Connectors& premise,
  caon_ptr<tNode>& node);

 void check_node_value_proxy(caon_ptr<tNode>& node);

 caon_ptr<RZ_Type_Object> get_node_type_object(tNode& node);

 void enter_new_lexical_scope();
 void leave_lexical_scope();

 void enter_logical_scope(int level, RZ_ASG_Result_Holder& rh,
   RZ_Phaon_User_Type& uty);

 void leave_logical_scope(int level, RZ_ASG_Result_Holder& rh);

 void enter_logical_scope(int level, RZ_ASG_Result_Holder& rh,
   RZ_Opaque_Type_Symbol& ots);

 void set_preinit_equal_to_type(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& function_token,
  RZ_ASG_Value_Holder& lhs, RZ_ASG_Value_Holder& rhs);


 caon_ptr<RZ_ASG_Token> get_token_from(tNode& node);


 void set_token_as_symbol(caon_ptr<RZ_ASG_Token> token, caon_ptr<tNode> node);

 QString form_type_expression(caon_ptr<ChasmRZ_Node> entry_node);

 caon_ptr<tNode> register_user_precycle(QString name);

 caon_ptr<ChasmRZ_Node> register_lexical_symbol
  (RZ_ASG_Token& function_token, RZ_ASG_Token& tok, RZ_Opaque_Type_Symbol& ots,
   caon_ptr<RZ_ASG_Logical_Scope> logs = nullptr);

 caon_ptr<ChasmRZ_Node> register_lexical_symbol
  (RZ_ASG_Token& function_token, RZ_ASG_Token& tok, RZ_Opaque_Call& ots,
   caon_ptr<RZ_ASG_Logical_Scope> logs = nullptr);

 caon_ptr<ChasmRZ_Node> register_lexical_symbol
  (RZ_ASG_Result_Holder& rh, RZ_ASG_Token& function_token, RZ_ASG_Token& tok, RZ_ASG_Symbol& sym,
   caon_ptr<RZ_ASG_Logical_Scope> logs = nullptr);

 caon_ptr<ChasmRZ_Node> register_lexical_symbols
  (RZ_ASG_Result_Holder& rh, RZ_ASG_Token& function_token, RZ_ASG_Token& tok, RZ_ASG_Vector& symvec,
   caon_ptr<RZ_ASG_Logical_Scope> logs = nullptr);

 caon_ptr<ChasmRZ_Node> check_skip_symbol_dep_nodes(caon_ptr<ChasmRZ_Node> start_node, QStringList& qsl);

 void set_equal_via_type(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& function_token,
  RZ_ASG_Value_Holder& lhs, RZ_ASG_Value_Holder& rhs);

 void set_equal(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& function_token,
  RZ_ASG_Value_Holder& lhs, RZ_ASG_Value_Holder& rhs,
  Set_Equal_Modes mode = Normal);

 void set_returns_to_equal(RZ_ASG_Result_Holder& rh,
  RZ_ASG_Value_Holder& rhs);

 void set_preinit_to_equal(RZ_ASG_Result_Holder& rh,
  caon_ptr<tNode> n, RZ_ASG_Value_Holder& rhs);

 void set_preinit_to_equal(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& function_token,
  RZ_ASG_Value_Holder& lhs, RZ_ASG_Value_Holder& rhs);

 void init_do_block(RZ_ASG_Result_Holder& rh, RZ_Function_Def_Info& rfdi);

 void check_init_do_map_inner_block(tNode& n, caon_ptr<RZ_Clasp_Source_Element> el);
 caon_ptr<RZ_Clasp_Source_Element> init_do_map_inner_block(RZ_Function_Def_Info& rfdi, caon_ptr<RZ_Clasp_Source_Element> el);


 void set_preinit_equal_formula(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& function_token,
  RZ_ASG_Value_Holder& lhs, RZ_ASG_Value_Holder& rhs);

 void set_preinit_equal(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& function_token,
  RZ_ASG_Value_Holder& lhs, RZ_ASG_Value_Holder& rhs);

 void set_init_or_preinit_equal(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& function_token,
  RZ_ASG_Value_Holder& lhs, RZ_ASG_Value_Holder& rhs, caon_ptr<RZ_Opaque_Type_Symbol> otsl);


 void assign_to_type(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& function_token,
  RZ_ASG_Value_Holder& lhs, RZ_ASG_Value_Holder& rhs);

 void assign_overloadable(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& function_token,
  RZ_ASG_Value_Holder& lhs, RZ_ASG_Value_Holder& rhs);

 void preinit_assign_overloadable(RZ_ASG_Result_Holder& rh, RZ_ASG_Token& function_token,
  RZ_ASG_Value_Holder& lhs, RZ_ASG_Value_Holder& rhs);



 void set_symbol_equal(RZ_ASG_Result_Holder& rh, caon_ptr<tNode> n, RZ_ASG_Token& function_token,
  RZ_ASG_Token& tok, RZ_ASG_Value_Holder& vh, Set_Equal_Modes mode);

 void set_lexical_symbol_equal(RZ_ASG_Result_Holder& rh,
  caon_ptr<tNode> n, RZ_ASG_Token& function_token, RZ_ASG_Token& tok,
  RZ_ASG_Rvalue& rv, Set_Equal_Modes mode);

 caon_ptr<tNode> initialize_string_plex(ChasmRZ_Tuple_Info& rti, caon_ptr<tNode> first_node);

 caon_ptr<tNode> initialize_vector(ChasmRZ_Tuple_Info& rti, caon_ptr<tNode> first_node);
 caon_ptr<RZ_ASG_Valuer::tNode> initialize_data_entry(tNode& node);

 caon_ptr<tNode> retval_follow(caon_ptr<tNode> n);

 caon_ptr<RZ_Function_Def_Info> new_function_def_info(caon_ptr<ChasmRZ_Function_Def_Entry> fdef);



 QString node_to_string(tNode& n);
 QString value_to_string(QString& type, const RZ_ASG_Scope_Token& st);

 void insert_collection_node(tNode& new_node, tNode& lhs_node,
  ChasmRZ_Connectors& rhs_premise, tNode& rhs_node);

 void run_rename(RZ_ASG_Result_Holder& rh,
  QString function_name, tNode& function_node);
 void run_dc_rename(RZ_ASG_Result_Holder& rh,
  QString function_name, tNode& function_node);

 void run_deferred_callback(QString qs);

 void run_Debug_Hook();

 void run_Enum(RZ_ASG_Result_Holder& rh,
  tNode& start_node);

 void run_Else(RZ_ASG_Result_Holder& rh,
  tNode& start_node);

 void run_Caserun(RZ_ASG_Result_Holder& rh,
  tNode& start_node);

 void define_pass_proxy(caon_ptr<tNode> n1, caon_ptr<tNode> n2);
 void define_pass_proxy(RZ_ASG_Result_Holder& rh);

};

_RZNS(GVal)


#endif //RZ_ASG_VALUER__H
