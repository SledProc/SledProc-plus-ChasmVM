
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "glyph-layers-8b.h"

#include "glyph-vector-8b.h"

#include "glyph-argument-package.h"

#include "glyph-layer-8b.h"

#include "glyphdeck-base-8b.h"

#include "diacritic-glyphdeck-base.h"

#include "kans.h"

#include <QString>
#include <QList>
#include <QVector>

#include <QTextStream>

#include <functional>

//class WCM_WhiteDB;

USING_KANS(HTXN)

Glyph_Layers_8b::Glyph_Layers_8b()
{

}


void Glyph_Layers_8b::get_htxne_out(Glyph_Vector_8b& gv, 
  u4 index, Glyph_Argument_Package& gap)
{
 GlyphDeck_Base_8b& deck = *gap.internal_deck;
 //?Diacritic_GlyphDeck_Base& dia = *gap.internal_diacritic_deck;

 gv.check_external(index, deck, gap);

// if(gap.no_flags())
// {
//  deck.get_htxne_out((u1)gap.glyph_code, gap);
// }

 if(gap.flags.normal)
   deck.get_htxne_out((u1)gap.glyph_code, gap);
 else if(gap.flags.confirmed_non_diacritic)
 {
  deck.get_htxne_out((u1)gap.glyph_code, gap);
 }
 else if(gap.flags.confirmed_external_diacritic)
 {
  u2 edc = gap.external_deck_code;
  u2 code = gap.external_diacritic_code;
  Diacritic_GlyphDeck_Base* dia = 
    decks_by_id_[edc - 1].dia;
   
  char cc = deck.get_char_code((u1)gap.glyph_code);
  
  dia->get_full_htxne_representation(code,
    QChar::fromLatin1(cc), gap.chr, gap.str);

 // QString get_htxne_out(u1 gp, char cue);
 // dia.get_htxne_out(gap.glyph_code, 
 }



}


void Glyph_Layers_8b::get_qstring_out(Glyph_Vector_8b& gv, 
  u4 index, Glyph_Argument_Package& gap)
{
 GlyphDeck_Base_8b& deck = *gap.internal_deck;
// if(numeral_diacritic_code_)
//   gv.check_external(index, deck, gap);
// else
//   ;//gv.check_external_excluding_numeral_diacritic(index, deck, gap);
 gv.check_external(index, deck, gap);
 if(gap.flags.normal)
 {
  deck.get_qstring_out((u1)gap.glyph_code, gap);
 }
 else if(gap.flags.confirmed_non_diacritic)
 {
  deck.get_qstring_out((u1)gap.glyph_code, gap);
 }
 else if(gap.flags.confirmed_external_diacritic)
 {
  char cc = deck.get_char_code((u1)gap.glyph_code);
  gap.chr = QChar::fromLatin1(cc);
 }

#ifdef HIDE
 if(gap.flags.request_deck_resolve)
 {
  auto it = decks_by_id_.find(gap.external_deck_code);
  if(it == decks_by_id_.end())
  {
   return;
  }
  else
  {

  }
 }
 else if(gap.flags.request_deck_resolve)
#endif //def HIDE

}

void Glyph_Layers_8b::get_screened_code(Glyph_Vector_8b& gv,
  u4 index, Glyph_Argument_Package& gap)
{
 GlyphDeck_Base_8b& deck = *gap.internal_deck;
 gv.check_external(index, deck, gap);
 if(gap.flags.normal)
   deck.get_screened_code((u1)gap.screened_code, gap);
 else if(gap.flags.confirmed_non_diacritic)
 {
  gap.screened_code = deck.null_code();
 }
}

bool Glyph_Layers_8b::check_letter(Glyph_Vector_8b& gv,
  u4 index, Glyph_Argument_Package& gap)
{
 GlyphDeck_Base_8b& deck = *gap.internal_deck;
 gv.check_external(index, deck, gap);
 if(gap.flags.normal)
   return deck.check_letter((u1)gap.glyph_code);
 else if(gap.flags.confirmed_external_diacritic)
   return true;
 else if(gap.flags.confirmed_non_diacritic)
   return false;
 return false;
}

bool Glyph_Layers_8b::check_sentence_end_space(Glyph_Vector_8b& gv,
  u4 index, Glyph_Argument_Package& gap)
{
 GlyphDeck_Base_8b& deck = *gap.internal_deck;
 gv.check_external(index, deck, gap);
 if(gap.flags.normal)
   return deck.check_sentence_end_space((u1)gap.glyph_code);
 else if(gap.flags.confirmed_non_diacritic)
   return deck.check_sentence_end_space((u1)gap.glyph_code);
 return false;
}

bool Glyph_Layers_8b::check_sentence_end_marker(Glyph_Vector_8b& gv,
  u4 index, Glyph_Argument_Package& gap)
{
 GlyphDeck_Base_8b& deck = *gap.internal_deck;
 gv.check_external(index, deck, gap);
 if(gap.flags.normal)
   return deck.check_sentence_end_marker((u1)gap.glyph_code);
 else if(gap.flags.confirmed_non_diacritic)
   return deck.check_sentence_end_marker((u1)gap.glyph_code);
 return false;
}

void Glyph_Layers_8b::get_xml_out(Glyph_Vector_8b& gv,
  u4 index, Glyph_Argument_Package& gap)
{
 GlyphDeck_Base_8b& deck = *gap.internal_deck;
 gv.check_external(index, deck, gap);
 if(gap.flags.normal)
   deck.get_xml((u1)gap.glyph_code, gap);
 else if(gap.flags.confirmed_non_diacritic)
 {
  deck.get_xml_nondia((u1)gap.glyph_code, gap);
 }
 else if(gap.flags.confirmed_external_diacritic)
 {
  u2 edc = gap.external_deck_code;
  u2 code = gap.external_diacritic_code;
  Diacritic_GlyphDeck_Base* dia = 
    decks_by_id_[edc - 1].dia;
   
  char cc = deck.get_char_code((u1)gap.glyph_code);
  
  gap.str = dia->get_xml_out(code, cc);
 }
}


void Glyph_Layers_8b::get_latex_command_out(Glyph_Vector_8b& gv,
  u4 index, Glyph_Argument_Package& gap)
{
 get_latex_out(gv, index, gap);
} 

void Glyph_Layers_8b::get_latex_out(Glyph_Vector_8b& gv,
  u4 index, Glyph_Argument_Package& gap)
{
// Glyph_Vector_8b* vec = value(layer);
// if(!vec)
// {
//  gap.str = "?";
//   return;
// }

 GlyphDeck_Base_8b& deck = *gap.internal_deck;
 //?Diacritic_GlyphDeck_Base& dia = *gap.internal_diacritic_deck;

 gv.check_external(index, deck, gap);

// if(gap.no_flags())
// {
//  deck.get_htxne_out((u1)gap.glyph_code, gap);
// }

 if(gap.flags.normal)
   deck.get_latex((u1)gap.glyph_code, gap);
 else if(gap.flags.confirmed_non_diacritic)
 {
  deck.get_latex_nondia((u1)gap.glyph_code, gap);
 }
 else if(gap.flags.confirmed_external_diacritic)
 {
  u2 edc = gap.external_deck_code;
  u2 code = gap.external_diacritic_code;
  Diacritic_GlyphDeck_Base* dia = 
    decks_by_id_[edc - 1].dia;
   
  char cc = deck.get_char_code((u1)gap.glyph_code);
  
  gap.str = dia->get_latex_out(code, cc);

 // QString get_htxne_out(u1 gp, char cue);
 // dia.get_htxne_out(gap.glyph_code, 
 }




// vec->check_external(index, gap);
// if(gap.flags.request_deck_resolve)
// {
//  auto it = decks_by_id_.find(gap.external_deck_code);
//  if(it == decks_by_id_.end())
//  {
//   return
//  }
//  else
//  {

//  }

// }
}

KANS_(HTXN)

QTextStream& operator<<(QTextStream& qts, 
  const QVector<Glyph_Layer_8b*>& rhs)
{
 qts << rhs.size() << '\n';
 for(Glyph_Layer_8b* gl : rhs)
 {
  qts << '=' << gl->id() << '\n';
  gl->write_ranges(qts);
  qts << "!\n";
 }
 return qts;
}

QTextStream& operator>>(QTextStream& qts, 
  QVector<Glyph_Layer_8b*>& rhs)
{
 int sz;
 qts >> sz;
  // // read newline
 qts.read(1);
  //? rhs.resize(sz);
 for(Glyph_Layer_8b* gl : rhs)
 {
   // // read '='
  qts.read(1);
  u4 id;
  qts >> id;
     //? gl = new Glyph_Layer_8b(id);
   // // read newline
  qts.read(1);
  gl->read_ranges(qts);
   // // read newline
  qts.read(1);
 }
 return qts;
}

_KANS(HTXN)


