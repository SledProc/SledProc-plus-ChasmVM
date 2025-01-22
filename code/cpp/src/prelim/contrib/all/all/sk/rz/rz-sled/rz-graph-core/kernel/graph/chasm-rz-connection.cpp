
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "chasm-rz-connection.h"


#include "rzns.h"
USING_RZNS(RZ_Core)

ChasmRZ_Connection::ChasmRZ_Connection(caon_ptr<ChasmRZ_Node> chasm_rz_node)
 : chasm_rz_node_(chasm_rz_node)
{

}
