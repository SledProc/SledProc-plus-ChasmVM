
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef HTXN_INFOSET_8B__H
#define HTXN_INFOSET_8B__H

#include "accessors.h"

#include "global-types.h"

#include "flags.h"

#include "kans.h"

#include <QString>
#include <QVector>

#include <map>

#include <functional>

#include "htxn-infoset-range.h"

class QTextStream;

KANS_(HTXN)

class Glyph_Layer_8b;
class HTXN_Document_8b;
class HTXN_Infoset_Cursor_8b;

class HTXN_Infoset_8b 
{ 
 HTXN_Document_8b* htxn_document_;

 std::map<QString*, QVector<QString*>> connector_defers_;

 HTXN_Infoset_Range* anchor_range_;

 QVector<HTXN_Infoset_Range*> ranges_; 

public:

 HTXN_Infoset_8b(HTXN_Document_8b* htxn_document);

 ACCESSORS(HTXN_Infoset_Range* ,anchor_range)

 void set_anchor_range(u4 enter, u4 leave, u4 layer_id = 0);
 void set_anchor_range(u4 enter, u4 layer_id = 0);

 HTXN_Layer_Position range_entry_to_position(HTXN_Infoset_Range* range);
 HTXN_Layer_Position range_leave_to_position(HTXN_Infoset_Range* range);

 QString* check_connector(QString label);
 void add_connector_defer(QString label, QString defer);
 void add_connector_defer(QString* label, QString* defer);

 void add_connection(HTXN_Infoset_Range* range, QString label, 
   HTXN_Infoset_Range* target); 

 HTXN_Infoset_Cursor_8b* new_cursor();

};

_KANS(HTXN)




#endif // HTXN_INFOSET_8B__H
