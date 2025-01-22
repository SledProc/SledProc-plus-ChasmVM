
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#define DOMINION_NODE_TYPE ChasmRZ_Node

#ifndef DOMINION_HIDE_NO_NAMESPACE
// // No namespace
#include "relae-graph/dominion-macros.h"
DOMINION_TYPE(dbl, double, Double)
DOMINION_TYPE(integer, int, Int)
DOMINION_TYPE(i8, char, I8)
DOMINION_TYPE(qstring, QString, QStr)
#endif

#include "relae-graph/dominion-macros.h"

#define DOMINION_OUTER_NAMESPACE RZ


#define DOMINION_INNER_NAMESPACE RZ_Core
#include "relae-graph/dominion-macros.h"
DOMINION_TYPE(chasm_rz_root, ChasmRZ_Root, ChasmRZ_Root)
DOMINION_TYPE(chasm_rz_token, ChasmRZ_Token, ChasmRZ_Token)
DOMINION_TYPE(proxy, ChasmRZ_Node_Proxy, Proxy)
DOMINION_TYPE(chasm_rz_tuple_info, ChasmRZ_Tuple_Info, ChasmRZ_Tuple_Info)
DOMINION_TYPE(chasm_rz_call_entry, ChasmRZ_Call_Entry, ChasmRZ_Call_Entry)
DOMINION_TYPE(chasm_rz_casement_call_entry, ChasmRZ_Casement_Call_Entry, ChasmRZ_Casement_Call_Entry)
DOMINION_TYPE(chasm_rz_block_entry, ChasmRZ_Block_Entry, ChasmRZ_Block_Entry)
DOMINION_TYPE(chasm_rz_casement_block_entry, ChasmRZ_Casement_Block_Entry, ChasmRZ_Casement_Block_Entry)
DOMINION_TYPE(chasm_rz_function_def_entry, ChasmRZ_Function_Def_Entry, ChasmRZ_Function_Def_Entry)
DOMINION_TYPE(string_plex_builder, RZ_String_Plex_Builder, StrPlexBuilder)
#undef DOMINION_INNER_NAMESPACE



//#define DOMINION_INNER_NAMESPACE Text
//#include "relae-graph/dominion-macros.h"
//DOMINION_TYPE(text_punctuation, RZ_Text_Punctuation, RZ_Text_Punctuation)
//#undef DOMINION_INNER_NAMESPACE

#define DOMINION_INNER_NAMESPACE GEmbed
#include "relae-graph/dominion-macros.h"
DOMINION_TYPE(embed_token, RZ_Graph_Embed_Token, RZ_Graph_Embed_Token)
DOMINION_TYPE(cpp_token, RZ_Graph_Cpp_Token, RZ_Graph_Cpp_Token)
DOMINION_TYPE(kauvir_token, RZ_Graph_Kauvir_Token, RZ_Graph_Kauvir_Token)
DOMINION_TYPE(dynamo_token, RZ_Graph_Dynamo_Token, RZ_Graph_Dynamo_Token)
DOMINION_TYPE(ecl_token, RZ_Graph_ECL_Token, RZ_Graph_ECL_Token)
#undef DOMINION_INNER_NAMESPACE


#define DOMINION_INNER_NAMESPACE GBuild
#include "relae-graph/dominion-macros.h"
//?
DOMINION_TYPE(core_function, RZ_ASG_Core_Casement_Function, Core_Function)
//?DOMINION_TYPE(cfn, RZ_ASG_Core_Casement_Function, Token_CoreFun)
DOMINION_TYPE(fair_annotation, RZ_FAIR_Annotation, Fair_Annotation)
DOMINION_TYPE(null_value, RZ_Null_Value, Null_Value)
DOMINION_TYPE(empty_tuple, RZ_ASG_Empty_Tuple, EmptyTuple)
#undef DOMINION_INNER_NAMESPACE


#define DOMINION_INNER_NAMESPACE GVal
#include "relae-graph/dominion-macros.h"
DOMINION_TYPE(fair_codepoint, RZ_FAIR_Codepoint, FAIR_Codepoint)
DOMINION_TYPE(fair_archpoint, RZ_FAIR_Archpoint, FAIR_Archpoint)
DOMINION_TYPE(fair_textpoint, RZ_FAIR_Textpoint, FAIR_Textpoint)
DOMINION_TYPE(fair_filepoint, RZ_FAIR_Filepoint, FAIR_Filepoint)
DOMINION_TYPE(vec, RZ_ASG_Vector, ASG_Vector)
DOMINION_TYPE(map, RZ_ASG_Map, ASG_Map)
DOMINION_TYPE(str, RZ_String, Str)
DOMINION_TYPE(str_plex, RZ_String_Plex, StrPlex)
DOMINION_TYPE(mlt, RZ_Match_Literal, Mlt)
DOMINION_TYPE(opc, RZ_Opaque_Call, Opc)
DOMINION_TYPE(sym, RZ_ASG_Symbol, Sym)
DOMINION_TYPE(ots, RZ_Opaque_Type_Symbol, Ots)
DOMINION_TYPE(lexs, RZ_ASG_Lexical_Scope, Lexs)
DOMINION_TYPE(block_info, RZ_ASG_Block_Info, Block_Info)
DOMINION_TYPE(rz_function_def_info, RZ_Function_Def_Info, RZ_Function_Def_Info)
DOMINION_TYPE(rz_code_statement, RZ_Code_Statement, RZ_Code_Statement)
DOMINION_TYPE(rz_expression_review, RZ_Expression_Review, RZ_Expression_Review)
DOMINION_TYPE(core_pair, RZ_ASG_Valuer_Core_Pair, RZ_ASG_Valuer_Core_Pair)
DOMINION_TYPE(user_type, RZ_Phaon_User_Type, Uty)
DOMINION_TYPE(logs, RZ_ASG_Logical_Scope, Logs)
#undef DOMINION_INNER_NAMESPACE

#undef DOMINION_OUTER_NAMESPACE
#undef DOMINION_NODE_TYPE

