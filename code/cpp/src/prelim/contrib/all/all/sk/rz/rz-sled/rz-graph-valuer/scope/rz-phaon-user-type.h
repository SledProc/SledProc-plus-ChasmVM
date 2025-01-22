
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_PHAON_USER_TYPE__H
#define RZ_PHAON_USER_TYPE__H

#include "accessors.h"
#include "flags.h"

//#include "rz-asg-value-holder.h"

#include <QString>
#include <QMap>


#include "rz-asg-scope-token.h"

#include "accessors.h"

#include "relae-graph/relae-caon-ptr.h"

#include "rz-phaon-precycle.h"

KANS_CLASS_DECLARE(Phaon ,PHR_Type_Object)

USING_KANS(Phaon)

RZNS_CLASS_DECLARE(RZ_Core, ChasmRZ_Node)

#include "rzns.h"
RZNS_(GVal)

class RZ_ASG_Logical_Scope;

class RZ_Phaon_User_Type
{
 QString name_;
 caon_ptr<RZ_ASG_Logical_Scope> scope_;
 RZ_Phaon_Precycle precycle_;
 RZ_Phaon_Precycle cocycle_;

 caon_ptr<ChasmRZ_Node> node_;

public:

 enum class Declaration_Modes
 {
  None, Precycle, Cocycle, Class
 };

private:

 Declaration_Modes declaration_mode_;

 QMap<Declaration_Modes, int> field_count_;

public:

 RZ_Phaon_User_Type(QString name);

 ACCESSORS(QString ,name)
 ACCESSORS(caon_ptr<RZ_ASG_Logical_Scope> ,scope)

 ACCESSORS(Declaration_Modes ,declaration_mode)

 ACCESSORS(caon_ptr<ChasmRZ_Node> ,node)

 ACCESSORS__RGET(RZ_Phaon_Precycle ,precycle)
 ACCESSORS__RGET(RZ_Phaon_Precycle ,cocycle)

 int field_count_for_current_declaration_mode();
 QString get_declaration_mode_string();

 void increment_field_count();
};

_RZNS(GVal)

#endif // RZ_PHAON_USER_TYPE__H
