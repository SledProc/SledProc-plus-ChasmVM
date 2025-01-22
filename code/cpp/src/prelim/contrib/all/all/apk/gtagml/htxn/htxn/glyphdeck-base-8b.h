
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef GLYPHDECK_BASE_8B__H
#define GLYPHDECK_BASE_8B__H

#include "accessors.h"

#include "global-types.h"

#include "kans.h"

#include <QString>
#include <QList>
#include <QDate>

#include <functional>

//class WCM_WhiteDB;

KANS_(HTXN)

struct Glyph_Argument_Package; 

class GlyphDeck_Base_8b
{
protected:
 u2 deck_id_;
 
public:

 GlyphDeck_Base_8b();

 ACCESSORS(u2 ,deck_id)

// QChar get_latex(u1 gp, QString& extra);
// QChar get_xml(u1 gp, QString& extra);
// u1 get_diacritic_code(u1 gp);

 virtual void check_external(u1 gp, Glyph_Argument_Package& gap) = 0;
 //virtual void check_external_excluding_numeral_diacritic(u1 gp, Glyph_Argument_Package& gap) = 0;
 virtual void get_latex(u1 gp, Glyph_Argument_Package& gap) = 0;
 virtual void get_latex_nondia(u1 gp, Glyph_Argument_Package& gap) = 0;
 virtual void get_xml(u1 gp, Glyph_Argument_Package& gap) = 0;
 virtual void get_xml_nondia(u1 gp, Glyph_Argument_Package& gap) = 0;
 virtual void get_nondiacritic_supplement(u1 cue, QString& result) = 0;

 virtual char get_char_code(u1 gp) = 0;

 virtual void get_htxne_out(u1 gp, Glyph_Argument_Package& gap) = 0;
 virtual void get_qstring_out(u1 gp, Glyph_Argument_Package& gap) = 0;

 virtual void get_screened_code(u1 gp, Glyph_Argument_Package& gap) = 0;
 virtual u1 null_code() = 0;

 virtual void swap_false_sentence_end(u1& end) = 0;
 //virtual void swap_sentence_end(u1& e1, u1& e2) = 0;
 
 virtual void swap_sentence_end_space(u1& end) = 0;
 virtual bool check_swap_dot(u1& dot, bool to_non_punctuation) = 0;

 virtual bool check_letter(u1 gp) = 0;
 virtual bool check_sentence_end_marker(u1 gp) = 0;
 virtual bool check_sentence_end_space(u1 gp) = 0;

 virtual u1 get_standard_equivalent(u1 gp);
};

_KANS(HTXN)

#endif // GLYPHDECK_BASE_8B__H
