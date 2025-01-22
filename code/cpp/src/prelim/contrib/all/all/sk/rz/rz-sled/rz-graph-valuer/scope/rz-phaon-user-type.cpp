
#include "rz-phaon-user-type.h"
//?#include "rz-asg-logical-scope.h"

#include <QString>
#include <QMap>

USING_RZNS(GVal)

RZ_Phaon_User_Type::RZ_Phaon_User_Type(QString name)
 : name_(name), node_(nullptr), declaration_mode_(Declaration_Modes::None)
{}

int RZ_Phaon_User_Type::field_count_for_current_declaration_mode()
{
 return field_count_.value(declaration_mode_);
}

void RZ_Phaon_User_Type::increment_field_count()
{
 ++field_count_[declaration_mode_];
}


QString RZ_Phaon_User_Type::get_declaration_mode_string()
{
 switch(declaration_mode_)
 {
 case Declaration_Modes::None:
   return "??";
 case Declaration_Modes::Class:
   return "Cl";
 case Declaration_Modes::Cocycle:
   return "Co";
 case Declaration_Modes::Precycle:
   return "Pr";
 }
}

