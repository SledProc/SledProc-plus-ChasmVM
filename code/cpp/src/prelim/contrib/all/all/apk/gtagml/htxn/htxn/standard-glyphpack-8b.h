
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef STANDARD_GLYPHPACK_8B__H
#define STANDARD_GLYPHPACK_8B__H

#include "accessors.h"

#include "global-types.h"

#include "kans.h"

#include <QString>
#include <QList>
#include <QDate>

#include <functional>

//class WCM_WhiteDB;

KANS_(HTXN)

class Standard_GlyphPack_8b
{
 u1 code_;

public:

 explicit Standard_GlyphPack_8b(u1 code); 

 std::pair<u1, u1> get_latex_cue();
 std::pair<u1, u1> get_xml_cue();
 u1 get_diacritic_code_cue();



};

_KANS(HTXN)

#endif // STANDARD_GLYPHDECK
