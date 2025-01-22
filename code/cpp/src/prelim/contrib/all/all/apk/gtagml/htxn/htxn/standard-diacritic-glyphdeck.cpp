
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "standard-diacritic-glyphdeck.h"

USING_KANS(HTXN)

//void Standard_Diacritic_GlyphDeck::get_full_latex_representation
//  (u2 gp, QChar cue, QString& rep)
//{
 
//}

u1 Standard_Diacritic_GlyphDeck::get_representation(u1 gp, QChar& rep)
{
 u1 scope = (gp >> 6) + 1;
 gp &= 63;
 get_htxne_representation(gp, rep);
 return scope;
}

void Standard_Diacritic_GlyphDeck::get_htxne_representation(u1 gp, QChar& rep)
{
 rep = QChar::fromLatin1(get_htxne_representation(gp));
}

void Standard_Diacritic_GlyphDeck::get_full_htxne_representation
  (u2 gp, QChar cue, QChar& rep, QString& full_rep)
{
 if( (gp < 49) || (gp > 51) )
 {
  QChar qc;
  get_htxne_representation(gp, qc);
  u1 scope = (gp >> 6) + 1;
  gp &= 63;
  switch(scope)
  {
   default:
   case 1: 
    full_rep = QString("|%1%2").arg(qc).arg(cue);
    break;
   case 2:
    if(qc == ':')
      full_rep = QString("|+%1").arg(cue);
    else
      full_rep = QString("|>%1%2").arg(qc).arg(cue);
    break;
   case 3: 
    full_rep = QString("|&%1%2").arg(qc).arg(cue);
    break;
   case 4: 
    full_rep = QString("|\\%1%2").arg(qc).arg(cue);
    break;
  }
 }
 else
 {
  rep = cue;
 }
}


char Standard_Diacritic_GlyphDeck::get_htxne_representation(u1 gp)
{
 switch(gp)
 {
  case 0: return '\0'; // null
  case 1: return '_'; // empty
  case 2: return '`'; // // acute
  case 3: return '\''; // grave
  case 4: return '^'; // // circ
  case 5: return '"'; // // uml
  case 6: return '~'; // // tilde
  case 7: return '='; // // macr
  case 8: return '.'; // // dot
  case 9: return 'u'; // // breve
  case 10: return 'v'; // // caron
  case 11: return 'H'; // // dbl acute
  case 12: return 'b'; // // bar-under 
  case 13: return 'c'; // // cedil
  case 14: return 'k'; // // ogon
  case 15: return 'r'; // // ring over ...
  case 16: return 'R'; // // ring over (alt), e.g. aa instead of ra ...
  case 17: return 'K'; // // stroke
  case 18: return 't'; // // tie-after
  case 19: return '/'; // // slash
  case 20: return 'm'; // // midot
  case 21: return 'g'; // // eng, kgreen, ital &  ...
  case 22: return 'G'; // // eng, ital & with more space

  case 23: return 'h'; // // eth, thorn, Thorn 
  case 24: return 'z'; // // eszat i.e. ss ...

  case 25: return 'e'; // // ae, oe
  case 26: return 'E'; // // AE, OE

  case 27: return 'f'; // // fi, fj, fl, ff, FI, FJ, FL, FF 

  case 28: return 'F'; // // fi, ffj, ffl, FFI, FFJ, FFL

  case 29: return 'n'; // // no ligature
  case 30: return 'N'; // // no ligature
  case 31: return 'C'; // // italic correction

  case 32: return '@'; // // special circled
  case 33: return ')'; // // circled
  case 34: return '%'; // // circle left
  case 35: return 'o'; // // circle ring
  case 36: return 'O'; // // circle ring (larger)

  case 37: return '$'; // // strike
  case 38: return '-'; // // strike

  case 39: return 'D'; // // dieresis

  case 40: return '['; // //
  case 41: return ']'; // //
  case 42: return '{'; // //
  case 43: return '}'; // //
  case 44: return ','; // //
  case 45: return ';'; // //

  case 46: return ':'; // // elevated

    // // multi-character codes: 
     //    + double elevated
     //    > double 
     //    & triple
     //    \ external 
     //   e.g.
     //     |:t (TM as one character)   
     //     |+t (TM as two characters) 
     //     |fl (fl ligature as one character)
     //     |Fl (ffl ligature as one character)
     //     |>fl. (fl ligature as two character)
     //     |&Fl.. (ffl ligature as three characters) 

  case 47: return '<'; // // inherited
  case 48: return '+'; // // middle inherited
  case 49: return '>'; // // double inherited

  case 50: return '0'; // //
  case 51: return '1'; // //
  case 52: return '2'; // //
  case 53: return '3'; // //
  case 54: return '4'; // //
  case 55: return '5'; // //
  case 56: return '6'; // //
  case 57: return '7'; // //
  case 58: return '8'; // //
  case 59: return '9'; // //

  case 60: return '#'; // // special 
  case 61: return '*'; // // special 

  case 62: return 'V'; // // (invert)

  case 63: return '?'; // // unrec
 }
}

QString Standard_Diacritic_GlyphDeck::get_xml_stem(u1 gp)
{
 switch(gp)
 {
 case 2: return "acute"; // ` // acute
 case 3: return "grave"; //  ' // grave
 case 4: return "circ"; //  ^ // circ
 case 5: return "uml"; //  " // uml
 case 6: return "tilde"; //  ~ // tilde
 case 7: return "macr"; //  = // macr
 case 8: return "dot"; //  . // dot
 case 9: return "breve"; //  u // breve
 case 10: return "caron"; //  v // caron
 case 11: return "dblac"; //  H // dbl acute
//? case 12: return "acute"; //  b // bar-under 
 case 13: return "cedil"; //  c // cedil
 case 14: return "ogon"; //  k // ogon
 case 15: return "ring"; //  r // ring over ...
 case 16: return "ring"; //  R // ring over (alt)
 case 17: return "strok"; //  K // stroke
 case 19: return "slash"; //  / // slash
 case 20: return "midot"; //  m // midot
 default: return QString();
 }
}


QString Standard_Diacritic_GlyphDeck::get_xml_out(u2 gp, char cue)
{
 //Standard_Diacritic_GlyphDeck::

#ifdef HIDE
\`A&\'A&\^A&\~A&\quot;A&\AA&\AE&\c C&\`E&\'E&\^E&\"E&\`I&\'I&\^I&\"I\\
\DH&\~N&\`O&\'O&\^O&\~O&\"O&\texttimes&\O&\`U&\'U&\^U&\"U&\'Y&\TH&\ss\\
\`a&\'a&\^a&\~a&\"a&\aa&\ae&\c c&\`e&\'e&\^e&\"e&\`i&\'i&\^i&\"i\\
\dh&\~n&\`o&\'o&\^o&\~o&\"o&\textdiv&\o&\`u&\'u&\^u&\"u&\'y&\th&\"y\\

#endif // def HIDE

 static QMap<u1, QString> static_map {

   { 10118, "&inodot;" },  //  _.i
   { 10119, "&inodot;" },  //  _.j

   { 12123, "&eng;" },  //  g.n
   { 12150, "&ENG;" },  //  g.N

   { 12110, "&amp;" },  //  g.a //? &#128629;
   { 12120, "&kgreen;" },  //  g.k
   { 12137, "&amp;" },  //  g.A //? &#128629;
   { 12147, "&kgreen;" },  //  g.K  
  
   { 12223, "&eng;" },  //  G.n  
   { 12250, "&ENG;" },  //  G.N 
 
   { 12210, "&amp;" },  //  G.a ? &#128629;
   { 12220, "&kgreen;" },  //  G.k
   { 12237, "&amp;" },  //  G.A ? &#128629;
   { 12247, "&kgreen;" },  //  G.K  

   { 11610, "&Aring;" },  //  R.a  
   { 11637, "&aring;" },  //  R.A  

   { 14610, "&ordf;" }, // :.a
   { 24610, "&ordf;" }, // :.a
   { 14637, "&ordf;" }, // :.A
   { 24637, "&ordf;" }, // :.A

   { 14624, "&ordm;" }, // :.o
   { 24624, "&ordm;" }, // :.o
   { 14651, "&ordm;" }, // :.O
   { 24651, "&ordm;" }, // :.O

   { 14629, "&trade;" }, // :.t
   { 24629, "&trade;" }, // :.t

   { 14631, "&#8480;" }, // :.v
   { 24631, "&#8480;" }, // :.v

   { 14628, "st" }, // :.s  i.e. 1st ...
   { 24628, "st" }, // :.s
   { 14655, "st" }, // :.S  
   { 24655, "st" }, // :.S

   { 14623, "nd" }, // :.n  i.e. 2nd ...
   { 24623, "n" }, // :.n
   { 14650, "nd" }, // :.N  
   { 24650, "n" }, // :.N

   { 14627, "rd" }, // :.r  i.e. 3rd ...
   { 24627, "rd" }, // :.r
   { 14654, "rd" }, // :.R  
   { 24654, "rd" }, // :.R

   { 14617, "th" }, // :.h  i.e. 4th ...
   { 24617, "th" }, // :.h
   { 14644, "th" }, // :.H  
   { 24644, "th" }, // :.H

   { 12310, "&eth;" },  //  h.d  
   { 12337, "&ETH;" },  //  h.D  
   { 12329, "&thorn;" },  //  h.t  
   { 12356, "&THORN;" },  //  h.T  

   { 12428, "&szlig;" },  //  z.s  
   { 12455, "&szlig;" },  //  z.S  

   { 12510, "&aelig;" },  //  e.a  
   { 12524, "&oelig;" },  //  e.o  
   { 12537, "&AElig;" },  //  e.A
   { 12551, "&OElig;" },  //  e.O
  
   { 12610, "&aelig;" },  //  E.a  
   { 12624, "&oelig;" },  //  E.o  
   { 12637, "&AElig;" },  //  E.A  
   { 12651, "&OElig;" },  //  E.O 

   { 13212, "&copy;" }, // @.c
   { 13227, "&reg;" }, // @.r

   { 13412, "[&copy;]" }, // %.c

   { 13936, "&diaresis;"}, // D._

#ifdef HIDE
   {1, "&Agrave;" }, // { "&#192;" }, Capital a with grave accent
   {1, "&Aacute;" }, // { "&#193;" }, Capital a with acute accent
   {1, "&Acirc;" }, // { "&#194;" }, Capital a with circumflex accent
   {1, "&Atilde;" }, // { "&#195;" }, Capital a with tilde
   {1, "&Auml;" }, // { "&#196;" }, Capital a with umlaut
   {1, "&Aring;" }, // { "&#197;" }, Capital a with ring
   {1, "&AElig;" }, // { "&#198;" }, Capital ae
   {1, "&Ccedil;" }, // { "&#199;" }, Capital c with cedilla
   {1, "&Egrave;" }, // { "&#200;" }, Capital e with grave accent
   {1, "&Eacute;" }, // { "&#201;" }, Capital e with acute accent
   {1, "&Ecirc;" }, // { "&#202;" }, Capital e with circumflex accent
   {1, "&Euml;" }, // { "&#203;" }, Capital e with umlaut
   {1, "&Igrave;" }, // { "&#204;" }, Capital i with grave accent
   {1, "&Iacute;" }, // { "&#205;" }, Capital i with accute accent
   {1, "&Icirc;" }, // { "&#206;" }, Capital i with circumflex accent
   {1, "&Iuml;" }, // { "&#207;" }, Capital i with umlaut
   {1, "&ETH;" },      //  {1, "&#208;" }, Capital eth (Icelandic)
   {1, "&Ntilde;" }, // { "&#209;" }, Capital n with tilde
   {1, "&Ograve;" }, // { "&#210;" }, Capital o with grave accent
   {1, "&Oacute;" }, // { "&#211;" }, Capital o with accute accent
   {1, "&Ocirc;" }, // { "&#212;" }, Capital o with circumflex accent
   {1, "&Otilde;" }, // { "&#213;" }, Capital o with tilde
   {1, "&Ouml;" }, // { "&#214;" }, Capital o with umlaut
   {1, "&Oslash;" }, // { "&#216;" }, Capital o with slash
   {1, "&Ugrave;" }, // { "&#217;" }, Capital u with grave accent
   {1, "&Uacute;" }, // { "&#218;" }, Capital u with acute accent
   {1, "&Ucirc;" }, // { "&#219;" }, Capital u with circumflex accent
   {1, "&Uuml;" }, // { "&#220;" }, Capital u with umlaut
   {1, "&Yacute;" }, // { "&#221;" }, Capital y with acute accent
   {1, "&THORN;" }, // { "&#222;" }, Capital thorn (Icelandic)
   {1, "&szlig;" }, // { "&#223;" }, Lowercase sharp s (German)
   {1, "&agrave;" }, // { "&#224;" }, Lowercase a with grave accent
   {1, "&aacute;" }, // { "&#225;" }, Lowercase a with acute accent
   {1, "&acirc;" }, // { "&#226;" }, Lowercase a with circumflex accent
   {1, "&atilde;" }, // { "&#227;" }, Lowercase a with tilde
   {1, "&auml;" }, // { "&#228;" }, Lowercase a with umlaut
   {1, "&aring;" }, // { "&#229;" }, Lowercase a with ring
   {1, "&aelig;" }, // { "&#230;" }, Lowercase ae
   {1, "&ccedil;" }, // { "&#231;" }, Lowercase c with cedilla
   {1, "&egrave;" }, // { "&#232;" }, Lowercase e with grave accent
   {1, "&eacute;" }, // { "&#233;" }, Lowercase e with acute accent
   {1, "&ecirc;" }, // { "&#234;" }, Lowercase e with circumflex accent
   {1, "&euml;" }, // { "&#235;" }, Lowercase e with umlaut
   {1, "&igrave;" }, // { "&#236;" }, Lowercase i with grave accent
   {1, "&iacute;" }, // { "&#237;" }, Lowercase i with acute accent
   {1, "&icirc;" }, // { "&#238;" }, Lowercase i with circumflex accent
   {1, "&iuml;" }, // { "&#239;" }, Lowercase i with umlaut
   {1, "&eth;" },   //  { "&#240;" }, Lowercase eth (Icelandic)
   {1, "&ntilde;" }, // { "&#241;" }, Lowercase n with tilde
   {1, "&ograve;" }, // { "&#242;" }, Lowercase o with grave accent
   {1, "&oacute;" }, // { "&#243;" }, Lowercase o with acute accent
   {1, "&ocirc;" }, // { "&#244;" }, Lowercase o with circumflex accent
   {1, "&otilde;" }, // { "&#245;" }, Lowercase o with tilde
   {1, "&ouml;" }, // { "&#246;" }, Lowercase o with umlaut
   {1, "&oslash;" }, // { "&#248;" }, Lowercase o with slash
   {1, "&ugrave;" }, // { "&#249;" }, Lowercase u with grave accent
   {1, "&uacute;" }, // { "&#250;" }, Lowercase u with acute accent
   {1, "&ucirc;" }, // { "&#251;" }, Lowercase u with circumflex accent
   {1, "&uuml;" }, // { "&#252;" }, Lowercase u with umlaut
   {1, "&yacute;" }, // { "&#253;" }, Lowercase y with acute accent
   {1, "&thorn;" }, // { "&#254;" }, Lowercase thorn (Icelandic)
   {1, "&yuml;" },     // &#255; Lowercase y with umlaut

#endif //def HIDE
 };

 gp &= 63;
 auto it = static_map.find(gp);
 if(it == static_map.end())
 {
  QString stem = get_xml_stem(gp);
  if(stem.isEmpty())
    return "?";
  if(cue == '_')
    return QString("&%1;").arg(stem);  
  return QString("&%1%2;").arg(QChar::fromLatin1(cue)).arg(stem);
 }
 else
   return it.value();
}


QString Standard_Diacritic_GlyphDeck::get_htxne_out(u1 gp, char cue)
{
 QString result;
 u1 scope = (gp >> 6) + 1;
 gp &= 63;

 QChar rep;
 get_htxne_representation(gp, rep);

 switch(scope)
 {
 default: 
 case 1:
  result = QString("|%1%2"); break;
 case 2:
  result = QString("|>%1%2"); break;
 case 3:
  result = QString("|&%1%2"); break;
 case 4:
  result = QString("|\\%1%2"); break;
 }
 
 return result.arg(rep).arg(QChar::fromLatin1(cue));
}

void Standard_Diacritic_GlyphDeck::get_qstring_out
  (u2 code, u1 gp, char cue, QString& result)
{
 static QMap<u2, QString> static_map {
 };
 result = static_map.value(code, QChar::fromLatin1(cue));
}

void Standard_Diacritic_GlyphDeck::get_latex_out
  (u2 code, u1 gp, u1 scope, char cue, QString& result)
{
 if(gp == 0)
 {
  result = "{\\IndQM}";
  return;
 }
 static QMap<u2, QString> static_map {
   { 10323, "{\\dianapos}" }, // '.n
   { 10350, "{\\diaNapos}" }, // '.N

   { 20323, "{\\ddianapos" }, // '.n
   { 20350, "{\\ddiaNapos" }, // '.N
 
   { 12119, "{\\diaij}" },  //  g.j
   { 12146, "{\\diaIJ}" },  //  g.J

   { 22118, "{\\ddiaij" },  //  g.i
   { 22145, "{\\ddiaIJ" },  //  g.I

   { 12123, "{\\ng}" },  //  g.n
   { 12150, "{\\NG}" },  //  g.N

   { 12110, "{\\&}" },  //  g.a
   { 12120, "\\textsc{k}" },  //  g.k
   { 12137, "{\\upamp}" },  //  g.A
   { 12147, "{\\Kgreenalt}" },  //  g.K  
  
   { 12223, "{\\ngalt}" },  //  G.n  
   { 12250, "{\\NGalt}" },  //  G.N 
 
   { 12210, "{\\itampalt}" },  //  G.a  
   { 12220, "{\\kGreenalt}" },  //  G.k
   { 12237, "{\\upampalt}" },  //  G.A
   { 12247, "{\\Kgreenalt}" },  //  G.K  

   { 11610, "{\\aa}" },  //  R.a  
   { 11637, "{\\AA}" },  //  R.a  

   { 12310, "{\\dh}" },  //  h.d  
   { 12337, "{\\DH}" },  //  h.D  
   { 12329, "{\\th}" },  //  h.t  
   { 12356, "{\\TH}" },  //  h.T  

   { 12428, "{\\ss}" },  //  z.s  
   { 12455, "{\\sSalt}" },  //  z.S  

   { 12510, "{\\ae}" },  //  e.a  
   { 12524, "{\\oe}" },  //  e.o  
   { 12537, "{\\Aealt}" },  //  e.A
   { 12551, "{\\Oealt}" },  //  e.O
  
   { 12610, "{\\aEalt}" },  //  E.a  
   { 12624, "{\\oEalt}" },  //  E.o  
   { 12637, "{\\AE}" },  //  E.A  
   { 12651, "{\\OE}" },  //  E.O 

   { 13136, "{\\/}" }, // C._

   { 13212, "{\\textcopyright}" }, // @.c
   { 13227, "{\\textregistered}" }, // @.r

   { 13412, "{\\textcopyleft}" }, // %.c

   { 13936, "{\\\"}"}, // D._

   { 14610, "{\\diaordfem}" }, // :.a
   { 24610, "{\\ddiaordfem}" }, // :.a
   { 14637, "{\\diaordfemalt}" }, // :.A
   { 24637, "{\\ddiaordfemalt}" }, // :.A

   { 14624, "{\\diaordmasc}" }, // :.o
   { 24624, "{\\ddiaordmasc}" }, // :.o
   { 14651, "{\\diaordmascalt}" }, // :.O
   { 24651, "{\\ddiaordmascalt}" }, // :.O

   { 14629, "{\\texttrademark}" }, // :.t
   { 24629, "{\\ttrademark" }, // :.t

   { 14631, "{\\textservicemark}" }, // :.v
   { 24631, "{\\tservicemark" }, // :.v

   { 14628, "{\\diaST}" }, // :.s  i.e. 1st ...
   { 24628, "{\\ddiaST}" }, // :.s
   { 14655, "{\\altdiaST}" }, // :.S  
   { 24655, "{\\altddiaST" }, // :.S

   { 14623, "{\\diaND}" }, // :.n  i.e. 2nd ...
   { 24623, "{\\ddiaND}" }, // :.n
   { 14650, "{\\altdiaND}" }, // :.N  
   { 24650, "{\\altddiaND" }, // :.N

   { 14627, "{\\diaRD}" }, // :.r  i.e. 3rd ...
   { 24627, "{\\ddiaRD}" }, // :.r
   { 14654, "{\\altdiaRD}" }, // :.R  
   { 24654, "{\\altddiaRD" }, // :.R

   { 14617, "{\\diaTH}" }, // :.h  i.e. 4th ...
   { 24617, "{\\ddiaTH}" }, // :.h
   { 14644, "{\\altdiaTH}" }, // :.H  
   { 24644, "{\\altddiaTH" }, // :.H

 };
 auto it = static_map.find(code);
 if(it == static_map.end())
 {
  QChar rep;
  get_representation(gp, rep);
  if( cue == '_' )
    result = QString("\\%1{}").arg(rep);
  else if( ((cue == 'i') || (cue == 'j')) 
    && (gp != 14) ) // ogon
  {
   if(gp == 1) // _.i or _.j
     result = QString("{\\%2nodot}").arg(rep).arg(cue);
   else    
     result = QString("\\%1{\\%2nodot}").arg(rep).arg(cue);
  }
  else if(gp < 16) //  normal ...
    result = QString("\\%1{%2}").arg(rep).arg(cue);
  else 
  {
   switch(gp)
   {
   case 17: // K
    result = QString("{\\%1strok}").arg(cue); break;      
   case 18: // t
    if(scope == 1)
      result = QString("\\t{%1}").arg(cue); 
    else
      result = QString("\\t{%1").arg(cue);   
    break; 
   case 19: // /
    if( (cue == 'o') || (cue == 'O' ) )
      result = QString("{\\%1").arg(QChar::fromLatin1(cue)); 
    else
      result = QString("\\%1slash").arg(cue);   
    break;          
   case 20: // m
     result = QString("{\\%2midot}").arg(cue); break;
   case 27: // f
    if(scope == 1)
      result = QString("\\fLig{%1}").arg(cue); 
    else if(scope == 2)
      result = QString("\\fLigg{%1").arg(cue); 
    else if(scope == 3)
      result = QString("\\fLiggg{%1").arg(cue); 
    break; 
   case 28: // F
    if(scope == 1)
      result = QString("\\ffLig{%1}").arg(cue); 
    else if(scope == 2)
      result = QString("\\ffLigg{%1").arg(cue); 
    else if(scope == 3)
      result = QString("\\ffLiggg{%1").arg(cue); 
    break;
   case 29: // n
    if(scope == 1)
      result = QString("\\noLig{%1}").arg(cue); 
    else if(scope == 2)
      result = QString("\\noLigg{%1").arg(cue); 
    else if(scope == 3)
      result = QString("\\noLiggg{%1").arg(cue); 
    break;
   case 30: // N
    if(scope == 1)
      result = QString("\\NoLig{%1}").arg(cue); 
    else if(scope == 2)
      result = QString("\\NoLigg{%1").arg(cue); 
    else if(scope == 3)
      result = QString("\\NoLiggg{%1").arg(cue); 
    break;
   case 31: // C
    if(scope == 1)
      result = QString("\\AltNoLig{%1}").arg(cue); 
    else if(scope == 2)
      result = QString("\\AltNoLigg{%1").arg(cue); 
    else if(scope == 3)
      result = QString("\\AltNoLiggg{%1").arg(cue); 
    break;
   case 33: // circled
    if(scope == 1)
      result = QString("\\diacircled{%1}").arg(cue);
    else
      result = QString("\\diacircled{%1").arg(cue);
    break;
   case 34: // circled flip
    if(scope == 1)
      result = QString("\\diacircledflip{%1}").arg(cue);
    else
      result = QString("\\diacircledflip{%1").arg(cue);
    break;
   case 35: // circle ring
    result = QString("\\diacirclering{%1}").arg(cue);
    break;
   case 36: // circle Ring
    result = QString("\\diacircleRing{%1}").arg(cue);
    break;
   case 37: // $
    result = QString("\\diavstrike{%1}").arg(cue);
    break;
   case 38: // -
    result = QString("\\diahstrike{%1}").arg(cue);
    break;
   case 39: // D
    result = QString("\\\"{%1}").arg(cue);
    break;

   case 46: // :
    if(scope == 1)
      result = QString("\\diaele{%1}").arg(cue);
    else
      result = QString("\\diaele{%1").arg(cue);
    break;

   case 47: // inherited
    if(cue == '.')
      result = QString("}");
    else
      result = QString("%1}").arg(cue);
    break;

   case 48: // middle inherited
    if(cue == '.')
      result = QString();
    else
      result = rep;
    break;

   case 49: // double inherited
    if(cue == '.')
      result = QString("}");
    else
      result = QString("%1}").arg(cue);
    break;

   case 62: // V
    result = QString("\\diainv{%1}").arg(cue);
    break;

   case 63: // ?
    result = QString("\\diaNotRecognized{%1}").arg(cue);
    break;

   default:     
    result = QString("\\%1{%2}").arg(rep).arg(cue); break; 
   }
  }
 }
 else
   result = it.value();
}

QString Standard_Diacritic_GlyphDeck::get_latex_out(u2 gp, char cue)
{
 QString result;
 u1 scope = (gp >> 6) + 1;
 u1 code = gp;
 code &= 63;
 code *= 100;
 code += (scope * 10000);
 code += get_cue_code(cue);
 get_latex_out(code, gp, scope, cue, result);
 return result;
}

#ifdef HIDE
QString Standard_Diacritic_GlyphDeck::get_xml_out(u2 gp, char cue)
{
 QString result;
 u1 scope = (gp >> 6) + 1;
 u1 code = gp;
 code &= 63;
 code *= 100;
 code += (scope * 10000);
 code += get_cue_code(cue);
 get_xml_out(code, gp, scope, cue, result);
 return result;
}
#endif //def HIDE




QString Standard_Diacritic_GlyphDeck::get_qstring_out(u1 gp, char cue)
{
 QString result;
 u1 scope = (gp >> 6) + 1;
 u1 code = gp;
 code &= 63;
 code *= 100;
 code += (scope * 10000);
 code += get_cue_code(cue);
 get_qstring_out(code, gp, cue, result);
 return result;
}

u1 Standard_Diacritic_GlyphDeck::get_diacritic_code_inh(u1 pos, u1 length)
{
 // //  47: < // inherited
 // //  48: + // middle inherited
 // //  49: > // double inherited

 switch( (pos * 10) + length )
 {
 case 22: return 47;
 case 23: return 48;
 case 33: return 49;
 default: return 0;
 }
}


u1 Standard_Diacritic_GlyphDeck::get_cue_code(char cue)
{
 switch(cue)
 {
 case '0': return 0;
 case '1': return 1;
 case '2': return 2;
 case '3': return 3;
 case '4': return 4;
 case '5': return 5;
 case '6': return 6;
 case '7': return 7;
 case '8': return 8;
 case '9': return 9;

 case 'a': return 10;
 case 'b': return 11;
 case 'c': return 12;
 case 'd': return 13;
 case 'e': return 14;
 case 'f': return 15;
 case 'g': return 16;
 case 'h': return 17;
 case 'i': return 18;
 case 'j': return 19;
 case 'k': return 20;
 case 'l': return 21;
 case 'm': return 22;
 case 'n': return 23;
 case 'o': return 24;
 case 'p': return 25;
 case 'q': return 26;
 case 'r': return 27;
 case 's': return 28;
 case 't': return 29;
 case 'u': return 30;
 case 'v': return 31;
 case 'w': return 32;
 case 'x': return 33;
 case 'y': return 34;
 case 'z': return 35;

 case '_': return 36;

 case 'A': return 37;
 case 'B': return 38;
 case 'C': return 39;
 case 'D': return 40;
 case 'E': return 41;
 case 'F': return 42;
 case 'G': return 43;
 case 'H': return 44;
 case 'I': return 45;
 case 'J': return 46;
 case 'K': return 47;
 case 'L': return 48;
 case 'M': return 49;
 case 'N': return 50;
 case 'O': return 51;
 case 'P': return 52;
 case 'Q': return 53;
 case 'R': return 54;
 case 'S': return 55;
 case 'T': return 56;
 case 'U': return 57;
 case 'V': return 58;
 case 'W': return 59;
 case 'X': return 60;
 case 'Y': return 61;
 case 'Z': return 62;
 default: break;
 }

}

QPair<u1, u1> Standard_Diacritic_GlyphDeck::get_length(char cue)
{
 switch(cue)
 {
 case '+': return {2, get_code(':', 2)};
 case '>': return {2, 0};
 case '&': return {3, 0};
 default: return {0, 0};
 } 
}

u1 Standard_Diacritic_GlyphDeck::get_code(char cue, u1 scope)
{
 u1 result = 0;
 switch(cue)
 {
 case '_': result = 1; break;
 case '`': result = 2; break;
 case '\'': result = 3; break;
 case '^': result = 4; break;
 case '"': result = 5; break;
 case '~': result = 6; break;
 case '=': result = 7; break;
 case '.': result = 8; break;
 case 'u': result = 9; break;

 case 'v': result = 10; break;
 case 'H': result = 11; break;
 case 'b': result = 12; break;
 case 'c': result = 13; break;
 case 'k': result = 14; break;
 case 'r': result = 15; break;
 case 'R': result = 16; break;
 case 'K': result = 17; break;
 case 't': result = 18; break;

 case '/': result = 19; break;
 case 'm': result = 20; break;
 case 'g': result = 21; break;
 case 'G': result = 22; break;
 case 'h': result = 23; break;
 case 'z': result = 24; break;
 case 'e': result = 25; break;
 case 'E': result = 26; break;
 case 'f': result = 27; break;

 case 'F': result = 28; break;
 case 'n': result = 29; break;
 case 'N': result = 30; break;
 case 'C': result = 31; break;
 case '@': result = 32; break;
 case ')': result = 33; break;
 case '%': result = 34; break;
 case 'o': result = 35; break;
 case 'O': result = 36; break;

 case '$': result = 37; break;
 case '-': result = 38; break;
 case 'D': result = 39; break;
 case '[': result = 40; break;
 case ']': result = 41; break;
 case '{': result = 42; break;
 case '}': result = 43; break;
 case ',': result = 44; break;
 case ';': result = 45; break;

 case ':': result = 46; break;
 case '<': result = 47; break;
 case '+': result = 48; break;
 case '>': result = 49; break;
 case '0': result = 50; break;
 case '1': result = 51; break;
 case '2': result = 52; break;
 case '3': result = 53; break;
 case '4': result = 54; break;

 case '5': result = 55; break;
 case '6': result = 56; break;
 case '7': result = 57; break;
 case '8': result = 58; break;
 case '9': result = 59; break;
 case '#': result = 60; break;
 case '*': result = 61; break;
 case 'V': result = 62; break;
 case '?': result = 63; break;
 default: break;
 }

 switch(scope)
 {
 default:
 case 1: return result;
 case 2: return result + 64;
 case 3: return result + 128;
 case 4: return result + 192;
 }
}

#ifdef HIDE
0: null
1: _ empty
2: ` // acute
3: ' // grave
4: ^ // circ
5: " // uml
6: ~ // tilde
7: = // macr
8: . // dot
9: u // breve
10: v // caron
11: H // dbl acute
12: t // tie-after 
13: c // cedil
14: r // ring over ...
15: R // ring over (alt), e.g. aa instead of ra ...
16: k // ogon
17: b // bar-under
18: K // stroke
19: / // slash
20: m // midot
21: g // eng, kgreen, |&  ...
22: G // eng, |& with more space

23: h // eth, thorn, Thorn 
24: z // eszat i.e. ss ...

25: e // ae, oe
26: E // AE, OE

27: f // fi, fj, fl, ff, FI, FJ, FL, FF 

28: F // fi, ffj, ffl, FFI, FFJ, FFL

29: n // no ligature
30: N // no ligature
31: C // italic correction

32: ) // circled left
33: @ // special circled

34: ( // circled

35: $ // strike
36: - // strike

37: # // special 
38: % // special 
39: * // special

40: [
41: ]
42: {
43: }
44: ,
45: ;

46: : // elevated

// // multi-character codes: 
 //    + double elevated
 //    > double 
 //    & triple
 //    \ external 
 //   e.g.
 //     |:t (TM as one character)   
 //     |+t (TM as two characters) 
 //     |fl (fl ligature as one character)
 //     |Fl (ffl ligature as one character)
 //     |>fl (fl ligature as two character)
 //     |&Fl (ffl ligature as three characters) 


47: < // inherited
48: + // middle inherited
49: > // double inherited



50: 0
51: 1
52: 2
53: 3
54: 4
55: 5
56: 6
57: 7
58: 8
59: 9

60: D // dieresis
61: i // generic italics  

62: V (invert)

63: ? // unrec

#endif //def HIDE


