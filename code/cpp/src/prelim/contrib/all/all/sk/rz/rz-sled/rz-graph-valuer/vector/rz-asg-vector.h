
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_ASG_VECTOR__H
#define RZ_ASG_VECTOR__H

#include "rz-typedefs.h"
#include "flags.h"
#include "accessors.h"


#include "relae-graph/relae-caon-ptr.h"


#include <QDebug>
#include <QVector>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QMultiMap>


#include "rzns.h"


RZNS_(RZ_Core)

class ChasmRZ_Node;
class ChasmRZ_Tuple_Info;

_RZNS(RZ_Core)

USING_RZNS(RZ_Core)


RZNS_(GBuild)

class RZ_Type_Object;
class RZ_ASG_Token;
class RZ_String_Phrase;

_RZNS(GBuild)

USING_RZNS(GBuild)


RZNS_(GVal)

class RZ_String;

class RZ_ASG_Valuer;

class RZ_ASG_Vector
{
 typedef ChasmRZ_Node tNode;

public:

 flags_(1)
  bool is_declaration_array:1;
  bool is_final:1;
  bool is_pure_token:1;
 _flags

private:

 caon_ptr<ChasmRZ_Tuple_Info> tuple_info_;
 caon_ptr<RZ_ASG_Token> token_representation_;

 QVector<caon_ptr<tNode>> nodes_;

 caon_ptr<tNode> entry_node_;

public:

 RZ_ASG_Vector(caon_ptr<ChasmRZ_Tuple_Info> tuple_info, RZ_Type_Object& rto);

 bool elements_are_evaluable()
 {
  return !(flags.is_declaration_array || flags.is_pure_token);
 }

 ACCESSORS(caon_ptr<ChasmRZ_Tuple_Info> ,tuple_info)
 ACCESSORS(caon_ptr<RZ_ASG_Token> ,token_representation)

 ACCESSORS__RGET(QVector<caon_ptr<tNode>> ,nodes)

 caon_ptr<tNode> get_call_entry_node();

 void add_node(caon_ptr<tNode> n);

 void to_string_list(QStringList& qsl, RZ_ASG_Valuer& valuer);


 template<typename T>
 friend T& operator<<(T& t, const RZ_ASG_Vector& rhs)
 {
//?  return t << rhs.to_string();
 }


 friend void operator<<(QDebug lhs, const RZ_ASG_Vector& rhs)
 {
//?  tString s = rhs.to_string();
//?  lhs << s;
 }

};

_RZNS(GVal)

#endif //RZ_ASG_VECTOR__H
