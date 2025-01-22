
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RPI_BLOCK__H
#define RPI_BLOCK__H

#include "accessors.h"
#include "flags.h"

#include <QString>
#include <QMap>

#include <QVector>

#include <QTextStream>

#include "rzns.h"

#include "relae-graph/relae-caon-ptr.h"

#include "multistep-token.h"

#include "phr-graph-core/kernel/graph/pgb-ir-build.h"

RZNS_(RZ_Core)
 class ChasmRZ_Node;
 class ChasmRZ_Block_Entry;
_RZNS(RZ_Core)

USING_RZNS(RZ_Core)

RZNS_(GBuild)
 class RZ_ASG_Visitor;
 class RZ_Graph_Visitor_Phaon;
 class RZ_ASG_Token;
_RZNS(GBuild)

USING_RZNS(GBuild)

RZNS_(PhrGraphCore)
 class PGB_IR_Build;
_RZNS(PhrGraphCore)

USING_RZNS(PhrGraphCore)

RZNS_(GVal)

class RPI_Stage_Form;
class RZ_ASG_Lexical_Scope;

class RZ_Function_Def_Syntax;
class RZ_ASG_Block_Info;
class RZ_Code_Statement;
class RZ_Function_Def_Info;


class RPI_Block
{
public:

 enum class Block_Sequence_Modes {
  N_A, If_Else, If_Elsif, Map, Ghost
 };

private:

 PGB_IR_Build& pgb_;

 //?QStringList header_step_forms_;

 caon_ptr<RZ_ASG_Lexical_Scope> lexical_scope_;


 caon_ptr<RPI_Block> parent_block_;
 caon_ptr<RPI_Block> continue_block_;

 QVector<caon_ptr<RPI_Stage_Form>> forms_;

 caon_ptr<RPI_Stage_Form> current_form_;
 caon_ptr<RPI_Stage_Form> last_form_;

 caon_ptr<RZ_ASG_Block_Info> pending_block_info_;
 caon_ptr<RZ_ASG_Block_Info> block_info_;
 caon_ptr<RPI_Stage_Form> preceding_expression_form_;

 MS_Token held_token_;
 caon_ptr<RZ_ASG_Token> held_sigma_token_;

 QString entry_lisp_code_;

 int parent_lambda_position_;

 void add_form_from_call_entry_node(RZ_Graph_Visitor_Phaon& visitor_phaon,
   ChasmRZ_Node& entry_node,
   caon_ptr<ChasmRZ_Block_Entry> rbe = nullptr,
   caon_ptr<RPI_Stage_Form> prior_form = nullptr);

 QString es_argument_;

 Block_Sequence_Modes block_sequence_mode_;

 QVector<caon_ptr<RZ_Code_Statement>> code_statements_;
 caon_ptr<RZ_Function_Def_Info> function_def_info_;

 //?QString held_type_name_;

 static int get_new_hd_code();

public:

 RPI_Block(PGB_IR_Build& pgb, caon_ptr<RPI_Block> parent_block = nullptr);

 ACCESSORS(caon_ptr<RPI_Block> ,parent_block)
 ACCESSORS(caon_ptr<RPI_Block> ,continue_block)

 ACCESSORS(QVector<caon_ptr<RPI_Stage_Form>> ,forms)
 ACCESSORS(int ,parent_lambda_position)
 ACCESSORS(QString ,entry_lisp_code)

 ACCESSORS(caon_ptr<RZ_ASG_Block_Info> ,pending_block_info)
 ACCESSORS(caon_ptr<RZ_ASG_Block_Info> ,block_info)
 ACCESSORS(caon_ptr<RPI_Stage_Form> ,preceding_expression_form)

 ACCESSORS(Block_Sequence_Modes ,block_sequence_mode)

 ACCESSORS(caon_ptr<RZ_ASG_Lexical_Scope> ,lexical_scope)
 ACCESSORS(caon_ptr<RZ_Function_Def_Info> ,function_def_info)



 ACCESSORS(QString ,es_argument)

 caon_ptr<ChasmRZ_Block_Entry> get_block_entry();

 RZ_ASG_Visitor& visitor();

 void write(QList<PGB_IR_Build::Text_With_Purpose>& tps, QTextStream* qts);
 void write_top_level(QList<PGB_IR_Build::Text_With_Purpose>& tps, QTextStream* qts);

 void scan_top_level(RZ_Graph_Visitor_Phaon& visitor_phaon);

 void build_phaon_graph();

 void scan(RZ_Graph_Visitor_Phaon& visitor_phaon, ChasmRZ_Node& start_node);

 void scan_form_from_start_node(RZ_Graph_Visitor_Phaon& visitor_phaon,
   ChasmRZ_Node& start_node);

 void scan_form_from_statement_entry_node(RZ_Graph_Visitor_Phaon& visitor_phaon,
   ChasmRZ_Node& start_node);

};

_RZNS(GVal)

#endif //RPI_BLOCK__H
