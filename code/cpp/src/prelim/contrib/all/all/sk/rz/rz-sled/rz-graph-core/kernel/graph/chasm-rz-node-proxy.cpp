
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "chasm-rz-node-proxy.h"

#include "token/chasm-rz-token.h"

#include "rzns.h"

USING_RZNS(RZ_Core)

ChasmRZ_Node_Proxy::ChasmRZ_Node_Proxy(caon_ptr<ChasmRZ_Node> node)
  : node_(node)
{

}
