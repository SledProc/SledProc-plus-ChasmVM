
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef HTXN_INFOSET_CURSOR_8B__H
#define HTXN_INFOSET_CURSOR_8B__H

#include "accessors.h"

#include "global-types.h"

#include "flags.h"

#include "kans.h"

#include <QString>
#include <QList>
#include <QDate>

#include <functional>

#include "htxn-infoset-range.h"


class QTextStream;

KANS_(HTXN)

class Glyph_Layer_8b;
class HTXN_Infoset_8b;

class HTXN_Infoset_Cursor_8b 
{ 
 HTXN_Infoset_8b* infoset_;
 HTXN_Infoset_Range* current_range_;
  
 HTXN_Layer_Position current_position_;

public:

 HTXN_Infoset_Cursor_8b(HTXN_Infoset_8b* infoset);
};



_KANS(HTXN)

#endif // HTXN_CURSOR_8B__H
