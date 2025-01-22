
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef RZ_NGML_WHITESPACE_HOLDER__H
#define RZ_NGML_WHITESPACE_HOLDER__H

#include <QString>

#include "accessors.h"

#include "rz-ngml-whitespace.h"

#include "rzns.h"

RZNS_(NGML)

class NGML_Whitespace_Holder
{
 NGML_Whitespace ws_;

 NGML_Whitespace_Holder* inherited_;

public:

 NGML_Whitespace_Holder();

 ACCESSORS(NGML_Whitespace ,ws)
 ACCESSORS(NGML_Whitespace_Holder* ,inherited)

 void attach_whitespace(QString whitespace);

 void inherit_whitespace(NGML_Whitespace_Holder* wsh);

 void get_whitespace_counts_as_inherited(u1* result);

 template<typename T>
 void write_whitespace(T& t)
 {
  t << ws_.to_string();
 }

};

_RZNS(NGML)

#endif
