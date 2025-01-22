
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


CHASMRZ_SPECIAL_TOKEN("doc-info_", DocInfo_, Text_Map_Enter)
CHASMRZ_SPECIAL_TOKEN("_doc-info", _DocInfo, Text_Map_Leave)

CHASMRZ_SPECIAL_TOKEN_RENAME("----", Logical_Scope_Enter_4, Auto_Statement_End)
CHASMRZ_SPECIAL_TOKEN_RENAME(";;;;", Logical_Scope_Leave_4, Auto_Statement_End)

CHASMRZ_SPECIAL_TOKEN("->", Call_Arrow, Arrow)
CHASMRZ_SPECIAL_TOKEN("\\->", Call_Arrow_Fdef, Arrow)
CHASMRZ_SPECIAL_TOKEN("%->", Call_Arrow_To_Block, Arrow)
CHASMRZ_SPECIAL_TOKEN("->def", Call_Arrow_No_Def, Arrow)
CHASMRZ_SPECIAL_TOKEN("$->", Call_Arrow_Note, Arrow)
CHASMRZ_SPECIAL_TOKEN("@->", Call_Arrow_Async, Arrow)
CHASMRZ_SPECIAL_TOKEN("->>", Call_Arrow_No_Cast, Arrow)

//CHASMRZ_SPECIAL_TOKEN("-->", Call_Arrow_Future, Arrow)
//CHASMRZ_SPECIAL_TOKEN("'->", Call_Arrow_Type_Expression, Arrow)
//CHASMRZ_SPECIAL_TOKEN("`->", Call_Arrow_Monad, Arrow)
//CHASMRZ_SPECIAL_TOKEN("-/>", Call_Arrow_With_Matching, Arrow)
//CHASMRZ_SPECIAL_TOKEN(">-->", Call_Arrow_Via_ASG_Callback, Normal)
//CHASMRZ_SPECIAL_TOKEN(">-", Pending_Call_Arrow_Via_ASG_Callback, Normal)
//CHASMRZ_SPECIAL_TOKEN("/-->", Default_With_Matching, Auto_Statement_End)
//CHASMRZ_SPECIAL_TOKEN("$$", Matching_Default, Normal)

CHASMRZ_SPECIAL_TOKEN("-!", Auto_Expand_To_Null_Test, Normal)
CHASMRZ_SPECIAL_TOKEN("-?", Auto_Expand_To_Not_Null_Test, Normal)

CHASMRZ_SPECIAL_TOKEN("retval", Return_Value_Blank_Or_Identity, Normal)
