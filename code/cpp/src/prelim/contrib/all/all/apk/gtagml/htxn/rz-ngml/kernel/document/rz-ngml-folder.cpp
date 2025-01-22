
#include "rz-ngml-folder.h"

#include "rz-ngml-document.h"

#include "output/rz-ngml-output-htxn.h"
#include "output/rz-ngml-output-latex.h"


#include "output/rz-ngml-output-html.h"


#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>
#include <QDir>
#include <QDebug>

#include "rzns.h"

USING_RZNS(NGML)


NGML_Folder::NGML_Folder(QString local_path)
 : local_path_(local_path)
{

}


void NGML_Folder::read_output_path(QString path)
{
 QFileInfo qfi(path);
 QFile file(path);
 if(file.open(QFile::ReadOnly | QIODevice::Text))
 {
  output_path_ = file.readAll().trimmed();
  local_path_ = qfi.absolutePath();
  input_path_ = path;
 }
}


void NGML_Folder::convert_to_latex()
{
 QDir dir(local_path_);
 dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
 dir.setSorting(QDir::Size | QDir::Reversed);

 QFileInfoList qfil = dir.entryInfoList();

 QListIterator<QFileInfo> it(qfil);

 QStringList qsl;
 QString cabinet;

 while(it.hasNext())
 {
  QFileInfo qfi = it.next();
  if(qfi.suffix() == "ngml")
  {
   qsl << qfi.absoluteFilePath();
  }
 }

 QStringListIterator slit(qsl);
 while(slit.hasNext())
 {
  QString path = slit.next();

  NGML_Document doc;
  doc.load_and_parse(path);

  doc.set_annotations(&annotations_);

  NGML_Output_Latex nol(doc);
  nol.export_latex();

  doc.write_local_annotations(path + ".an");

 }
 NGML_Document::write_annotations(local_path_ + "/annotation-summary.html", annotations_);
}

#ifdef HIDE
void NGML_Folder::convert_all_khif_files(QString khif_dir, QString html_dir)
{
 QDir dir(local_path_);
 dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
 dir.setSorting(QDir::Size | QDir::Reversed);

 QFileInfoList qfil = dir.entryInfoList();

 QListIterator<QFileInfo> it(qfil);

 QStringList qsl;
 QString cabinet;

 while(it.hasNext())
 {
  QFileInfo qfi = it.next();
  if(qfi.suffix() == "ngml")
  {
   qsl << qfi.absoluteFilePath();
  }
 }
 QStringListIterator slit(qsl);
 while(slit.hasNext())
 {
  QString path = slit.next();

  QFileInfo qfi(path);

  NGML_Document ngd;
  ngd.load_and_parse(path);

  NGML_Output_Khif ngo(ngd);

  QString flo = khif_dir + '/' +
    qfi.completeBaseName() + ".khif";

  QString hlo = html_dir + '/' +
    qfi.completeBaseName() + ".htm";

  file_list_output_[flo] = hlo;

  ngo.export_khif(flo);

  QString s = ngd.khi_info();

  if(s.isEmpty())
  {
   //?
   //QString route = QString("/%1").arg(qfi.completeBaseName());
   //non_partials_codes_[route] = flo;
  }
  else
  {
   QStringList qsl = s.split(QRegularExpression("\\s+"), QString::SkipEmptyParts);
   if(qsl.count() > 1)
   {
    if(qsl[1] == "p")
    {
     partials_codes_[qsl[0]] = flo;
    }
    else if(qsl[1] == "s")
     non_partials_codes_[qsl[0]] = flo;
   }
   else if(qsl.count() == 1)
   {
    non_partials_codes_[qsl[0]] = flo;
   }
   CAON_DEBUG_NOOP
  }
 }
}
#endif //def HIDE

void NGML_Folder::output_file_list(QString& result, const QMap<QString, QString>& files)
{
 QMapIterator<QString, QString> it(files);
 while(it.hasNext())
 {
  it.next();
  QString k = it.key();
  if(k.startsWith('/'))
   k.remove(0, 1);
  QString v = it.value();
  result += QString(" \"%1\" \n  \"%2\"\n")
    .arg(v).arg(file_list_output_[v]);
 }
}

QString NGML_Folder::output_file_list()
{
 QString result;

 output_file_list(result, partials_codes_);
 output_file_list(result, non_partials_codes_);
 return result;
}


QString NGML_Folder::partials_codes_to_lisp()
{
 QString result;
 QMapIterator<QString, QString> it(partials_codes_);
 while(it.hasNext())
 {
  it.next();
  QString k = it.key();
  if(k.startsWith('/'))
   k.remove(0, 1);
  QString v = it.value();
  result += QString("\n (setf (gethash :@@%1 khi-forms::*partials*) \"%2\")").arg(k).arg(v);
 }
 return result;
}

void NGML_Folder::get_ngml_files(QStringList& result)
{
 QDir dir(local_path_);
 dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
 dir.setSorting(QDir::Size | QDir::Reversed);

 QFileInfoList qfil = dir.entryInfoList();

 QListIterator<QFileInfo> it(qfil);

 while(it.hasNext())
 {
  QFileInfo qfi = it.next();
  if(qfi.suffix() == "ngml")
    result << qfi.absoluteFilePath();
 }
}


void NGML_Folder::convert_all_files(QString output_path, QString khif_path)
{
 QDir dir(local_path_);
 dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
 dir.setSorting(QDir::Size | QDir::Reversed);

 QFileInfoList qfil = dir.entryInfoList();

 QListIterator<QFileInfo> it(qfil);

 QStringList qsl;
 QString cabinet;

 while(it.hasNext())
 {
  QFileInfo qfi = it.next();
  if(qfi.suffix() == "ngml")
  {
   qDebug() << qfi.absoluteFilePath();
   if(qfi.baseName() == "cabinet")
   {
    cabinet = qfi.absoluteFilePath();
   }
   else
   {
    qsl << qfi.absoluteFilePath();
   }
  }
 }

 if(!cabinet.isEmpty())
 {
  qsl << cabinet;
 }

 QStringListIterator slit(qsl);
 while(slit.hasNext())
 {
  QString path = slit.next();

  QFileInfo qfi(path);

  NGML_Document ngd;
  ngd.load_and_parse(path);

  NGML_Output_Html ngo(ngd);
  ngo.export_html(output_path + '/' + qfi.completeBaseName() + ".html");

  // //?
  NGML_Output_HTXN noh(ngd, nullptr);
  noh.export_htxne(output_path + '/' + qfi.completeBaseName() + ".htxne");

  NGML_Output_Latex nol(ngd);
  nol.export_latex(output_path + '/' + qfi.completeBaseName() + ".tex");


//?  if(!khif_path.isEmpty())
//?  {
//?   NGML_Output_Khif khif(ngd);
//?   khif.export_khif(khif_path + '/' + qfi.completeBaseName() + ".khif");
//?  }

 }
}


void NGML_Folder::convert_all_files()
{
 convert_all_files(output_path_);
}


void NGML_Folder::display_user_highlights()
{
 QFile file(input_path_ + ".usrh.txt");
 if(file.open(QFile::WriteOnly | QIODevice::Text))
 {
  QTextStream qts(&file);
   //  QMap<QString, int>& uh = user_highlights_;
  QMapIterator<QString, int> it(user_highlights_);
  while(it.hasNext())
  {
   it.next();
   QString text = it.key();
   int num = it.value();
   qts << text << ' ' << num <<"\n";
  }
  file.close();
 }

}
