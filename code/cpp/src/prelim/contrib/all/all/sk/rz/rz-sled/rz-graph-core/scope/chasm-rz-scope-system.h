
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef CHASM_RZ_SCOPE_SYSTEM__H
#define CHASM_RZ_SCOPE_SYSTEM__H

#include "accessors.h"
#include "flags.h"


#include <QString>
#include <QMap>


#include "relae-graph/relae-caon-ptr.h"

#include "flags.h"
#include "global-types.h"

#include "rzns.h"

#include "chasm-rz-lexical-scope.h"


RZNS_(GVal)

class RZ_ASG_Lexical_Scope;

_RZNS(GVal)

USING_RZNS(GVal)

RZNS_(RZ_Core)

class ChasmRZ_Node;
class ChasmRZ_Lexical_Scope;

class ChasmRZ_Scope_System
{
 u2 last_id_;
 ChasmRZ_Lexical_Scope* current_lexical_scope_;

public:

 ChasmRZ_Scope_System();

 void enter_new_lexical_scope();

 void register_variable_name(QString name, caon_ptr<ChasmRZ_Node> node)
 {
  current_lexical_scope_->register_variable_name(name, node);
 }

 ChasmRZ_Lexical_Scope* lookup_variable_name(QString name);
};

_RZNS(RZ_Core)

#endif //CHASM_RZ_SCOPE_SYSTEM__H
