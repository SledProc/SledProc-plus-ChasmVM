
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef GLYPH_LAYER_8B__H
#define GLYPH_LAYER_8B__H

#include "glyph-vector-8b.h"

#include "graph/htxn-node-detail.h"

#include "flags.h"
#include "accessors.h"

#include "global-types.h"

#define Glyph_Layer_8b__SET_DESCRIPTION(_this) \
   _this->set_description(#_this);


KANS_(HTXN)

class HTXN_Node_Detail;


class Glyph_Layer_8b : public Glyph_Vector_8b
{
public:
 flags_(1)
  bool main:1;
  bool latex:1;
  bool xml:1;
 _flags

private:
 QString description_;

 QMap<u4, QVector<QPair<u4, u4>>> ranges_; 
 
 QMap<u4, QVector<QPair<QString, const HTXN_Node_Detail*>>> processing_leaves_;

 QMap<QPair<u4, u4>, u4>* assymetric_leaves_;

 u4 id_;

 QMap<u4, u4> insert_loop_guards_;

// QSet<u4> marked_sentence_starts_;
// QSet<u4> marked_non_pivot_sentence_ends_;

public:

 Glyph_Layer_8b(u4 id);

 ACCESSORS(QString ,description)
 ACCESSORS(u4 ,id)

 void activate_assymetric_leaves();
 void add_assymetric_leave(u4 leave, u4 node_code, u4 order);

 u4 add_range(u4 enter, u4 leave, u4 node_code);
 void set_range_leave(u4 enter, u4 order, u4 leave);

 u4 get_range_by_enter(u4 enter, u4& leave, u2 count);

 void add_insert_loop_guard(u4 enter);
 void update_insert_loop_guard(u4 enter, u4 leave);

 u4 check_insert_loop_guard(u4 enter);

 QVector<QPair<QString, const HTXN_Node_Detail*>> check_leave(u4 leave);
 void add_leave(u4 leave, QString cmd, const HTXN_Node_Detail* nd, u4 nc);

 void write_ranges(QTextStream& qts) const;
 void read_ranges(QTextStream& qts);
};


_KANS(HTXN)

#endif // GLYPH_LAYER_8B__H


