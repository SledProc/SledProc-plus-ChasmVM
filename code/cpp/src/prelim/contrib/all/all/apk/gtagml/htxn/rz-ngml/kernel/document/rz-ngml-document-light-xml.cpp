
#include "rz-ngml-document-light-xml.h"

#include <QFile>
#include <QTextStream>

#include <QRegularExpression>
#include <QRegularExpressionMatch>

#include "rzns.h"

USING_RZNS(NGML)

NGML_Document_Light_Xml::NGML_Document_Light_Xml()
{

}

void NGML_Document_Light_Xml::add_tile(QString tile_str)
{
 text_ += tile_str;
}

void NGML_Document_Light_Xml::add_attribute_tile(QString tile_str)
{
 QRegularExpression rx("(\\S+)\\s+(.+)", QRegularExpression::DotMatchesEverythingOption);
 QRegularExpressionMatch rxm = rx.match(tile_str);
 if(rxm.hasMatch())
 {
  QString key = rxm.captured(1);
  QString value = rxm.captured(2);

  add_attribute_tile(key, value);
 }
}

void NGML_Document_Light_Xml::add_attribute_tile(QString key, QString value)
{
 last_attribute_key_ = key;
 last_attribute_value_ = value;
 value.replace("\\\"", "\"");
 value.replace('"', "\\\"");

 text_ += ' ' + key + "=\"" + value + '"';
}

void NGML_Document_Light_Xml::reset_tag()
{
 last_tag_name_.clear();
 last_attribute_key_.clear();
 last_attribute_value_.clear();
}

void NGML_Document_Light_Xml::tag_body_leave(QString prefix)
{
 text_ += prefix + '>';
 reset_tag();
}

void NGML_Document_Light_Xml::tag_entry(QString name)
{
 text_ += "\n<" + name;
 last_tag_name_ = name;
}

void NGML_Document_Light_Xml::tag_leave(QString name)
{
 text_ += "\n</" + name + ">\n";
}

bool NGML_Document_Light_Xml::check_href(QString& tagname, QString& value)
{
 if(last_attribute_key_ == "href")
 {
  tagname = last_tag_name_;
  value = last_attribute_value_;
  return true;
 }
 return false;
}
