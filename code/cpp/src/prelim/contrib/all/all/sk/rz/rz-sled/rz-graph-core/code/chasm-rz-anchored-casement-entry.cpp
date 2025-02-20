

//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "chasm-rz-anchored-casement-entry.h"

#include "kernel/graph/chasm-rz-node.h"

#include "token/chasm-rz-token.h"

#include "rzns.h"

#include <QDebug>



USING_RZNS(RZ_Core)

ChasmRZ_Anchored_Casement_Entry::ChasmRZ_Anchored_Casement_Entry(Statement_Entry_Modes statement_entry_mode)
 : statement_entry_mode_(statement_entry_mode)
{

}

QString ChasmRZ_Anchored_Casement_Entry::string_summary()
{
 QMap<Statement_Entry_Modes, QString> static_map {

  {Statement_Entry_Modes::Token_Single, "single"},
  {Statement_Entry_Modes::Token_Split, "split"},
  {Statement_Entry_Modes::Token_Split_Opaque, "split-opaque"},
  {Statement_Entry_Modes::Token_Repeat, "repeat"},
  {Statement_Entry_Modes::Token_Reset, "reset"},
  {Statement_Entry_Modes::No_Anchor, "no-anchor"},
  {Statement_Entry_Modes::Discard_Return, "discard"},
 };

 return "((%1))"_qt.arg(static_map.value(statement_entry_mode_, "?"));
}

