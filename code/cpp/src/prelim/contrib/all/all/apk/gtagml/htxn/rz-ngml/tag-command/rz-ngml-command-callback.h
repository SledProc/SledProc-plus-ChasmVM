
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef RZ_NGML_COMMAND_CALLBACK__H
#define RZ_NGML_COMMAND_CALLBACK__H

#include "accessors.h"
#include "flags.h"

#include "relae-graph/relae-caon-ptr.h"

#include "global-types.h"

#include <QString>
#include <QMap>

#include <QTextStream>

#include <functional>

#include "rzns.h"
RZNS_(NGML)


class NGML_Node;

class NGML_Command_Callback
{
public:
 flags_(1)
  bool has_pre_callback:1;
  bool has_post_callback:1;
  bool has_around_callback:1;
  bool pre_fallthrough:1;
  bool post_fallthrough:1;
  bool has_rename:1;
  bool has_rename_style_class:1;
 _flags

private:

 QString command_name_trigger_;

 std::function<void(QTextStream&, caon_ptr<NGML_Node> node, u4 index, caon_ptr<NGML_Command_Callback> cb)> pre_callback_;
 std::function<void(QTextStream&, caon_ptr<NGML_Node> node, u4 index, caon_ptr<NGML_Command_Callback> cb)> post_callback_;
 std::function<void(QTextStream&, caon_ptr<NGML_Node> node, u4 index, caon_ptr<NGML_Command_Callback> cb)> around_callback_;

 QString rename_tag_;
 QString rename_style_class_;

public:

 ACCESSORS(QString ,rename_tag)
 ACCESSORS(QString ,rename_style_class)

 void rename_style_class(QString sc)
 {
  rename_style_class_ = sc;
  flags.has_rename_style_class = true;
 }

 typedef QMap<QString, std::function<void(QTextStream&, caon_ptr<NGML_Node>, u4 index, caon_ptr<NGML_Command_Callback> cb)> >
  Callback_Map_type;

 NGML_Command_Callback(QString command_name);

 NGML_Command_Callback(QString command_name,
  Callback_Map_type callbacks);

 void absorb(Callback_Map_type callbacks);

 NGML_Command_Callback(QString command_name, QString tag);
 NGML_Command_Callback(QString command_name, QString tag, QString style_class);




 void pre_callback(QTextStream& qts, caon_ptr<NGML_Node> node, u4 index, caon_ptr<NGML_Command_Callback> cb) const;
 void post_callback(QTextStream& qts, caon_ptr<NGML_Node> node, u4 index, caon_ptr<NGML_Command_Callback> cb) const;
 void around_callback(QTextStream& qts, caon_ptr<NGML_Node> node, u4 index, caon_ptr<NGML_Command_Callback> cb) const;


};

_RZNS(NGML)

#endif
