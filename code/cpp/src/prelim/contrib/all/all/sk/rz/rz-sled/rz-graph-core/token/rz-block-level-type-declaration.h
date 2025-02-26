
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_BLOCK_LEVEL_TYPE_DECLARATION__H
#define RZ_BLOCK_LEVEL_TYPE_DECLARATION__H

#include "accessors.h"
#include "flags.h"


#include "code/chasm-rz-code-representation.h"

#include <QString>

#include <functional>

#include "chasm-rz-token.h"

#include "relae-graph/relae-caon-ptr.h"

#include "rzns.h"


RZNS_(RZ_Core)


class RZ_Block_Level_Type_Declaration
{
 caon_ptr<ChasmRZ_Token> token_;

 caon_ptr<ChasmRZ_Node> continuation_node_;

public:

 ACCESSORS(caon_ptr<ChasmRZ_Token> ,token)
 ACCESSORS(caon_ptr<ChasmRZ_Node> ,continuation_node)

 RZ_Block_Level_Type_Declaration(caon_ptr<ChasmRZ_Token> token);

 QString token_string_summary()
 {
  return token_->string_summary();
 }

};

_RZNS(RZ_Core)

#endif //RZ_BLOCK_LEVEL_TYPE_DECLARATION__H
