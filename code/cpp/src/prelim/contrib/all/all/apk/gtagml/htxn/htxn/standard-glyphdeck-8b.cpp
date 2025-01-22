
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "standard-glyphdeck-8b.h"

#include "standard-glyphpack-8b.h"

#include "glyph-argument-package.h"


USING_KANS(HTXN)

Standard_GlyphDeck_8b::Standard_GlyphDeck_8b()
 :  deck_id_(0)
{

}

//void Standard_GlyphDeck_8b::check_external_excluding_numeral_diacritic
//  (u1 gp, Glyph_Argument_Package& gap)
//{
// if( (gp > 63) && (gp < 74) )
//   gap.glyph_code = (n8) gp;
// else if(gp == 100)
//   gap.glyph_code = (n8) gp;
// else
//   check_external(gp, gap);
//}

void Standard_GlyphDeck_8b::get_htxne_out(u1 gp, Glyph_Argument_Package& gap)
{
 if(gp < 64)
 {
  gap.chr = get_text_default(gp);
  return;
 }
 gap.chr = get_nondiacritic_default(gp & 63);
 
 // //  get alt number 
 static QMap<u1, u1> static_map {
  { Standard_GlyphDeck_8b::NpDot, 1 },
  { Standard_GlyphDeck_8b::NsPer, 2 },
  { Standard_GlyphDeck_8b::NpExcX, 1 },
  { Standard_GlyphDeck_8b::NpOParX, 1 },
  { Standard_GlyphDeck_8b::NpCParX, 1 },

  { Standard_GlyphDeck_8b::DashSML, 1 },
  { Standard_GlyphDeck_8b::SnDash, 2 },
  { Standard_GlyphDeck_8b::NpMinus, 3 },
  { Standard_GlyphDeck_8b::DashX, 4 },

  { Standard_GlyphDeck_8b::NmComX, 1 },
  { Standard_GlyphDeck_8b::SqSqX, 1 },
  { Standard_GlyphDeck_8b::TxtNumX, 1 },
  { Standard_GlyphDeck_8b::TxtDolX, 1 },
  { Standard_GlyphDeck_8b::TxtPerX, 1 },
  { Standard_GlyphDeck_8b::TxtAmpX, 1 },
  { Standard_GlyphDeck_8b::SqDqX, 1 },
  { Standard_GlyphDeck_8b::TxtStarX, 1 },
  { Standard_GlyphDeck_8b::TxtPlusX, 1 },

  { Standard_GlyphDeck_8b::FslX, 1 },
  { Standard_GlyphDeck_8b::SpaceX, 2},
  { Standard_GlyphDeck_8b::Boundary, 3 },
  { Standard_GlyphDeck_8b::Null, 4 },

  { Standard_GlyphDeck_8b::NpColX, 1 },
  { Standard_GlyphDeck_8b::NpSemiX, 1 },
  { Standard_GlyphDeck_8b::NpQmX, 1 },
//?  { Standard_GlyphDeck_8b::IndQm, 1 },
  { Standard_GlyphDeck_8b::NpLtX, 1 },
  { Standard_GlyphDeck_8b::NpGtX, 1 },
  { Standard_GlyphDeck_8b::OSqBrX, 1 },
  { Standard_GlyphDeck_8b::CSqBrX, 1 },
  { Standard_GlyphDeck_8b::OCyBrX, 1 },
  { Standard_GlyphDeck_8b::CCyBrX, 1 },
  { Standard_GlyphDeck_8b::BslX, 1 },
  //?{ Standard_GlyphDeck_8b::NullX, 1 },
 };

 u1 alt_code = static_map.value(gap.glyph_code);
 switch(alt_code)
 {
 default: // // should always be 0 - 4 ...
 case 0:
  break;
 case 1:
  {
   gap.flags.has_alt = true;
   gap.flags.alt_1 = true; break;
  }
 case 2:
  {
   gap.flags.has_alt = true;
   gap.flags.alt_2 = true; break;
  }
 case 3:
  {
   gap.flags.has_alt = true;
   gap.flags.alt_3 = true; break;
  }
 case 4:
  {
   gap.flags.has_alt = true;
   gap.flags.alt_4 = true; break;
  }
 }

} 

void Standard_GlyphDeck_8b::check_external(u1 gp, Glyph_Argument_Package& gap)
{
 if( (gp > 63) && (gp < 74) && (!gap.interpret.flags.use_numeral_diacritic) )
 {
  gap.flags.confirmed_non_diacritic = true;
  //gap.flags.normal = true;
  gap.glyph_code = gp & 63;
  return;
 }
 if( (gp == 100) && (!gap.interpret.flags.use_underscore_diacritic) )
 {
  gap.flags.normal = true;
  return;
 }
 if( (gp > 100) && (gp < 123) && gap.interpret.flags.use_refinements )
 {
  gap.flags.maybe_refinement = true;
 }
 if( (gp & 128) > 0 )
 {
  gap.flags.maybe_external_deck = true;
  // // provisional; may revert to gp ...
  gap.glyph_code = (n8) gp & 127;
  return;
 }
 if( (gp & 64) > 0 )
 {
  gap.flags.maybe_external_diacritic = true;
  // // provisional; may revert to gp ...
  gap.glyph_code = (n8) gp & 63;
  return;
 }
 gap.flags.normal = true;
}

void Standard_GlyphDeck_8b::get_qstring_out(u1 gp, Glyph_Argument_Package& gap)
{
 if(gp < 64)
   gap.chr = get_text_default(gp);
 else if(gp == Boundary)
   gap.chr = '\n';
 else
   gap.chr = get_nondiacritic_default(gp - 64);
}


char Standard_GlyphDeck_8b::get_char_code(u1 gp)
{
 gp &= 63;
 switch(gp)
 {
 case 0: return '0';
 case 1: return '1';
 case 2: return '2';
 case 3: return '3';
 case 4: return '4';
 case 5: return '5';
 case 6: return '6';
 case 7: return '7';
 case 8: return '8';
 case 9: return '9';
 case 10: return 'a';
 case 11: return 'b';
 case 12: return 'c';
 case 13: return 'd';
 case 14: return 'e';
 case 15: return 'f';
 case 16: return 'g';
 case 17: return 'h';
 case 18: return 'i';
 case 19: return 'j';
 case 20: return 'k';
 case 21: return 'l';
 case 22: return 'm';
 case 23: return 'n';
 case 24: return 'o';
 case 25: return 'p';
 case 26: return 'q';
 case 27: return 'r';
 case 28: return 's';
 case 29: return 't';
 case 30: return 'u';
 case 31: return 'v';
 case 32: return 'w';
 case 33: return 'x';
 case 34: return 'y';
 case 35: return 'z';
 case 36: return '_';
 case 37: return 'A';
 case 38: return 'B';
 case 39: return 'C';
 case 40: return 'D';
 case 41: return 'E';
 case 42: return 'F';
 case 43: return 'G';
 case 44: return 'H';
 case 45: return 'I';
 case 46: return 'J';
 case 47: return 'K';
 case 48: return 'L';
 case 49: return 'M';
 case 50: return 'N';
 case 51: return 'O';
 case 52: return 'P';
 case 53: return 'Q';
 case 54: return 'R';
 case 55: return 'S';
 case 56: return 'T';
 case 57: return 'U';
 case 58: return 'V';
 case 59: return 'W';
 case 60: return 'X';
 case 61: return 'Y';
 case 62: return 'Z';
 case 63: return ' ';
 default: return '\0'; 
 }

}

QChar Standard_GlyphDeck_8b::get_text_default(u1 cue)
{
 switch(cue)
 {
 case 0: return QChar('0');
 case 1: return QChar('1');
 case 2: return QChar('2');
 case 3: return QChar('3');
 case 4: return QChar('4');
 case 5: return QChar('5');
 case 6: return QChar('6');
 case 7: return QChar('7');
 case 8: return QChar('8');
 case 9: return QChar('9');
 case 10: return QChar('a');
 case 11: return QChar('b');
 case 12: return QChar('c');
 case 13: return QChar('d');
 case 14: return QChar('e');
 case 15: return QChar('f');
 case 16: return QChar('g');
 case 17: return QChar('h');
 case 18: return QChar('i');
 case 19: return QChar('j');
 case 20: return QChar('k');
 case 21: return QChar('l');
 case 22: return QChar('m');
 case 23: return QChar('n');
 case 24: return QChar('o');
 case 25: return QChar('p');
 case 26: return QChar('q');
 case 27: return QChar('r');
 case 28: return QChar('s');
 case 29: return QChar('t');
 case 30: return QChar('u');
 case 31: return QChar('v');
 case 32: return QChar('w');
 case 33: return QChar('x');
 case 34: return QChar('y');
 case 35: return QChar('z');
 case 36: return QChar('_');
 case 37: return QChar('A');
 case 38: return QChar('B');
 case 39: return QChar('C');
 case 40: return QChar('D');
 case 41: return QChar('E');
 case 42: return QChar('F');
 case 43: return QChar('G');
 case 44: return QChar('H');
 case 45: return QChar('I');
 case 46: return QChar('J');
 case 47: return QChar('K');
 case 48: return QChar('L');
 case 49: return QChar('M');
 case 50: return QChar('N');
 case 51: return QChar('O');
 case 52: return QChar('P');
 case 53: return QChar('Q');
 case 54: return QChar('R');
 case 55: return QChar('S');
 case 56: return QChar('T');
 case 57: return QChar('U');
 case 58: return QChar('V');
 case 59: return QChar('W');
 case 60: return QChar('X');
 case 61: return QChar('Y');
 case 62: return QChar('Z');
 case 63: return QChar(' ');
 default: return QChar(); 
 }
}

void Standard_GlyphDeck_8b::get_xml_nondia(u1 gp, Glyph_Argument_Package& gap)
{
 // gap.chr = get_nondiacritic_default(gp & 63);
 static QMap<u1, QString> static_map {
   {Alt_Interpretation_Codes::Boundary, {}},
   {Alt_Interpretation_Codes::SnDash, "&emdash;"},
 };
 auto it = static_map.find(gp);
 if(it == static_map.end())
   gap.chr = get_nondiacritic_default(gp & 63);
 else
   gap.str = *it;
}



void Standard_GlyphDeck_8b::get_latex_nondia(u1 gp, Glyph_Argument_Package& gap)
{
 static QMap<u1, QString> static_map {
   {Alt_Interpretation_Codes::Boundary, {}},
   {Alt_Interpretation_Codes::SnDash, "{\\mdash}"},
 };
 auto it = static_map.find(gp);
 if(it == static_map.end())
   gap.chr = get_nondiacritic_default(gp & 63);
 else
   gap.str = *it;
}

#ifdef HIDE
QString Standard_GlyphDeck_8b::get_nondiacritic_xdefault_latex(u1 cue)
{
 switch(cue)
 {
 case 0: return QString();
 case 1: return QString();
 case 2: return QString("{\\DLi}");
 case 3: return QString("{\\DLj}");
 case 4: return QString("{\\dqDCe}");
 case 5: return QString("{\\dqDCl}");
 case 6: return QString("{\\sqDCe}");
 case 7: return QString("{\\sqDCl}");
 case 8: return QString("{\\emDCe}");
 case 9: return QString("{\\emDCl}");
 case 10: return QString("{\\bfDCe}");
 case 11: return QString("{\\bfDCl}");
 case 12: return QString("{\\ulDCe}");
 case 13: return QString("{\\ulDCl}");
 case 14: return QString("{\\dcIne}");
 case 15: return QString("{\\dcInl}");
 case 16: return QString("{\\dcAve}");
 case 17: return QString("{\\dcAvl}");
 case 18: return QString("{\\dcIAe}");
 case 19: return QString("{\\dcIAl}");
 case 20: return QString("{\\dcHve}");
 case 21: return QString("{\\dcHvl}");
 case 22: return QString("{\\sDiv}");
 case 23: return QString("{\\mDiv}");
 case 24: return QString("{\\lDiv}");
 case 25: return QString("{\\emD}");
 case 26: return QString("{\\enD}");
 case 27: return QString("{\\num}");
 case 28: return QString("{\\dol}");
 case 29: return QString("{\\prc}");
 case 30: return QString("{\\amp}");
 case 31: return QString("{\\sct}");
 default: return QChar(); 
}
#endif //def HIDE

#ifdef HIDE
 case 4: return QString("{\\dqIne}");
 case 5: return QString("{\\dqInl}");
 case 6: return QString("{\\dqAve}");
 case 7: return QString("{\\dqAvl}");
 case 8: return QString("{\\dqHve}");
 case 9: return QString("{\\dqHvl}");
 case 10: return QString("{\\dqIAe}");
 case 11: return QString("{\\dqIAl}");
 case 12: return QString("{\\sqIne}");
 case 13: return QString("{\\sqInl}");
 case 14: return QString("{\\sqAve}");
 case 15: return QString("{\\dqAvl}");
 case 16: return QString("{\\sqHve}");
 case 17: return QString("{\\sqHvl}");
 case 18: return QString("{\\sqIAe}");
 case 19: return QString("{\\sqIAl}");
#endif // HIDE



void Standard_GlyphDeck_8b::swap_false_sentence_end(u1& end)
{
 u1 cue = end & 63;
 static QMap<u1, u1> subs {
  {0, 12 | 63},
  {1 , 25 | 63},
  {9 , 26 | 63}
 };
 end = subs.value(cue, end);   
}

void Standard_GlyphDeck_8b::swap_sentence_end_space(u1& space)
{
 if( (space == 63) || (space & 63) == 63 )
   space = 27 | 63;
}

//void Standard_GlyphDeck_8b::swap_sentence_end(u1& e1, u1& e2)
//{
// if(e1 == 64)
//   e1 = (12 | 63);
// else 
//   swap_false_sentence_end(e2);
//}

bool Standard_GlyphDeck_8b::check_swap_dot(u1& dot, bool to_non_punctuation)
{
 if(dot == 64)
 {
  dot = to_non_punctuation? 
    (12 | 63) : (24 | 63);
  return true;
 }
 return false;
}

bool Standard_GlyphDeck_8b::check_letter(u1 gp)
{
 if(gp >= 63)
   return false;
 if(gp < 10) // 0-9
   return false;
 if(gp < 36) // a-z
   return true;
 if(gp == 36)
   return false;
 if(gp < 63) // A-Z
   return true; 
}

bool Standard_GlyphDeck_8b::check_sentence_end_marker(u1 gp)
{
 if(gp <= 63)
   return false;
 gp &= 63;
 return (gp == 0) // .
   || (gp == 1) // !
   || (gp == 9); // ?  
}

bool Standard_GlyphDeck_8b::check_sentence_end_space(u1 gp)
{
 return gp == (27 | 63);
}

QChar Standard_GlyphDeck_8b::get_nondiacritic_default(u1 cue)
{
 switch(cue)
 {
 case 0: return QChar('.'); // // 64
 case 1: return QChar('!'); // // 65
 case 2: return QChar('('); // // 66
 case 3: return QChar(')'); // // 67
 case 4: return QChar('-'); // // 68  // hyphen
 case 5: return QChar(','); // // 69
 case 6: return QChar('\''); // // 70
 case 7: return QChar(':'); // // 71
 case 8: return QChar(';'); // // 72
 case 9: return QChar('?'); // // 73

 case 10: return QChar('-'); // // 74  // NpMinus
 case 11: return QChar('+'); // // 75

 case 12: return QChar('.'); // // 76  // NpDot
 case 13: return QChar('#'); // // 77
 case 14: return QChar('$'); // // 78
 case 15: return QChar('%'); // // 79
 case 16: return QChar('&'); // // 80
 case 17: return QChar('-'); // // 81 // SnDash
 case 18: return QChar('-'); // // 82 // DashSML
 case 19: return QChar('='); // // 83

 case 20: return QChar('['); // // 84
 case 21: return QChar(']'); // // 85

 case 22: return QChar('/'); // // 86
 case 23: return QChar('\\'); // // 87

 case 24: return QChar('.'); // // 88  // NsPer
 case 25: return QChar('?'); // // 89  // NsQm
 case 26: return QChar('!'); // // 90  // NsExc
 case 27: return QChar(' '); // // 91  // SnSp

 case 28: return QChar('/'); // // 92 // Boundary

 case 29: return QChar(); // // 93 // SpaceX

 case 30: return QChar('-'); // // 94  // DashX
 case 31: return QChar('/'); // // 95  // FslX

 case 32: return QChar('['); // // 96  // OSqBrX
 case 33: return QChar(']'); // // 97  // CSqBrX


 case 34: return QChar('{'); // // 98  // OCyBrX
 case 35: return QChar('}'); // // 99  // CCyBrX

 case 36: return QChar(); // // 100  // Null

 case 37: return QChar('^'); // // 101  // TxtHatX
 case 38: return QChar(','); // // 102  // NpCmaX
 case 39: return QChar('\''); // // 103  // SqSqX
 case 40: return QChar(':'); // // 104  // NpColX
 case 41: return QChar(';'); // // 105  // NpSemX
 case 42: return QChar('?'); // // 106  // NpQmX

 case 43: return QChar('\\'); // // 107  // BslX
 case 44: return QChar('|'); // // 108  // PipeX 

 case 45: return QChar('#'); // // 109  // TxtNumX
 case 46: return QChar('$'); // // 110  // TxtDolX
 case 47: return QChar('%'); // // 111  // TxtPerX
 case 48: return QChar('+'); // // 112  // TxtPlusX
 case 49: return QChar('&'); // // 113  // TxtAmpX

 case 50: return QChar('*'); // // 114  // TxtStarX
 case 51: return QChar('='); // // 115  // TxtEqX

 case 52: return QChar('!'); // // 116  // NpExcX
 case 53: return QChar('('); // // 117  // NpOParX
 case 54: return QChar(')'); // // 118  // NpCParX

 case 55: return QChar('.'); // // 119  // NmDotX
 case 56: return QChar('<'); // // 120  // NpLtX
 case 57: return QChar('>'); // // 121  // NpGtX
 case 58: return QChar(','); // // 122  // NmComX
 case 59: return QChar('"'); // // 123  // SqDqX

 case 60: return QChar('@'); // // 124  // TxtAtX
 case 61: return QChar('`'); // // 125  // BqX
 case 62: return QChar('~'); // // 126  // TildeX
 case 63: return QChar('\n'); // // 127

 default: return QChar(); 
 }
}

void Standard_GlyphDeck_8b::get_screened_code(u1 gp, Glyph_Argument_Package& gap)
{
 if(gp < 64)
 {
  gap.screened_code = gp;
  return;
 }
 gp &= 63;
 if(gp < 29)
   gap.screened_code = gp | 63; 
 else if(gp == 63)
   gap.screened_code = gp | 63;
 else
   gap.screened_code = 100;
}

u1 Standard_GlyphDeck_8b::null_code()
{
 return 100;
}


void Standard_GlyphDeck_8b::get_nondiacritic_supplement(u1 cue, QString& result)
{
 static QVector<QString> static_vec {
  "", //0: return QChar('.'); // // 65
  "", //1: return QChar('!'); // // 65
  "", //2: return QChar('('); // // 66
  "", //3: return QChar(')'); // // 67
  "", //4: return QChar(','); // // 68
  "", //5: return QChar('~'); // // 69
  "", //6: return QChar('\''); // // 70
  "", //7: return QChar(':'); // // 71
  "", //8: return QChar(';'); // // 72
  "", //9: return QChar('?'); // // 73
  "", //10: return QChar('-'); // // 74

  "", //11: return QChar('+'); // // 75
  "", //12: return QChar('@'); // // 76
  "", //13: return QChar('#'); // // 77
  "", //14: return QChar('$'); // // 78
  "", //15: return QChar('%'); // // 79
  "", //16: return QChar('^'); // // 80
  "", //17: return QChar('&'); // // 81
  "", //18: return QChar('*'); // // 82
  "", //19: return QChar('='); // // 83

  "", //20: return QChar('{'); // // 84
  "", //21: return QChar('}'); // // 85
  "", //22: return QChar('['); // // 86
  "", //23: return QChar(']'); // // 87
  "", //24: return QChar('<'); // // 88
  "", //25: return QChar('>'); // // 89
  "", //26: return QChar('\\'); // // 90
  "", //27: return QChar('/'); // // 91
  "", //28: return QChar('|'); // // 92
  "", //29: return QChar('"'); // // 93
  "", //30: return QChar('~'); // // 94
  "", //31: return QChar('`'); // // 95
 };
 
}


void Standard_GlyphDeck_8b::get_latex(u1 gp, Glyph_Argument_Package& gap)
{
 Standard_GlyphPack_8b code{gp}; 
 u1 kind, cue;
 std::tie(kind, cue) = code.get_latex_cue();

 switch(kind)
 {
 case 0: gap.chr = get_text_default(cue); break;
// case 1: case 3: case 4: dia = cue; return QChar();
// case 2:
 }
}


//void Standard_GlyphDeck_8b::get_latex_dia(u1 gp, Glyph_Argument_Package &gap)
//{
//}

void Standard_GlyphDeck_8b::get_xml(u1 gp, Glyph_Argument_Package& gap)
{
 Standard_GlyphPack_8b code{gp}; 
 u1 kind, cue;
 std::tie(kind, cue) = code.get_xml_cue();

 switch(kind)
 {
 case 0: gap.chr = get_text_default(cue); break;
// case 1: case 3: case 4: dia = cue; return QChar();
// case 2:
 }
}

//void Standard_GlyphDeck_8b::get_xml_dia(u1 gp, Glyph_Argument_Package& gap)
//{
//}


