
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "htxn-infoset-cursor-8b.h"

#include "glyph-layer-8b.h"

#include <QTextStream>

USING_KANS(HTXN)


HTXN_Infoset_Cursor_8b::HTXN_Infoset_Cursor_8b(HTXN_Infoset_8b* infoset)
  :  infoset_(infoset), current_range_(nullptr),
     current_position_({nullptr, 0})
{

}



