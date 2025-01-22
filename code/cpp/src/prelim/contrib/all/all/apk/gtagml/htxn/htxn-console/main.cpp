
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include <QDebug>

#include "htxn/htxn-document-8b.h"
#include "htxn/glyph-layer-8b.h"

#include <QFile>

USING_KANS(HTXN)


int main(int argc, char *argv[])
{
 HTXN_Document_8b doc;

 doc.add_standard_deck();
 doc.add_standard_diacritic_deck();

 QFile file(DEFAULT_HTXN_READ_FOLDER "/test/t1.txt");
 if(file.open(QIODevice::ReadOnly | QIODevice::Text))
 {
  doc.read_htxne_in(file); 
 }

 QString out;
 doc.get_qstring_out(0, out);
 qDebug() << out;
 out.clear();

 QFile file1(DEFAULT_HTXN_READ_FOLDER "/test/t1a.txt");
 if(file1.open(QIODevice::WriteOnly | QIODevice::Text))
 {
  doc.write_htxne_out(file1); 
 }

 QFile file2(DEFAULT_HTXN_READ_FOLDER "/test/t1.htxne.tex");
 if(file2.open(QIODevice::WriteOnly | QIODevice::Text))
 {
  QTextStream qts(&file2);
  doc.write_latex_out(qts);
 }

 return 0;
}

int main1(int argc, char *argv[])
{
 HTXN_Document_8b doc;

 doc.add_standard_deck();
 doc.add_standard_diacritic_deck();

 //
 Glyph_Layer_8b* gl1 = doc.read_layer("This is a test layer.  The end.");

 gl1->set_description("Main");
 gl1->flags.main = true;

 Glyph_Layer_8b* gl2 = doc.read_layer
   ("documentclassdocumenti");
 gl2->set_description("Latex");

 Glyph_Layer_8b* gl3 = doc.read_layer
   ("article");
 gl3->set_description("Latex");
 
// u4 rc1 = doc.add_detail_range_region(gl2, 22, 29);
// gl1->add_range(2, 32, rc1);

 u4 rc1 = doc.add_detail_range(gl2, 2, 14, HTXN_Node_Detail::Wrap_Mode_Indicator_Codes::Normal);
 u4 rc11 = doc.add_detail_range(gl3, 2, 9, HTXN_Node_Detail::Wrap_Mode_Indicator_Codes::Normal);
 doc.tie_detail_range_preempt(rc1, rc11);

 gl1->add_range(2, 0, rc1);

 u4 rc2 = doc.add_detail_range_region(gl2, 15, 22, HTXN_Node_Detail::Wrap_Mode_Indicator_Codes::Normal);
 gl1->add_range(2, 32, rc2);


 u4 rc3 = doc.add_detail_range(gl2, 23, 23, HTXN_Node_Detail::Wrap_Mode_Indicator_Codes::Normal);
 gl1->add_range(7, 8, rc3);

 //doc.read_layer("Th|`is.");

// QString out;

 QFile file(DEFAULT_HTXN_READ_FOLDER "/test/t1.txt");
 if(file.open(QIODevice::WriteOnly | QIODevice::Text))
 {
  doc.write_htxne_out(file); 
 }

//? doc.get_qstring_out(0, out);
//? qDebug() << out;
//? out.clear();

//? doc.get_htxne_out(0, out);
//? qDebug() << out;
//? 
//out.clear();

//doc.get_htxne_out(1, out);
//qDebug() << out;
//out.clear();
//doc.get_htxne_out(2, out);
//qDebug() << out;
//out.clear();

// doc.get_latex_out(0, out);
// qDebug() << out;

 return 0;
}



// case '0': return 0;
// case '1': return 1;
// case '2': return 2;
// case '3': return 3;
// case '4': return 4;
// case '5': return 5;
// case '6': return 6;
// case '7': return 7;
// case '8': return 8;
// case '9': return 9;

// case 'a': return 10;
// case 'b': return 11;
// case 'c': return 12;
// case 'd': return 13;
// case 'e': return 14;
// case 'f': return 15;
// case 'g': return 16;
// case 'h': return 17;
// case 'i': return 18;
// case 'j': return 19;
// case 'k': return 20;
// case 'l': return 21;
// case 'm': return 22;
// case 'n': return 23;
// case 'o': return 24;
// case 'p': return 25;
// case 'q': return 26;
// case 'r': return 27;
// case 's': return 28;
// case 't': return 29;
// case 'u': return 30;
// case 'v': return 31;
// case 'w': return 32;
// case 'x': return 33;
// case 'y': return 34;
// case 'z': return 35;

// case '_': return 36;

// case 'A': return 37;
// case 'B': return 38;
// case 'C': return 39;
// case 'D': return 40;
// case 'E': return 41;
// case 'F': return 42;
// case 'G': return 43;
// case 'H': return 44;
// case 'I': return 45;
// case 'J': return 46;
// case 'K': return 47;
// case 'L': return 48;
// case 'M': return 49;
// case 'N': return 50;
// case 'O': return 51;
// case 'P': return 52;
// case 'Q': return 53;
// case 'R': return 54;
// case 'S': return 55;
// case 'T': return 56;
// case 'U': return 57;
// case 'V': return 58;
// case 'W': return 59;
// case 'X': return 60;
// case 'Y': return 61;
// case 'Z': return 62;


