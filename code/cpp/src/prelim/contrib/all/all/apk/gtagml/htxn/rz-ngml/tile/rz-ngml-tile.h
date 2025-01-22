
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef RZ_NGML_TILE__H
#define RZ_NGML_TILE__H

#include <QString>

#include "accessors.h"
#include "flags.h"

#include "global-types.h"

#include "whitespace/rz-ngml-whitespace-holder.h"

#include "rzns.h"

RZNS_(NGML)

class NGML_HTXN_Node;

class NGML_Tile : public NGML_Whitespace_Holder
{
 QString raw_text_;

 u4 length_adjustment_;

 NGML_HTXN_Node* ngml_htxn_node_;

public:

 ACCESSORS(QString ,raw_text)
 ACCESSORS(NGML_HTXN_Node* ,ngml_htxn_node)

 ACCESSORS(u4 ,length_adjustment)


 NGML_Tile(QString raw_text);

 QString to_string();

 u4 effective_length();

 QString thumbnail(int max_characters = 6);

};

_RZNS(NGML)

#endif
