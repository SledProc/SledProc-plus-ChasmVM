
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef GLYPH_VECTOR_8B__H
#define GLYPH_VECTOR_8B__H

#include "accessors.h"

#include "global-types.h"

#include "kans.h"

#include <QString>
#include <QList>
#include <QVector>

#include <functional>

#include "glyph-refinement.h"

//class WCM_WhiteDB;

KANS_(HTXN)

class GlyphDeck_Base_8b;
struct Glyph_Argument_Package;



class Glyph_Vector_8b : public QVector<u1>
{
 QMap<u4, QPair<u2, u2> > dia_map_;
 QMap<u4, u2 > deck_map_;
 QMap<u4, void* > xx_map_;
 QMap<u4, Glyph_Refinement > refinement_map_;

 void check_external(u4 index, u1 gp, Glyph_Argument_Package& gap);

public:

 void check_external(u4 index,
   GlyphDeck_Base_8b& current_deck, Glyph_Argument_Package& gap); 

 void add_diacritic(u4 index, u2 deck_code, u2 dia_code);


// void check_external_excluding_numeral_diacritic(u4 index,
//   GlyphDeck_Base_8b& current_deck, Glyph_Argument_Package& gap);
 

};

_KANS(HTXN)

#endif // GLYPH_VECTOR_8B__H
