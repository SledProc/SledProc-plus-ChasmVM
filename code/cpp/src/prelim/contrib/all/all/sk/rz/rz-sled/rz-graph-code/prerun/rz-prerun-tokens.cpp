
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rz-prerun-tokens.h"

#include "rz-graph-token/token/rz-asg-token.h"

#include "rz-graph-core/token/chasm-rz-token.h"

#include "rz-graph-core/token/rz-observer-function.h"
#include "rz-graph-core/token/rz-block-level-type-declaration.h"

#include "rz-graph-core/kernel/graph/chasm-rz-node.h"

#include "rz-graph-core/kernel/grammar/chasm-rz-node-factory.h"

USING_RZNS(RZ_Core)
USING_RZNS(GBuild)


RZ_Prerun_Tokens::RZ_Prerun_Tokens(caon_ptr<ChasmRZ_Document> document)
 : ChasmRZ_ASG_Output(document)
{

}

void RZ_Prerun_Tokens::init_token(const ChasmRZ_Node& node, caon_ptr<ChasmRZ_Node>& rat_node)
{
 caon_ptr<RZ_ASG_Token> rat;
 QString label;
 QString label_template = "(%1)";

 switch (node.type_code())
 {
 case ChasmRZ_Node::Type_Code::Block_Level_Type_Declaration:
  {
   caon_ptr<RZ_Block_Level_Type_Declaration> blt = node.block_level_type_declaration_Unchecked();
   init_token(*blt->token());
   rat = blt->token()->asg_token();
   label = label_template.arg(blt->token_string_summary());

   caon_ptr<ChasmRZ_Node> cn = traverser_.find_type_declaration_continuation(&node);

   CAON_PTR_DEBUG(ChasmRZ_Node ,cn)

   if(cn)
   {
    blt->set_continuation_node(cn);

    if(caon_ptr<ChasmRZ_Token> ctok = cn->chasm_rz_token())
    {
     CAON_PTR_DEBUG(ChasmRZ_Token ,ctok)
     RZ_ASG_Token::init_asg_token(*ctok);
    }

   }

  }
  break;

 case ChasmRZ_Node::Type_Code::Observer_Function:
  {
   caon_ptr<RZ_Observer_Function> rzof  = node.rz_observer_function_Unchecked();
   CAON_PTR_DEBUG(RZ_Observer_Function ,rzof)

   rzof->init_asg_token();
   rat = rzof->asg_token();
   label = label_template.arg("<%1>"_qt.arg(rzof->name()));
  }
  break;

 default:
  break;
 }

 if(rat)
 {
  rat_node = node_factory_.make_new_node(rat, label);

 }

// if(caon_ptr<RZ_Observer_Function> rzof  = node.rz_compiler_function())
// {
//  rzof->init_get_asg_token();
//  rat = rzof->get_asg_token();
// }

}

void RZ_Prerun_Tokens::init_token(ChasmRZ_Token& token)
{
 RZ_ASG_Token::init_asg_token(token);
}

void RZ_Prerun_Tokens::report_token(QTextStream& qts,
 const ChasmRZ_Token& token)
{
 qts << QString("<<%1>>").arg(token.raw_text());
}


void RZ_Prerun_Tokens::report_tuple_info_entry(QTextStream& qts,
 const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> rce_ptr)
{

}

void RZ_Prerun_Tokens::report_tuple_info_leave(QTextStream& qts,
 const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> rce_ptr)
{

}

void RZ_Prerun_Tokens::report_call_entry(QTextStream& qts,
 const ChasmRZ_Call_Entry& rce)
{

}

void RZ_Prerun_Tokens::report_call_leave(QTextStream& qts,
 const ChasmRZ_Call_Entry& rce)
{

}

