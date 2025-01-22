
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef GLYPH_ARGUMENT_PACKAGE__H
#define GLYPH_ARGUMENT_PACKAGE__H

#include "kans.h"

#include "accessors.h"
#include "flags.h"

#include "global-types.h"


#include <QString>
#include <QList>
#include <QVector>

#include <functional>


KANS_(HTXN)

class GlyphDeck_Base_8b;
class Diacritic_GlyphDeck_Base;


struct Glyph_Argument_Package
{
 flags_(2)
  bool normal:1;

  bool maybe_external_deck:1;
  bool confirmed_external_deck:1;
  bool maybe_external_diacritic:1;
  bool confirmed_external_diacritic:1;

  bool confirmed_external_extended:1;
  bool maybe_external_range:1;
  bool request_deck_resolve:1;
  bool confirmed_non_diacritic:1;

  bool maybe_refinement:1;

  bool has_alt:1;

  bool alt_1:1;
  bool alt_2:1;
  bool alt_3:1;
  bool alt_4:1;

//  bool use_refinements:1;
//  bool use_numeral_diacritic:1;
//  bool use_underscore_diacritic:1;
 _flags

 struct Interpret
 {
  flags_(1)
    bool use_refinements:1;
    bool use_numeral_diacritic:1;
    bool use_underscore_diacritic:1;
  _flags
 };

 Interpret interpret;

 QChar chr;
 QString str; 
 void* external_extended; 
 u2 external_deck_code;
 u2 external_diacritic_code;
 n8 glyph_code;
 GlyphDeck_Base_8b* internal_deck;
 n8 screened_code;

//? Diacritic_GlyphDeck_Base* internal_diacritic_deck;

 Glyph_Argument_Package();

 void reset_all();
 void reset();

// bool no_flags()
// {
//  return (Flags & 0b0001 1111 1111 1111) == 0;
// }

};

_KANS(HTXN)

#endif // GLYPH_VECTOR_8B__H
