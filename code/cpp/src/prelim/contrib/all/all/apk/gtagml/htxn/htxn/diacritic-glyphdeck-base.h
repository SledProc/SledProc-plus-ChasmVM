
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef DIACRITIC_GLYPHDECK_BASE__H
#define DIACRITIC_GLYPHDECK_BASE__H

#include "accessors.h"

#include "global-types.h"

#include "kans.h"

#include <QString>
#include <QList>
#include <QDate>

#include <functional>


KANS_(HTXN)

class Diacritic_GlyphDeck_Base
{
public:
 virtual void get_full_htxne_representation
  (u2 gp, QChar cue, QChar& rep, QString& full_rep) = 0;

 virtual QString get_latex_out(u2 gp, char cue) = 0;
 virtual QString get_xml_out(u2 gp, char cue) = 0;
};

_KANS(HTXN)

#endif // DIACRITIC_GLYPHDECK_BASE__H
