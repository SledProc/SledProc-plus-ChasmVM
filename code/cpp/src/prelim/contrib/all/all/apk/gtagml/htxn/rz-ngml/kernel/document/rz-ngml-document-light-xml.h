
#ifndef RZ_NGML_DOCUMENT_LIGHT_XML__H
#define RZ_NGML_DOCUMENT_LIGHT_XML__H

#include "relae-graph/relae-node-ptr.h"

#include "accessors.h"

#include "rzns.h"

#include <QStringList>

RZNS_(NGML)

class NGML_Parser;
class NGML_Grammar;
class NGML_Graph_Build;
class NGML_Word_Entry_List;


class NGML_Document_Light_Xml
{

 QString text_;

 QString last_tag_name_;
 QString last_attribute_key_;
 QString last_attribute_value_;

 void reset_tag();

public:

 ACCESSORS(QString ,text)

 NGML_Document_Light_Xml();

 void tag_entry(QString name);
 void add_tile(QString tile_str);
 void add_attribute_tile(QString tile_str);
 void tag_body_leave(QString prefix);
 bool check_href(QString& tagname, QString& value);

 void add_attribute_tile(QString key, QString value);

 void tag_leave(QString name);

};

_RZNS(NGML)


#endif
