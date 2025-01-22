
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include <QString>
#include <QMap>

#include <QStringList>

#include "rz-asg-logical-scope.h"

#include "rz-phaon-user-type.h"


USING_RZNS(GVal)

RZ_ASG_Logical_Scope::RZ_ASG_Logical_Scope(caon_ptr<RZ_ASG_Logical_Scope> p)
 :  RZ_ASG_Lexical_Scope(nullptr), parent_scope_(p), user_type_(nullptr), node_(nullptr)
{
 if(p)
 {
  set_parent(p.raw_pointer());
 }
}

void RZ_ASG_Logical_Scope::increment_field_count()
{
 user_type_->increment_field_count();
}

QString RZ_ASG_Logical_Scope::get_index_key()
{
 if(user_type_)
 {
  int c = user_type_->field_count_for_current_declaration_mode();
  if(c == 0)
    return QString("!%1:%2").arg(user_type_->get_declaration_mode_string())
     .arg(c + 1);
  return QString("%1:%2").arg(user_type_->get_declaration_mode_string())
    .arg(c + 1);
 }
 return QString();
}

