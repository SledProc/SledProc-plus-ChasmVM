
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "chasm-rz-node-factory.h"

#include "kernel/chasm-rz-root.h"

#include "kernel/graph/chasm-rz-node.h"

#include "chasm-rz-grammar.h"

#include "code/chasm-rz-call-entry.h"
#include "code/chasm-rz-casement-call-entry.h"

#include "code/chasm-rz-block-entry.h"
#include "code/chasm-rz-casement-block-entry.h"

#include "kernel/graph/chasm-rz-graph.h"

#include "token/chasm-rz-token.h"
#include "token/rz-block-level-type-declaration.h"

#include "code/chasm-rz-anchored-casement-entry.h"

#include "kernel/chasm-rz-dominion.h"

#include "token/rz-compiler-function.h"


USING_RZNS(RZ_Core)

ChasmRZ_Node_Factory::ChasmRZ_Node_Factory()
  :  call_entry_count_(0), block_entry_count_(0), tuple_entry_count_(0)
{
}


caon_ptr<ChasmRZ_Node> ChasmRZ_Node_Factory::make_new_node(caon_ptr<ChasmRZ_Token> token) const
{
 caon_ptr<ChasmRZ_Node> result = new ChasmRZ_Node(token);
 RELAE_SET_NODE_LABEL(result, token->string_summary());
 return result;
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Node_Factory::make_new_node(caon_ptr<RZ_Block_Level_Type_Declaration> blt) const
{
 caon_ptr<ChasmRZ_Node> result = new ChasmRZ_Node(blt);
 RELAE_SET_NODE_LABEL(result, blt->token_string_summary());
 return result;
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Node_Factory::make_new_node(caon_ptr<ChasmRZ_Anchored_Casement_Entry> ace) const
{
 caon_ptr<ChasmRZ_Node> result = new ChasmRZ_Node(ace);
 RELAE_SET_NODE_LABEL(result, ace->string_summary());
 return result;
}




caon_ptr<ChasmRZ_Node> ChasmRZ_Node_Factory::make_new_node(caon_ptr<RZ_String_Plex_Builder> rzspb) const
{
 caon_ptr<ChasmRZ_Node> result = new ChasmRZ_Node(rzspb);

 return result;
}


caon_ptr<ChasmRZ_Node> ChasmRZ_Node_Factory::make_new_node(caon_ptr<ChasmRZ_Casement_Call_Entry> rce) const
{
 caon_ptr<ChasmRZ_Node> result = new ChasmRZ_Node(rce);
 RELAE_SET_NODE_LABEL(result, "<casement-call %1>"_qt.arg(rce->call_id()));
 return result;
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Node_Factory::make_new_node(caon_ptr<ChasmRZ_Call_Entry> rce) const
{
 caon_ptr<ChasmRZ_Node> result = new ChasmRZ_Node(rce);
 RELAE_SET_NODE_LABEL(result, "<call %1>"_qt.arg(rce->call_id()));
 return result;
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Node_Factory::make_new_node(caon_ptr<ChasmRZ_Function_Def_Entry> fdef) const
{
 caon_ptr<ChasmRZ_Node> result = new ChasmRZ_Node(fdef);
 RELAE_SET_NODE_LABEL(result, "<fdef>");
 return result;
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Node_Factory::make_new_node(caon_ptr<ChasmRZ_Block_Entry> rbe) const
{
 caon_ptr<ChasmRZ_Node> result = new ChasmRZ_Node(rbe);
 RELAE_SET_NODE_LABEL(result, "<block %1>"_qt.arg(rbe->block_id()));
 return result;
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Node_Factory::make_new_node(caon_ptr<ChasmRZ_Casement_Block_Entry> cbe) const
{
 caon_ptr<ChasmRZ_Node> result = new ChasmRZ_Node(cbe);
 RELAE_SET_NODE_LABEL(result, "<block %1>"_qt.arg(cbe->block_id()));
 return result;
}


caon_ptr<ChasmRZ_Node> ChasmRZ_Node_Factory::make_new_node(caon_ptr<RZ_Compiler_Function> rcf) const
{
 caon_ptr<ChasmRZ_Node> result = new ChasmRZ_Node(rcf);
 RELAE_SET_NODE_LABEL(result, "<%1>"_qt.arg(rcf->token_label()));
 return result;
}


