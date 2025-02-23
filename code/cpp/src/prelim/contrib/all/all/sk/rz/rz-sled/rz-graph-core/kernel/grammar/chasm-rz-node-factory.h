
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef CHASM_RZ_NODE_FACTORY__H
#define CHASM_RZ_NODE_FACTORY__H

#include "relae-graph/relae-parser.h"

#include "rz-graph-core/kernel/chasm-rz-dominion.h"

#include "global-types.h"

#include "rzns.h"
RZNS_(RZ_Core)

class ChasmRZ_Node;

class ChasmRZ_Token;
class RZ_Block_Level_Type_Declaration;
class ChasmRZ_Casement_Call_Entry;
class ChasmRZ_Call_Entry;
class ChasmRZ_Casement_Block_Entry;
class ChasmRZ_Block_Entry;
class ChasmRZ_Function_Def_Entry;
class RZ_String_Plex_Builder;
class ChasmRZ_Anchored_Casement_Entry;



class ChasmRZ_Node_Factory
{
 //QString raw_text_;

 u2 call_entry_count_;
 u2 block_entry_count_;
 u2 tuple_entry_count_;


 ChasmRZ_Node_Factory();

public:

#define ACCESSORS__MAKE_ID(ty ,x) \
 ty make_##x##_id() { return ++x##_count_; }


 ACCESSORS__MAKE_ID(u2 ,call_entry)
 ACCESSORS__MAKE_ID(u2 ,block_entry)
 ACCESSORS__MAKE_ID(u2 ,tuple_entry)


 //ACCESSORS(QString ,raw_text)

 static ChasmRZ_Node_Factory& instance()
 {
  ChasmRZ_Node_Factory* the_instance = new ChasmRZ_Node_Factory;
  return *the_instance;
 }


 caon_ptr<ChasmRZ_Node> make_new_node(caon_ptr<ChasmRZ_Token> token) const;
 caon_ptr<ChasmRZ_Node> make_new_node(caon_ptr<RZ_Block_Level_Type_Declaration> blt) const;
 caon_ptr<ChasmRZ_Node> make_new_node(caon_ptr<ChasmRZ_Casement_Call_Entry> rce) const;
 caon_ptr<ChasmRZ_Node> make_new_node(caon_ptr<ChasmRZ_Call_Entry> rce) const;
 caon_ptr<ChasmRZ_Node> make_new_node(caon_ptr<ChasmRZ_Casement_Block_Entry> rbe) const;
 caon_ptr<ChasmRZ_Node> make_new_node(caon_ptr<ChasmRZ_Block_Entry> rbe) const;
 caon_ptr<ChasmRZ_Node> make_new_node(caon_ptr<ChasmRZ_Function_Def_Entry> fdef) const;
 caon_ptr<ChasmRZ_Node> make_new_node(caon_ptr<RZ_String_Plex_Builder> rzspb) const;
 caon_ptr<ChasmRZ_Node> make_new_node(caon_ptr<ChasmRZ_Anchored_Casement_Entry> ace) const;
 caon_ptr<ChasmRZ_Node> make_new_node(caon_ptr<RZ_Compiler_Function> rcf) const;




};

_RZNS(RZ_Core)

#endif //CHASM_RZ_NODE_FACTORY__H
