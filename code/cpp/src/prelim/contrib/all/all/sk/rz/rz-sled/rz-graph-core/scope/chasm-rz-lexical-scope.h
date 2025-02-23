
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef CHASM_RZ_LEXICAL_SCOPE__H
#define CHASM_RZ_LEXICAL_SCOPE__H

#include "accessors.h"
#include "flags.h"


#include <QString>
#include <QMap>


#include "relae-graph/relae-caon-ptr.h"

#include "flags.h"
#include "global-types.h"

#include "rzns.h"


RZNS_(GVal)

class RZ_ASG_Lexical_Scope;

_RZNS(GVal)

USING_RZNS(GVal)

RZNS_(RZ_Core)

class ChasmRZ_Node;
class ChasmRZ_Token;

class ChasmRZ_Lexical_Scope
{
 u2 id_;
 ChasmRZ_Lexical_Scope* parent_scope_;
 QMap<QString, caon_ptr<ChasmRZ_Node>> known_variables_;

public:

 ChasmRZ_Lexical_Scope(u2 id, ChasmRZ_Lexical_Scope* parent_scope);

 ChasmRZ_Lexical_Scope* lookup_variable_name(QString name);

 void register_variable_name(QString name, caon_ptr<ChasmRZ_Node> node)
 {
  known_variables_[name] = node;
 }


};

_RZNS(RZ_Core)

#endif //CHASM_RZ_LEXICAL_SCOPE__H
