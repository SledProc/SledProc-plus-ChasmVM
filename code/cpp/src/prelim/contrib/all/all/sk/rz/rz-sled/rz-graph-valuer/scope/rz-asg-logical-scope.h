
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_ASG_LOGICAL_SCOPE__H
#define RZ_ASG_LOGICAL_SCOPE__H

#include "accessors.h"
#include "flags.h"

#include <QString>
#include <QMap>
#include <QList>

#include <QMultiMap>

#include "rz-asg-lexical-scope.h"

#include "rzns.h"

RZNS_(RZ_Core)

class ChasmRZ_Node;

_RZNS(RZ_Core)



RZNS_(GBuild)

class RZ_ASG_Token;

_RZNS(GBuild)



RZNS_(GVal)

class RZ_Phaon_User_Type;

class RZ_ASG_Logical_Scope : public RZ_ASG_Lexical_Scope
{
 QString name_;

 caon_ptr<RZ_ASG_Logical_Scope> parent_scope_;
 caon_ptr<RZ_Phaon_User_Type> user_type_;
 caon_ptr<ChasmRZ_Node> node_;

public:

 RZ_ASG_Logical_Scope(caon_ptr<RZ_ASG_Logical_Scope> p = nullptr);

 ACCESSORS(QString ,name)
 ACCESSORS(caon_ptr<RZ_ASG_Logical_Scope> ,parent_scope)
 ACCESSORS(caon_ptr<RZ_Phaon_User_Type> ,user_type)
 ACCESSORS(caon_ptr<ChasmRZ_Node> ,node)

 QString get_index_key();

 void increment_field_count();

};

_RZNS(GVal)

#endif //RZ_ASG_LOGICAL_SCOPE__H
