
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


// // List run types.  The RZ_RUN_TYPE macro is redefined in different places
 //   and expands to different code using the two values written on each line
//   here, depending on the context in which this file is included.
RZ_RUN_TYPE_NS(GVal, Str, str, RZ_String, RZ_String, Core_Class)
RZ_RUN_TYPE_NS(GVal, Mlt, mlt, RZ_Match_Literal, RZ_Match_Literal, Core_Class)
RZ_RUN_TYPE_NS(GVal, Sym, sym, RZ_ASG_Symbol, RZ_Symbol, Core_Class)
RZ_RUN_TYPE_NS(GVal, Opc, opc, RZ_Opaque_Call, RZ_Opaque_Call, Core_Class)
RZ_RUN_TYPE_NS(GVal, Ots, ots, RZ_Opaque_Type_Symbol, RZ_Opaque_Type_Symbol, Core_Class)
RZ_RUN_TYPE_NS(GVal, Block_Info, block_info, RZ_ASG_Block_Info, RZ_ASG_Block_Info, Core_Class)
RZ_RUN_TYPE_NS(GBuild, Null_Value, Null_Value, RZ_Null_Value, RZ_Null_Value, Core_Class)

RZ_RUN_TYPE_NS(GBuild, GraphCoreFun, cfn, RZ_ASG_Core_Casement_Function, RZ_Graph_Core_Function, Core_Class)
RZ_RUN_TYPE_NS(GBuild, EmptyTuple, empty-tuple, RZ_ASG_Empty_Tuple, RZ_Empty_Tuple, Core_Class)
RZ_RUN_TYPE_NS(GBuild, FAIRAnnotation, fairan, RZ_FAIR_Annotation, RZ_FAIR_Annotation, Core_Class)
RZ_RUN_TYPE_NS(GVal, Vec, vec, RZ_ASG_Vector, RZ_Vector, Core_Class)
RZ_RUN_TYPE_NS(GVal, StrPlex, str_plex, RZ_String_Plex, RZ_String_Plex, Core_Class)
RZ_RUN_TYPE_NS(GVal, Map, map, RZ_ASG_Map, RZ_Map, Core_Class)

RZ_RUN_TYPE_NS(GVal, FnDefInfo, fdef, RZ_Function_Def_Info, RZ_Function_Def_Info, Core_Class)

RZ_RUN_TYPE_NS(GVal, UserType, uty, RZ_Phaon_User_Type, RZ_Phaon_User_Type, Core_Class)


#define RZ_TEMP_CASES_TYPECODE_NAMESPACED 15

