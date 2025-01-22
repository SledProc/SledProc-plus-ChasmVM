
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_ASG_CORE_CASEMENT_FUNCTION__H
#define RZ_ASG_CORE_CASEMENT_FUNCTION__H


#include <QMap>
#include <QString>

#include "rzns.h"

#include "flags.h"
#include "accessors.h"

#include "rz-graph-token/valuer/rz-asg-function-info.h"

RZNS_(RZ_Core)

class ChasmRZ_Node;

_RZNS(RZ_Core)

USING_RZNS(RZ_Core)

RZNS_(GBuild)

class RZ_ASG_Token;

typedef ChasmRZ_Node tNode;

class RZ_ASG_Core_Casement_Function
{
public:
 enum Status_Codes {
  Preempt, Defer, Valuer_CB, Valuer_DH, Valuer_RN, Valuer_N,
  Valuer_N_S, Valuer_N_S_S, Valuer_N_T, Paste, Both,
  CO_Preempt, CO_Defer, CO_Both
 };

 flags_(1)
  bool preempt:1;
  bool defer:1;
  bool use_out_name:1;
  bool use_rz_out_name:1;
  bool paste:1;
  bool valuer:1;
 _flags

private:

 QString rz_name_;
 QString name_;
 int arity_;
 RZ_ASG_Function_Info info_;
 QString out_name_;

 static QMap<QString, QString> out_pastes_;

public:

 ACCESSORS(QString ,rz_name)
 ACCESSORS(QString ,name)
 ACCESSORS(int ,arity)
 ACCESSORS__RGET(RZ_ASG_Function_Info ,info)
 ACCESSORS(QString ,out_name)

 RZ_ASG_Core_Casement_Function(QString rz_name, QString name, int arity, Status_Codes sc);
 QString get_out_name();
 QString get_out_name_or_string();

};

_RZNS(GBuild)


#endif //RZ_ASG_CORE_CASEMENT_FUNCTION__H
