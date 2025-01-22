
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "htxn-node-details.h"

#include "glyph-layer-8b.h"

#include <QTextStream>

USING_KANS(HTXN)

HTXN_Node_Details::HTXN_Node_Details()
{

}

void HTXN_Node_Details::write_ties(QTextStream& qts)
{
 for(const HTXN_Node_Detail& nd : node_details_)
 {
  if(QVector<u4>* refs = nd.get_refs())
  {
   if( Glyph_Layer_8b* gl = nd.get_layer() )
     qts << "&&" << gl->id() << '\n';
   else
     qts << "&/\n";
   QVectorIterator<u4> it(*refs);
   while (it.hasNext())
   {
    qts << it.next();
    qts << (it.hasNext()? ' ':'~');
   }
   qts << '\n';
  }
  else
  {
   if( Glyph_Layer_8b* gl = nd.get_layer() )
     qts << "&!" << gl->id() << '\n';
   else
     qts << "&?\n";
  }
  qts << "!\n";
 }
}

void HTXN_Node_Details::read_ties(QTextStream& qts, 
  const QVector<Glyph_Layer_8b*>& layers)
{
 QString qs;
 u4 nums = 0;
 for(HTXN_Node_Detail& nd : node_details_)
 {
  nd.prepare_read();
  qs = qts.read(2);
  if(qs == "&?")
  {
    // //  read newline
   qts.read(1); 
   continue;
  }
  if(qs != "&/")
  {
   // //  get layer id
   qts >> nums;
   nd.set_layer(layers.value(nums));
  }
   // //  read newline
  qts.read(1);
  if(qs == "&!")
  {
   // //  read '!', newline ...
   qts.read(2);
   continue;
  }
  while(qs != "~")
  {
   qts >> nums;
   nd.add_node_ref(nums);
   qs = qts.read(1);
  }
  // //  read newline, '!', newline ...
  qts.read(3);
 }
}

HTXN_Node_Detail* HTXN_Node_Details::mark_fiat(u4 nc)
{
 HTXN_Node_Detail& nd = node_details_[nc - 1];
 nd.flags.is_fiat = true;
 return &nd;
}

HTXN_Node_Detail* HTXN_Node_Details::mark_ghosted(u4 nc)
{
 HTXN_Node_Detail& nd = node_details_[nc - 1];
 nd.flags.is_ghosted = true;
 return &nd;
}

HTXN_Node_Detail* HTXN_Node_Details::tie_detail_range(u4 nc1, u4 nc2)
{
 HTXN_Node_Detail& nd1 = node_details_[nc1 - 1];
 nd1.add_node_ref(nc2);
 return &nd1; 
}


HTXN_Node_Detail* HTXN_Node_Details::add_detail_range( // Glyph_Layer_8b* layer, 
  u4 enter, u4 leave, u4& nc)
{
 node_details_.push_back({enter, leave});
 nc = node_details_.size();
 return &(node_details_[nc - 1]);
}

HTXN_Node_Detail* HTXN_Node_Details::get_node_detail(u4 nc)
{
 return &(node_details_[nc - 1]);
}


KANS_(HTXN)

QTextStream& operator<<(QTextStream& qts, 
  const HTXN_Node_Detail& rhs)
{
 qts << rhs.Flags << ';' << rhs.enter << ':' << rhs.leave;
 return qts;
}

QTextStream& operator<<(QTextStream& qts, 
  const QVector<HTXN_Node_Detail>& rhs)
{
 qts << rhs.length() << "\n";
 for(const HTXN_Node_Detail& nd : rhs)
   qts << nd << "\n"; 
 return qts;
}

QTextStream& operator>>(QTextStream& qts, 
  HTXN_Node_Detail& rhs)
{
 char c;
 qts >> rhs.Flags >> c >> rhs.enter >> c >> rhs.leave;
 return qts;
}

QTextStream& operator>>(QTextStream& qts, 
  QVector<HTXN_Node_Detail>& rhs)
{
 int sz; 
 qts >> sz;
 rhs.resize(sz);
  // // read the newline
 qts.read(1);
 for(HTXN_Node_Detail& nd : rhs)
 {
  qts >> nd;
   // // read the newline
  qts.read(1);
 } 
 return qts;
}

_KANS(HTXN)


