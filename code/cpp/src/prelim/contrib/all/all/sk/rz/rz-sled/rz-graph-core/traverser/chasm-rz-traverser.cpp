
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "chasm-rz-traverser.h"


#include "kernel/graph/chasm-rz-graph.h"

#include "kernel/graph/chasm-rz-node.h"

#include "kernel/query/chasm-rz-query.h"

#include "textio.h"

USING_RZNS(RZ_Core)
USING_RZNS(GBuild)

USING_KANS(TextIO)

ChasmRZ_Traverser::ChasmRZ_Traverser(ChasmRZ_Graph& graph)
  : graph_(graph)
    ,Cf(ChasmRZ_Frame::instance("casement"))
    ,Sf(ChasmRZ_Frame::instance("semantic"))
    ,Tf(ChasmRZ_Frame::instance("traversal"))
    ,Qy(ChasmRZ_Query::instance())
{

}

#define in_Cf Cf,
#define in_Tf Tf,
#define in_Sf Sf,




caon_ptr<ChasmRZ_Node> ChasmRZ_Traverser::find_root_entry(const caon_ptr<ChasmRZ_Node> root_node)
{
 CAON_PTR_DEBUG(ChasmRZ_Node ,root_node)

 caon_ptr<ChasmRZ_Node> rnode = graph_.root_node();

 CAON_PTR_DEBUG(ChasmRZ_Node ,rnode)

 caon_ptr<ChasmRZ_Node> node;

 if(node = Qy.Block_Level_Entry(in_Cf root_node))
 {
  CAON_PTR_DEBUG(ChasmRZ_Node ,node)
  return node;
 }


 return nullptr;
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Traverser::find_type_declaration_continuation(const caon_ptr<ChasmRZ_Node> node)
{
 CAON_PTR_DEBUG(ChasmRZ_Node ,node)

 node->debug_connections();

 caon_ptr<ChasmRZ_Node> result = Qy.Type_Symbol_Declaration(in_Cf node);

 CAON_PTR_DEBUG(ChasmRZ_Node ,result)

 return result;
}


//void ChasmRZ_Traverser::scan()
//{
//}

