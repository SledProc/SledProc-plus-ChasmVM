
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "htxn-infoset-8b.h"

#include "htxn-infoset-range.h"
#include "htxn-infoset-cursor-8b.h"

#include "htxn-document-8b.h"

#include "glyph-layer-8b.h"

#include <QTextStream>

USING_KANS(HTXN)

HTXN_Infoset_8b::HTXN_Infoset_8b(HTXN_Document_8b* htxn_document)
  :  htxn_document_(htxn_document), anchor_range_(nullptr)
{

}

void HTXN_Infoset_8b::set_anchor_range(u4 enter, u4 leave, u4 layer_id)
{
 set_anchor_range(new HTXN_Infoset_Range(enter, leave, layer_id));
}


void HTXN_Infoset_8b::set_anchor_range(u4 enter, u4 layer_id)
{
 set_anchor_range(new HTXN_Infoset_Range(1, enter, layer_id));
}


HTXN_Layer_Position HTXN_Infoset_8b::range_entry_to_position(HTXN_Infoset_Range* range)
{
 return {htxn_document_->get_layer(range->layer_id()), range->enter()};
}

HTXN_Layer_Position HTXN_Infoset_8b::range_leave_to_position(HTXN_Infoset_Range* range)
{
 return {htxn_document_->get_layer(range->layer_id()), range->leave()};
}


QString* HTXN_Infoset_8b::check_connector(QString label)
{
 auto it = std::find_if(connector_defers_.begin(),
   connector_defers_.end(), [&label](
   std::pair<QString*, QVector<QString*>> const& pr) -> bool
 {
  return *(pr.first) == label;
 });
 if(it == connector_defers_.end())
 {
  QString* result = new QString(label);
  connector_defers_[result] = {};
  return result;
 }
 return it->first;
}

void HTXN_Infoset_8b::add_connector_defer(QString label, QString defer)
{
 QString* l = check_connector(label);
 QString* d = check_connector(defer);
 add_connector_defer(l, d);
}

void HTXN_Infoset_8b::add_connector_defer(QString* label, QString* defer)
{
 connector_defers_[label].push_back(defer);
}

void HTXN_Infoset_8b::add_connection(HTXN_Infoset_Range* range, QString label, 
  HTXN_Infoset_Range* target)
{
 QString* l = check_connector(label);
 range->add_connection(l, target);
}

HTXN_Infoset_Cursor_8b* HTXN_Infoset_8b::new_cursor()
{
 return new HTXN_Infoset_Cursor_8b(this);
}


