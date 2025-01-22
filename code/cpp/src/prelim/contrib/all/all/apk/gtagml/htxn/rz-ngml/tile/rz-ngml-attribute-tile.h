
#ifndef RZ_NGML_ATTRIBUTE_TILE__H
#define RZ_NGML_ATTRIBUTE_TILE__H

#include <QString>

#include "accessors.h"
#include "flags.h"

#include "whitespace/rz-ngml-whitespace-holder.h"

#include "rzns.h"

RZNS_(NGML)

class NGML_Attribute_Tile : public NGML_Whitespace_Holder
{
 QString key_;
 QString value_;

public:

 ACCESSORS(QString ,key)
 ACCESSORS(QString ,value)

 NGML_Attribute_Tile(QString key, QString value);

 template<typename T>
 void write_html(T& t)
 {
  //?
  value_.replace('"', '\\"');
  t << ' ' << key_ << "=\"" << value_ << '\"';
 }

 template<typename T>
 void write_khif(T& t)
 {
  t << " +[@] " << key_ << ' ' << value_ << ' ';
 }

 QString thumbnail();

};

_RZNS(NGML)

#endif
