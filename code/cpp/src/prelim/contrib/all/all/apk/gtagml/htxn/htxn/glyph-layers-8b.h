
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef GLYPH_LAYERS_8B__H
#define GLYPH_LAYERS_8B__H

#include "accessors.h"

#include "global-types.h"

#include "kans.h"

#include <QString>
#include <QList>
#include <QVector>

#include <functional>

class QTextStream;

KANS_(HTXN)

class GlyphDeck_Base_8b;
class Glyph_Layer_8b;
struct Glyph_Argument_Package;
class Glyph_Vector_8b;

class Diacritic_GlyphDeck_Base;


class Glyph_Layers_8b : public QVector<Glyph_Layer_8b*>
{
protected:

 union generic_glyph_base
 {
  GlyphDeck_Base_8b* deck; Diacritic_GlyphDeck_Base* dia;
 };


 QVector<generic_glyph_base> decks_by_id_;

 QMap<GlyphDeck_Base_8b*, u4> id_by_deck_;
 QMap<Diacritic_GlyphDeck_Base*, u4> dia_id_by_deck_;

public:

 Glyph_Layers_8b();

// void get_latex(u4 layer, u4 index, Glyph_Argument_Package& gap);

 void get_screened_code(Glyph_Vector_8b& gv,
   u4 index, Glyph_Argument_Package& gap);

 void get_qstring_out(Glyph_Vector_8b& gv,
   u4 index, Glyph_Argument_Package& gap);

 void get_latex_out(Glyph_Vector_8b& gv,
   u4 index, Glyph_Argument_Package& gap);

 void get_xml_out(Glyph_Vector_8b& gv,
   u4 index, Glyph_Argument_Package& gap);

 void get_latex_command_out(Glyph_Vector_8b& gv,
   u4 index, Glyph_Argument_Package& gap);

 void get_htxne_out(Glyph_Vector_8b& gv,
   u4 index, Glyph_Argument_Package& gap);

 bool check_letter(Glyph_Vector_8b& gv,
   u4 index, Glyph_Argument_Package& gap);

 bool check_sentence_end_marker(Glyph_Vector_8b& gv,
   u4 index, Glyph_Argument_Package& gap);

 bool check_sentence_end_space(Glyph_Vector_8b& gv,
   u4 index, Glyph_Argument_Package& gap);

 //virtual void check_external(u1 gp, Glyph_Argument_Package& gap); 

};


QTextStream& operator<<(QTextStream& qts, 
  const QVector<Glyph_Layer_8b*>& rhs);

QTextStream& operator>>(QTextStream& qts, 
  QVector<Glyph_Layer_8b*>& rhs);


_KANS(HTXN)

#endif // GLYPH_VECTOR_8B__H
