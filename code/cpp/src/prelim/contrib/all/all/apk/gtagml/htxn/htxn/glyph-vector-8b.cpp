

//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "glyph-vector-8b.h"

#include "glyphdeck-base-8b.h"

#include "glyph-argument-package.h"

USING_KANS(HTXN)

void Glyph_Vector_8b::add_diacritic(u4 index, u2 deck_code, u2 dia_code)
{
 dia_map_.insert(index, {deck_code, dia_code});
}


//void Glyph_Vector_8b::check_external_excluding_numeral_diacritic(u4 index,
//  GlyphDeck_Base_8b& current_deck, Glyph_Argument_Package& gap)
//{
// u1 gp = value(index);
// current_deck.check_external_excluding_numeral_diacritic(gp, gap);
// check_external(index, gp, gap);
//}


void Glyph_Vector_8b::check_external(u4 index, 
  GlyphDeck_Base_8b& current_deck, Glyph_Argument_Package& gap)
{
 u1 gp = value(index);
 current_deck.check_external(gp, gap);
 check_external(index, gp, gap);
}

void Glyph_Vector_8b::check_external(u4 index, u1 gp, Glyph_Argument_Package& gap)
{
 if(gap.flags.confirmed_non_diacritic)
 {
  gap.glyph_code = gp;
  return;
 }
 if(gap.flags.maybe_external_deck)
 {
  auto it = deck_map_.find(index);
  if(it == deck_map_.end())
  {
   auto iit = xx_map_.find(index);
   if(iit == xx_map_.end())
   {
    gap.flags.maybe_external_range = true;
    // look for a range ...
   }
   else
   {
    gap.flags.confirmed_external_extended = true;
    gap.flags.maybe_external_deck = false;
    gap.external_extended = iit.value();
   }
  }
  else
  {
   gap.flags.confirmed_external_deck = true;
   gap.flags.request_deck_resolve = true;
   gap.external_deck_code = it.value();    
   //gap.glyph_code = (u8) 
  }
  return;
 }
 if(gap.flags.maybe_external_diacritic)
 {
  auto it = dia_map_.find(index);
  if(it == dia_map_.end())
  {
   gap.flags.maybe_external_diacritic = false;
   gap.flags.confirmed_non_diacritic = true;
   gap.glyph_code = gp;
  }
  else
  {
   QPair<u2, u2> pr = it.value();
   gap.flags.confirmed_external_diacritic = true;
   gap.external_deck_code = pr.first;
   gap.external_diacritic_code = pr.second;
  }
  return;
 }
 gap.glyph_code = gp;
}



