
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_ASG_DOCUMENT__H
#define RZ_ASG_DOCUMENT__H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMap>

#include "rz-graph-core/kernel/document/chasm-rz-document.h"


#include "accessors.h"

#include "rzns.h"

USING_RZNS(RZ_Core)

RZNS_(GBuild)

class RZ_ASG_Document
{
 QString raw_text_;

 ChasmRZ_Document& rz_document_;

 QString data_layer_;

 QMap<int, QString> comments_by_line_;
 typedef QMap<int, QString> comments_by_line_type;

 typedef ChasmRZ_Node tNode;

public:

 ACCESSORS(QString ,raw_text)
 ACCESSORS(QString ,data_layer)
 ACCESSORS__RGET(comments_by_line_type ,comments_by_line)


 RZ_ASG_Document(ChasmRZ_Document& rz_document);


 void add_rz_comment(QString comment, int line);


};

_RZNS(GBuild)

#endif //RZ_ASG_DOCUMENT__H
