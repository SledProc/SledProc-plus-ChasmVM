
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef RZ_FUNCTION_DEF_INFO__H
#define RZ_FUNCTION_DEF_INFO__H

#include "accessors.h"

#include "flags.h"

#include <functional>

#include <QString>

#include "rz-graph-core/code/chasm-rz-function-def-entry.h"

#include "relae-graph/relae-caon-ptr.h"

#include "phr-graph-core/kernel/graph/pgb-ir-build.h"


#include "rzns.h"

#include "flags.h"

#include <QStack>

RZNS_(PhrGraphCore)
 class PGB_IR_Build;
_RZNS(PhrGraphCore)

USING_RZNS(PhrGraphCore)

RZNS_(RZ_Core)

class ChasmRZ_Node;
class ChasmRZ_Function_Def_Entry;
class ChasmRZ_Query;

_RZNS(RZ_Core)

USING_RZNS(RZ_Core)

RZNS_(GBuild)

class RZ_ASG_Token;

_RZNS(GBuild)

USING_RZNS(GBuild)


RZNS_(GVal)

class RZ_ASG_Valuer;
class RZ_Function_Def_Syntax;
class RZ_ASG_Lexical_Scope;

class RZ_Function_Def_Info
{
public:
 flags_(2)
  bool lambda:1;
  bool type_expression:1;
  bool type_symbol_assignment:1;
  bool monad:1;
  bool with_matching:1;
  bool no_impl:1;
  bool lexical_lambda:1;
  bool logical_lambda:1;
  bool do_lambda:1;
  bool no_def:1;
  bool async:1;
  bool has_preceding:1;
 _flags

 enum class Channel_Types {
  N_A, Lambda, Sigma, Error, Return, Context, CTOR_Mem, CTOR_Ret, Monad
 };

private:

 typedef ChasmRZ_Node tNode;

 typedef std::function<QString(QString)> type_name_callback_type;
 typedef std::function<void(RZ_ASG_Lexical_Scope&, QString, QString)> ls_callback_type;


 caon_ptr<ChasmRZ_Function_Def_Entry> function_def_entry_;
 type_name_callback_type type_name_callback_;
 ls_callback_type ls_callback_;

 QMap<Channel_Types, caon_ptr<tNode>> entry_nodes_map_;

 caon_ptr<tNode> map_key_sequence_ref_node_;

 void init_channels(tNode& fdef_node);

 caon_ptr<RZ_ASG_Token> channel_sequence(caon_ptr<tNode>& sequence_node, signed int& depth_change);

 const ChasmRZ_Query& Qy;

 QStack<caon_ptr<ChasmRZ_Node>> entry_nodes_;

 int map_key_sequence_order_;
 caon_ptr<RZ_Function_Def_Info> ref_fdi_;


public:

 RZ_Function_Def_Info(ChasmRZ_Function_Def_Entry& function_def_entry
  ,type_name_callback_type type_name_callback
  ,ls_callback_type ls_callback);

 ACCESSORS(caon_ptr<ChasmRZ_Function_Def_Entry> ,function_def_entry)
 ACCESSORS(caon_ptr<ChasmRZ_Node> ,map_key_sequence_ref_node)
 ACCESSORS(int ,map_key_sequence_order)
 ACCESSORS(caon_ptr<RZ_Function_Def_Info> ,ref_fdi)


 QString channel_string(const RZ_Function_Def_Syntax& syntax, caon_ptr<ChasmRZ_Node> sequence_node);
 QString lambda_channel_string(const RZ_Function_Def_Syntax& syntax);
 QString sigma_channel_string(const RZ_Function_Def_Syntax& syntax);
 QString return_channel_string(int& token_count);
 QString channel_string(Channel_Types ct, const RZ_Function_Def_Syntax& syntax);

 QString kauvir_entry_code_string(RZ_ASG_Lexical_Scope& ls);

 QString kauvir_entry_code_string_by_channel_type(Channel_Types ct, RZ_ASG_Lexical_Scope& ls);

 QString context_channel_string();

 caon_ptr<tNode> function_def_entry_node();

 caon_ptr<tNode> function_def_entry_prior_node()
 {
  return function_def_entry_->prior_node();
 }

 QString get_label();

 QString dynamo_signature_code_for_type(QString carrier_kind, QString type_name);
 QString dynamo_signature_code_for_symbol(QString carrier_kind, QString symbol_name);
 QString dynamo_signature_code_for_symbol(QString carrier_kind, QString symbol_name, QString type_name);

 QString dynamo_signature_code_for_empty_channel(QString carrier_kind);

 QString dynamo_signature_code_string();
 QString dynamo_signature_code_string_by_channel_type(Channel_Types ct);

 void write_phr_signature_code(PGB_IR_Build& pgb,
   QList<PGB_IR_Build::Text_With_Purpose>& step_forms);
 void write_phr_signature_code_by_channel_type(PGB_IR_Build& pgb,
   QList<PGB_IR_Build::Text_With_Purpose>& step_forms, Channel_Types ct);

 void write_phr_signature_code_for_empty_channel(PGB_IR_Build& pgb,
   QList<PGB_IR_Build::Text_With_Purpose>& step_forms, QString carrier_kind);

 void write_phr_signature_code_channel_entry(PGB_IR_Build& pgb,
  QList<PGB_IR_Build::Text_With_Purpose>& step_forms, QString carrier_kind);

 void write_phr_signature_code_for_type(PGB_IR_Build& pgb,
   QList<PGB_IR_Build::Text_With_Purpose>& step_forms,
   QString carrier_kind, QString type_name);
 void write_phr_signature_code_for_symbol(PGB_IR_Build& pgb,
   QList<PGB_IR_Build::Text_With_Purpose>& step_forms,
   QString carrier_kind, QString symbol_name);
 void write_phr_signature_code_for_symbol(PGB_IR_Build& pgb,
   QList<PGB_IR_Build::Text_With_Purpose>& step_forms,
   QString carrier_kind, QString symbol_name, QString type_name);

};

_RZNS(GVal)

#endif //RZ_FUNCTION_DEF_INFO__H
