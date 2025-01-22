
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef CHASM_RZ_FUNCTION_DEF_ENTRY__H
#define CHASM_RZ_FUNCTION_DEF_ENTRY__H

#include "accessors.h"
#include "flags.h"


#include <QString>
#include <QMap>

#include "relae-graph/relae-caon-ptr.h"

#include "chasm-rz-function-def-kinds.h"

#include "rzns.h"
RZNS_(RZ_Core)


class ChasmRZ_Node;

class ChasmRZ_Function_Def_Entry
{
 caon_ptr<ChasmRZ_Node> prior_node_;
 caon_ptr<ChasmRZ_Node> node_;
 caon_ptr<ChasmRZ_Node> label_node_;
 caon_ptr<ChasmRZ_Node> note_node_;

 ChasmRZ_Function_Def_Kinds kind_;

public:

 ACCESSORS(caon_ptr<ChasmRZ_Node> ,prior_node)
 ACCESSORS(caon_ptr<ChasmRZ_Node> ,node)
 ACCESSORS(caon_ptr<ChasmRZ_Node> ,label_node)
 ACCESSORS(ChasmRZ_Function_Def_Kinds ,kind)
 ACCESSORS(caon_ptr<ChasmRZ_Node> ,note_node)

 ChasmRZ_Function_Def_Entry(caon_ptr<ChasmRZ_Node> prior_node, ChasmRZ_Function_Def_Kinds kind, caon_ptr<ChasmRZ_Node> label_node = nullptr);


};

_RZNS(RZ_Core)

#endif //CHASM_RZ_FUNCTION_DEF_ENTRY__H
