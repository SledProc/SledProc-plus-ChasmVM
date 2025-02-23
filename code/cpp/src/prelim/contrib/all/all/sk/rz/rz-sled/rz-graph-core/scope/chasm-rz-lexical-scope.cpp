
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "chasm-rz-lexical-scope.h"

#include "rzns.h"


USING_RZNS(RZ_Core)


ChasmRZ_Lexical_Scope::ChasmRZ_Lexical_Scope(u2 id,
  ChasmRZ_Lexical_Scope* parent_scope)
 : //Flags(0),
   id_(id), //, nested_depth_(0),
   parent_scope_(parent_scope)
{

}

ChasmRZ_Lexical_Scope* ChasmRZ_Lexical_Scope::lookup_variable_name(QString name)
{
 if(known_variables_.contains(name))
   return this;

 if(parent_scope_)
   return parent_scope_->lookup_variable_name(name);

 return nullptr;
}

