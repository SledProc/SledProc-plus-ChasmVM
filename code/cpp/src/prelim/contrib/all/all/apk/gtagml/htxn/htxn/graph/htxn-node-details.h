
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef HTXN_NODE_DETAILS__H
#define HTXN_NODE_DETAILS__H

#include "accessors.h"

#include "global-types.h"

#include "kans.h"

#include "htxn-node-detail.h"

#include <QString>
#include <QList>
#include <QDate>

#include <functional>


KANS_(HTXN)

class Glyph_Layer_8b;


class HTXN_Node_Details
{
protected:
 QVector<HTXN_Node_Detail> node_details_;

public:

 HTXN_Node_Details();

 ACCESSORS__RGET(QVector<HTXN_Node_Detail> ,node_details)

 HTXN_Node_Detail* add_detail_range( // Glyph_Layer_8b* layer, 
   u4 enter, u4 leave, u4& nc);

 HTXN_Node_Detail* tie_detail_range(u4 nc1, u4 nc2);

 HTXN_Node_Detail* mark_ghosted(u4 nc);
 HTXN_Node_Detail* mark_fiat(u4 nc);

 HTXN_Node_Detail* get_node_detail(u4 nc);

 void write_ties(QTextStream& qts);
 void read_ties(QTextStream& qts, 
   const QVector<Glyph_Layer_8b*>& layers);

};


QTextStream& operator<<(QTextStream& qts, 
  const HTXN_Node_Detail& rhs);

QTextStream& operator<<(QTextStream& qts, 
  const QVector<HTXN_Node_Detail>& rhs);

QTextStream& operator>>(QTextStream& qts, 
  HTXN_Node_Detail& rhs);

QTextStream& operator>>(QTextStream& qts, 
  QVector<HTXN_Node_Detail>& rhs);

_KANS(HTXN)

#endif // HTXN_NODE_DETAILS__H
