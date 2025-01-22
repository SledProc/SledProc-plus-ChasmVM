
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef GLYPH_REFINEMENT__H
#define GLYPH_REFINEMENT__H

#include "accessors.h"

#include "flags.h"

#include "kans.h"

#include <QString>
#include <QList>
#include <QVector>

#include <functional>

#include "glyph-refinement.h"

//class WCM_WhiteDB;

KANS_(HTXN)

typedef quint8 u1;
typedef quint16 u2;
typedef quint32 u4;
typedef quint64 u8;


class Glyph_Refinement
{
 flags_(8)
  bool hyphen_break:1;

 _flags

 QString text_;

public:

 Glyph_Refinement(); 

 ACCESSORS(QString ,text)

// void check_external_excluding_numeral_diacritic(u4 index,
//   GlyphDeck_Base_8b& current_deck, Glyph_Argument_Package& gap);
 

};

_KANS(HTXN)

#endif // GLYPH_REFINEMENT__H
