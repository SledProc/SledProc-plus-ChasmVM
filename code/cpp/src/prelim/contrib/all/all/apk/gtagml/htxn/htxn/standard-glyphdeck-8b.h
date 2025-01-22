
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef STANDARD_GLYPHDECK_8B__H
#define STANDARD_GLYPHDECK_8B__H

#include "accessors.h"

#include "global-types.h"

#include "kans.h"

#include "glyphdeck-base-8b.h"

#include <QString>
#include <QList>
#include <QDate>

#include <functional>

//class WCM_WhiteDB;

KANS_(HTXN)

struct Glyph_Argument_Package;

class Standard_GlyphDeck_8b : public GlyphDeck_Base_8b
{
 u2 deck_id_;

 QChar get_text_default(u1 cue);

 QChar get_nondiacritic_default(u1 cue);
 QString get_nondiacritic_xdefault(u1 cue);
 
public:

 enum Alt_Interpretation_Codes {
   Space = 63,
   NpMinus = 74,
   NpDot = 76,
   SnDash = 81,
   DashSML = 82,
   //?IndQm = 87,
   NsPer = 88,
   NsQm = 89,
   NsExc = 90,
   SnSp = 91,
   Boundary = 92,

   SpaceX = 93,
   DashX = 94,
   FslX = 95,
   OSqBrX = 96,
   CSqBrX = 97,
   OCyBrX = 98,
   CCyBrX = 99,

   Null = 100,
  // NullX = 95,

   TxtHatX = 101,
   NpCmaX = 102,
   SqSqX = 103,
   NpColX = 104,
   NpSemiX = 105,
   NpQmX = 106,

   BslX = 107,
   PipeX = 108,

   TxtNumX = 109,
   TxtDolX = 110,
   TxtPerX = 111,
   TxtPlusX = 112,
   TxtAmpX = 113,
   TxtStarX = 114,
   TxtEqX = 115,

   NpExcX = 116,
   NpOParX = 117,
   NpCParX = 118,

   NpDotX = 119,

   NpLtX = 120,
   NpGtX = 121,

   NmComX = 122,
   SqDqX = 123,

   TxtAtX = 124,
   BqX = 125,
   TildeX = 126,

 };

 Standard_GlyphDeck_8b();

 ACCESSORS(u2 ,deck_id)

 void check_external(u1 gp, Glyph_Argument_Package& gap) Q_DECL_OVERRIDE;
 //?void check_external_excluding_numeral_diacritic(u1 gp, Glyph_Argument_Package& gap) Q_DECL_OVERRIDE;
 void get_latex(u1 gp, Glyph_Argument_Package& gap) Q_DECL_OVERRIDE;

 void get_latex_nondia(u1 gp, Glyph_Argument_Package& gap) Q_DECL_OVERRIDE;

 void get_xml(u1 gp, Glyph_Argument_Package& gap) Q_DECL_OVERRIDE;

 void get_xml_nondia(u1 gp, Glyph_Argument_Package& gap) Q_DECL_OVERRIDE;

 void get_nondiacritic_supplement(u1 cue, QString& result) Q_DECL_OVERRIDE;

 void get_qstring_out(u1 gp, Glyph_Argument_Package& gap) Q_DECL_OVERRIDE;

 void get_htxne_out(u1 gp, Glyph_Argument_Package& gap) Q_DECL_OVERRIDE;

 char get_char_code(u1 gp) Q_DECL_OVERRIDE;

 void get_screened_code(u1 gp, Glyph_Argument_Package& gap) Q_DECL_OVERRIDE;
 u1 null_code() Q_DECL_OVERRIDE;

 u1 get_standard_equivalent(u1 gp) Q_DECL_OVERRIDE
 {
  return gp;
 }
 
 void swap_false_sentence_end(u1& end) Q_DECL_OVERRIDE;
 void swap_sentence_end_space(u1& space) Q_DECL_OVERRIDE;
 bool check_swap_dot(u1& end, bool to_non_punctuation) Q_DECL_OVERRIDE;

 bool check_letter(u1 gp) Q_DECL_OVERRIDE;
 bool check_sentence_end_marker(u1 gp) Q_DECL_OVERRIDE;
 bool check_sentence_end_space(u1 gp) Q_DECL_OVERRIDE;

// void get_htxne_dia(u1 gp, Glyph_Argument_Package& gap) Q_DECL_OVERRIDE;

// virtual void check_external(u1 gp, Glyph_Argument_Package& gap);
// virtual void get_latex(u1 gp, Glyph_Argument_Package& gap);
// virtual void get_latex_dia(u1 gp, Glyph_Argument_Package& gap);
// virtual void get_xml(u1 gp, Glyph_Argument_Package& gap);
// virtual void get_xml_dia(u1 gp, Glyph_Argument_Package& gap);

// virtual void get_qstring_out(u1 gp, Glyph_Argument_Package& gap);
};

_KANS(HTXN)

#endif // STANDARD_GLYPHDECK
