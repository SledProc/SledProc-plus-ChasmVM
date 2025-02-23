
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "chasm-rz-scope-system.h"
#include "chasm-rz-lexical-scope.h"

#include "rzns.h"


USING_RZNS(RZ_Core)


ChasmRZ_Scope_System::ChasmRZ_Scope_System()
 :  current_lexical_scope_(nullptr),
    last_id_(0)
{

}


void ChasmRZ_Scope_System::enter_new_lexical_scope()
{
 current_lexical_scope_ = new ChasmRZ_Lexical_Scope(++last_id_, current_lexical_scope_);
}

ChasmRZ_Lexical_Scope* ChasmRZ_Scope_System::lookup_variable_name(QString name)
{
 if(current_lexical_scope_)
   return current_lexical_scope_->lookup_variable_name(name);

 return nullptr;
}



