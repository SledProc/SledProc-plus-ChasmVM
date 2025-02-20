
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef CHASM_RZ_ANCHORED_CASEMENT_ENTRY__H
#define CHASM_RZ_ANCHORED_CASEMENT_ENTRY__H

#include "accessors.h"
#include "flags.h"

#include "global-types.h"

#include "relae-graph/relae-caon-ptr.h"

#include <QString>
#include <QMap>

#include "rzns.h"

RZNS_(RZ_Core)

class ChasmRZ_Node;
class ChasmRZ_Connectors;
class ChasmRZ_Frame;

class ChasmRZ_Anchored_Casement_Entry
{

public:

 enum class Statement_Entry_Modes : u2  {
   N_A = 0, Token_Single = 1, Token_Split = 2, Token_Split_Opaque = 4,
   Token_Repeat = 8, Token_Reset = 16,

   Anchored = Token_Single | Token_Split | Token_Split_Opaque |
     Token_Repeat | Token_Reset,

   No_Anchor = 32, Discard_Return = 64,
   Void_Anchor = Anchored | Discard_Return
 };


private:

 Statement_Entry_Modes statement_entry_mode_;

public:

 ChasmRZ_Anchored_Casement_Entry(Statement_Entry_Modes statement_entry_mode
   = Statement_Entry_Modes::N_A);

 ACCESSORS__GET(Statement_Entry_Modes ,statement_entry_mode)

 QString string_summary();

};

_RZNS(RZ_Core)

#endif //CHASM_RZ_CASEMENT_CALL_ENTRY__H
