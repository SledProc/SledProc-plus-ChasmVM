
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef STANDARD_DIACRITIC_GLYPHDECK__H
#define STANDARD_DIACRITIC_GLYPHDECK__H

#include "accessors.h"

#include "global-types.h"

#include "kans.h"

#include <QString>
#include <QList>
#include <QDate>

#include <functional>

#include "diacritic-glyphdeck-base.h"

KANS_(HTXN)

class Standard_Diacritic_GlyphDeck : public Diacritic_GlyphDeck_Base
{
public:

//get_htxne_out

 char get_htxne_representation(u1 gp); 
 void get_htxne_representation(u1 gp, QChar& rep);

 void get_full_htxne_representation(u2 gp, 
   QChar cue, QChar& rep, QString& full_rep) Q_DECL_OVERRIDE;
 
 QString get_latex_out(u2 gp, char cue) Q_DECL_OVERRIDE;
 QString get_xml_out(u2 gp, char cue) Q_DECL_OVERRIDE;

 u1 get_representation(u1 gp, QChar& rep); 
 u1 get_code(char cue, u1 scope); 

 u1 get_cue_code(char cue); 
 u1 get_diacritic_code_inh(u1 pos, u1 length);


 QPair<u1, u1> get_length(char cue); 

 QString get_qstring_out(u1 gp, char cue);
 void get_qstring_out(u2 code, u1 gp, char cue, QString& result); 

 void get_latex_out(u2 code, u1 gp, u1 scope, char cue, QString& result); 

//? void get_xml_out(u2 code, u1 gp, u1 scope, char cue, QString& result); 

 QString get_htxne_out(u1 gp, char cue);

//? QString get_xml_out(u1 gp, char cue);

 QString get_xml_stem(u1 gp);

};

_KANS(HTXN)

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
12: b // bar-under 
13: c // cedil
14: k // ogon
15: r // ring over ...
16: R // ring over (alt), e.g. aa instead of ra ...
17: K // stroke
18: t // tie-after
19: / // slash
20: m // midot
21: g // eng, kgreen, ital &  ...
22: G // eng, ital & with more space

23: h // eth, thorn, Thorn 
24: z // eszat i.e. ss ...

25: e // ae, oe
26: E // AE, OE

27: f // fi, fj, fl, ff, FI, FJ, FL, FF 

28: F // fi, ffj, ffl, FFI, FFJ, FFL

29: n // no ligature
30: N // no ligature
31: C // italic correction

32: @ // special circled
33: ) // circled
34: % // circle left
35: o // circle ring
36: O // circle ring (larger)

37: $ // strike
38: - // strike

39: D // dieresis

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

60: # // special 
61: * // special 

62: V // (invert)

63: ? // unrec
#endif

#endif // STANDARD_DIACRITIC_GLYPHDECK__H
