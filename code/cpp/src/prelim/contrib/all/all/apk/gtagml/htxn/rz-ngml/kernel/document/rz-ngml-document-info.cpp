
#include "rz-ngml-document-info.h"


#include "rz-ngml-document-light-xml.h"

#include <QFile>
#include <QTextStream>

#include "rzns.h"

USING_RZNS(NGML)


NGML_Document_Info::NGML_Document_Info() : light_xml_(nullptr)
{

}


void NGML_Document_Info::init_light_xml()
{
 light_xml_ = new NGML_Document_Light_Xml();
}


void NGML_Document_Info::add_quote(QString quote)
{
 quotes_.push_back(quote);
}


void NGML_Document_Info::add_word(QString word)
{
 word_stream_ += word + ' ';
 word = word.toLower();
 if(word.startsWith("'"))
 {
  word.remove(0, 1);
 }
 if(word.endsWith("'"))
 {
  word.chop(1);
 }
 ++word_count_[word];
}

void NGML_Document_Info::check_light_xml_href()
{
 if(light_xml_)
 {
  QString tagname;
  QString url;
  if(light_xml_->check_href(tagname, url))
  {
   add_href(tagname, url);
  }
 }
}

void NGML_Document_Info::add_href(QString tagname, QString url)
{
 if(!tagname.isEmpty())
 {
  if(url.startsWith("javascript:"))
   return;
  if(url.startsWith("#"))
   return;
  if(url.size() > 1)
  {
   hrefs_[tagname].push_back(url);
  }
 }
}


void NGML_Document_Info::save_word_count(QString file)
{
 QString review = "<html><head><style>b{color:purple}</style><body><table>";
 QMapIterator<QString, int> it(word_count_);
 while(it.hasNext())
 {
  it.next();
  review += QString("<tr><td><b>%1</b></td><td>%2</td></tr>\n").arg(it.key(), -40).arg(it.value());
 }
 review += "</table></body></head></html>";
 save_file(file, review);
}


void NGML_Document_Info::save_hrefs(QString file)
{
 QString review = "<html><head><style>a{font-weight:900,font-size:14pt}</style><body>";
 QMapIterator<QString, QStringList> it(hrefs_);
 while(it.hasNext())
 {
  it.next();
  QString key = it.key();
  QStringList value = it.value();

  review += QString("\n<h1>&ldquo;%1&rdquo; Tags</h1>\n").arg(key.toUpper());
  for(QString str : value)
  review += QString("<br><a href = \"%1\">%1</a><br>").arg(str);
 }
 review += "</body></head></html>";
 save_file(file, review);
}

void NGML_Document_Info::save_word_stream(QString file)
{
 save_file(file, word_stream_);
}


void NGML_Document_Info::save_quotes(QString file)
{
 QString review = "<html><head><style>p{font-weight:900,font-size:14pt}</style><body>";
 for(QString s : quotes_)
 {
  review += "\n<p>" + s + "\n</p>";
 }
 review += "</body></head></html>";
 save_file(file, review);
}


void NGML_Document_Info::save_light_xml(QString file)
{
 if(light_xml_)
 {
  QString text = light_xml_->text();

  text.replace("$OP$", "(");
  text.replace("$CP$", ")");
  text.replace("$EQ$", "=");

  save_file(file, text);
 }
}

void NGML_Document_Info::save_file(QString path, QString contents)
{
 QFile file(path);
 if(file.open(QIODevice::WriteOnly | QIODevice::Text))
 {
  QTextStream out(&file);
  out << contents;
 }
}
