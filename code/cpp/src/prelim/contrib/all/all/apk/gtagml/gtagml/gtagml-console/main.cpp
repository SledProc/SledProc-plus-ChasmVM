
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include <QDebug>

#include <QFileInfo>

//#include <QFileDialog>


#include "gtagml/kernel/document/gtagml-document.h"
#include "get-cmdl.h"

#include "gtagml/output/gtagml-output-latex.h"
#include "gtagml/output/gtagml-output-blocks.h"
#include "gtagml/output/gtagml-output-sdi-infoset.h"


#include "gtagml/kernel/document/gtagml-folder.h"

#include "gtagml/kernel/document/gtagml-project-info.h"



#include "gh/gh-block-writer.h"

#include "sdi/gh-sdi-document.h"

#include "textio.h"

USING_KANS(GTagML)
USING_KANS(Util)
USING_KANS(SDI)
USING_KANS(TextIO)


void process_gtagml_file(QString path, GTagML_Project_Info* gpi, GTagML_Folder* fld)
{
 //QString& carried_setup,

 qDebug() << "Processing file: " << path;

// GTagML_Document gdoc;
// gdoc.load_and_parse(path);

// GTagML_Output_Blocks gob(gdoc);
// gob.init_standard_8bit();
// gob.export_blocks(); //(path + ".");


// GTagML_Output_Latex gol(gdoc);
// gol.export_latex(path + ".tex");

 GTagML_Document* gdoc = new GTagML_Document;

 if(fld)
   gdoc->set_man_folder_path(fld->man_path());

 gdoc->set_project_info(gpi);

 gdoc->load_and_parse(path);

 gdoc->insert_latex_template(path + "-template.tex");
 gdoc->insert_xml_template(path + "-template.xml");

 gdoc->save_jats(path + ".jats");
 gdoc->save_latex(path + ".tex");
 gdoc->save_sentences(path + ".sentences.sdi");

 //? gdoc->sdi_check(path + ".sentences.sdi", path + ".test");

 qDebug() << "path = " << path + ".jats";

 return; // 0;

 GH_Block_Writer* blw = new GH_Block_Writer;

// QString divert;
// QTextStream qts(&divert);
// gdoc->set_divert(&qts);

 GTagML_Output_Blocks* gob = new GTagML_Output_Blocks(*gdoc, blw);
 gob->init_standard_8bit();
 gob->export_blocks();

 GTagML_Output_SDI_Infoset* gsi = new GTagML_Output_SDI_Infoset(*gdoc, blw);

 QString fnc = gdoc->file_job_name();
 gsi->set_file_job_name(fnc);

 //goi->init_standard_8bit();
 gsi->export_infoset(path + ".info.txt"); // export_blocks(); //(path + ".");

 QString cpy = gsi->copy_path();
 QString setup = gsi->setup_path();

 if(!setup.isEmpty())
 {
  gpi->set_gtagml_setup(setup);
  if(cpy.isEmpty())
    cpy = setup;
 }

 QString ffolder;

 if(gpi->gtagml_setup().isEmpty())
   ffolder = make_folder_from_file_name(path, DEFAULT_SDI_FOLDER);
 else
   ffolder = make_folder_from_file_name(path, gpi->gtagml_setup());


 GH_SDI_Document* gsd = gsi->sdi_document();


 QString sdi_path = get_path_with_different_folder(path, ffolder);

 gsd->finalize_sentence_boundaries(*blw->current_main_text_block(), sdi_path + ".sdi-prelatex.ntxh");

 QString mark_path = gob->export_marks(); //(path + ".");
 gob->load_marks(mark_path);


 GTagML_Output_Latex* gol = new GTagML_Output_Latex(*gdoc, gsd);
 gol->export_latex(path + ".tex");

 QString cp = copy_file_to_folder(path + ".tex", ffolder);

 qDebug() << "Copied " << path + ".tex" << " to " << cp;

 if(!cpy.isEmpty())
 {
  QString cp;

  if(setup.isEmpty())
  {
   cp = copy_file_to_folder(path + ".tex", cpy);
   qDebug() << "Copied " << path + ".tex" << " to " << cp;
  }

  cpy += "/sdi";

  QDir sdi_dir(cpy);
  if(!sdi_dir.exists())
    if(sdi_dir.mkpath("."))
      qDebug() << "Made path: " << sdi_dir.absolutePath();

  cp = copy_file_to_folder(mark_path, cpy);
  qDebug() << "Copied " << mark_path << " to " << cp;

  cp = copy_file_to_folder(sdi_path + ".sdi-prelatex.ntxh", cpy);
  qDebug() << "Copied " << sdi_path + ".sdi-prelatex.ntxh" << " to " << cp;
 }


 if(!setup.isEmpty())
 {
  prepend_template_to_file(cp, DEFAULT_SDI_FOLDER "/prepend",
  {
   { "%CONSOLE", ROOT_FOLDER "/code/cpp/qmake-console/projects/gtagml/ngml-sdi-console"},
   { "%FILE", cp },
   { "%SDI-FILE", sdi_path + ".sdi.ntxh" },
   { "%SDI-PFILE", sdi_path + ".sdi-prelatex.ntxh" }
  });

  QFileInfo qfi(gdoc->local_file_name());

  GH_SDI_Document::setup_folder_from_template(gdoc->local_file_name() + ".tex",
    {}, DEFAULT_SDI_FOLDER "/template", qfi.absolutePath(), ffolder);

 }

}



int main(int argc, char *argv[])
{
 QString folder;// = cmdl.size() > 2? cmdl[2]: DEFAULT_SDI_FOLDER;
 QString file;// = cmdl.size() > 3? cmdl[3]: DEFAULT_SDI_FOLDER
 QString manfolder;

// file = "/home/nlevisrael/gits/PacTk-web/PacTk-jats/docs/gt/paper.gt";
// folder = "/home/nlevisrael/gits/PacTk-web/PacTk-jats/docs/gt/src";

// file = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/foreword/foreword.gt";
// folder = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/foreword";

// file = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/intro/intro.gt";
// folder = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/intro";

// file = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch1/ch1.gt";
// folder = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch1";

// file = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch2/ch2.gt";
// folder = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch2";

// file = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch3/ch3.gt";
// folder = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch3";

// file = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch4/ch4.gt";
// folder = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch4";

// file = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch5/ch5.gt";
// folder = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch5";

// file = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch6/ch6.gt";
// folder = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch6";

// file = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch7/ch7.gt";
// folder = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch7";

// file = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch8/ch8.gt";
// folder = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch8";

// file = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch9/ch9.gt";
// folder = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch9";

 file = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch10/ch10.gt";
 folder = "/home/nlevisrael/gits/sled/s+p-prelim/ar/m2m/ch10";


// QStringList cmdl = get_cmdl(argc, argv, 2, {
//   {&folder, DEFAULT_GTAGML_FOLDER "/dg/ctg/src"},
//   {&file, DEFAULT_GTAGML_FOLDER "/dg/t1/t1.gt"},
//   {&manfolder, {}}
//   });

//? folder = "/home/.../ntxh-ngml/ar/htxn/sdi-test/t9";
//? file = "/home/.../ntxh-ngml/ar/htxn/sdi-test/t9/t9.ngml";

 qDebug() << "Folder: " << folder << ", File: " << file;
 qDebug() << "ManFolder: " << manfolder;

 // file = "/home/nlevisrael/gits/ntxh/wip-sebi/ar/dev/consoles/gtagml/gt/dg/ctg/src/t1.gt";

 // file.clear();

// manfolder.clear();
// file = "/home/nlevisrael/lehmann/gtagml/lehmann/ideas.gt";

 if(manfolder.isEmpty())
 {
  if(file.isEmpty())
  {
   GTagML_Folder fld(folder);
   fld.convert_all_files( &process_gtagml_file );
  }
  else
  {
   GTagML_Project_Info gpi(folder);

   process_gtagml_file(file, &gpi, nullptr);
  }
 }
 else
 {
  GTagML_Folder fld(folder, file, manfolder);
  fld.convert_all_files( &process_gtagml_file );
 }
 return 0;
}


template<u1 length>
QString bin_to_qs(u2 num)
{
 u2 m = 1;
 QString result;
 for(u1 i = 0; i < length; ++i, m *= 2)
 {
  if(num & m)
    result += "a";
  else
    result += "b";
 }
 return result;
}

void invert(QString& qs)
{
 std::reverse(qs.begin(), qs.end());
}

u2 score(QString qs)
{
 invert(qs);
 qs.replace('a', '1');
 qs.replace('b', '0');
 return qs.toUShort(nullptr, 2);
}

void rotate(QString& qs)
{
 QString qc = qs.right(1);
 qs.chop(1);
 qs.prepend(qc);
}


void check_rotation(QMap<QString, QPair<u2, QStringList>>& diagrams, QString d)
{
 if(diagrams.contains(d))
   return;

 QString r = d;

 bool r_found = false;

 while(true)
 {
  rotate(r);
  if(r == d)
    break;

  if(diagrams.contains(r))
  {
   r_found = true;
   u2 ds = score(d);
   diagrams[r].second.push_back(d);
   u2 rs = diagrams[r].first;
   if(ds < rs)
   {
    auto pr = diagrams[r];
    diagrams.remove(r);
    pr.first = ds;
    diagrams[d] = pr;
   }
   break;
  }
 }

 if(!r_found)
 {
  diagrams[d] = {score(d), {d}};
 }
}

QString to_digits(QString d)
{
 QString result;

 for(u1 i = 0; i < 8; ++i)
 {
  if(d[i] == 'a')
    result += QString::number(i + 1);
 }

 return result;
}


QString to_digits12(QString d)
{
 QString result;

 for(u1 i = 0; i < 11; ++i)
 {
  if(d[i] == 'a')
  {
   if(i == 0)
     result += "c";

   else if(i == 11)
    result += "b";

   else if(i == 10)
    result += "a";

   else
     result += QString::number(i);

  }
 }

 return result;
}


u1 period_8(QString qs)
{
 QString try_1 = qs.left(1).repeated(8);
 if(try_1 == qs)
   return 1;

 QString try_2 = qs.left(2).repeated(4);
 if(try_2 == qs)
   return 2;

 QString try_4 = qs.left(4).repeated(2);
 if(try_4 == qs)
   return 4;

 return 8;
}


u1 period_12(QString qs)
{
 QString try_1 = qs.left(1).repeated(12);
 if(try_1 == qs)
   return 1;

 QString try_2 = qs.left(2).repeated(6);
 if(try_2 == qs)
   return 2;

 QString try_3 = qs.left(3).repeated(4);
 if(try_3 == qs)
   return 3;

 QString try_4 = qs.left(4).repeated(3);
 if(try_4 == qs)
   return 4;

 QString try_6 = qs.left(6).repeated(2);
 if(try_6 == qs)
   return 6;

 return 12;
}

int main2(int argc, char *argv[])
{
 QMap<QString, QPair<u2, QStringList>> diagrams;

 for(u2 i = 0; i < 256; ++i)
 {
  QString b = bin_to_qs<8>(i);
  check_rotation(diagrams, b);
//  u2 bscore = score(b);
//  qDebug() << i << " = " << b << " : " << bscore;
 }

 QStringList ks = diagrams.keys();

 qDebug() << "total = " << ks.size() << "\n";

 std::sort(ks.begin(), ks.end(), [](const QString& lhs, const QString& rhs) -> bool
 {
  return score(lhs) < score(rhs);
 });

 QMap<QString, QString> to_lowest;

 for(QString key : ks)
 {
  for(QString alt : diagrams[key].second)
  {
   to_lowest[alt] = key;
  }
 }

 auto flip = [&to_lowest](QString qs)
 {
  QString flipped = "xxxxxxxx";
  flipped[0] = qs[0];
  flipped[7] = qs[1];
  flipped[6] = qs[2];
  flipped[5] = qs[3];
  flipped[4] = qs[4];
  flipped[3] = qs[5];
  flipped[2] = qs[6];
  flipped[1] = qs[7];
  return to_lowest[flipped];
 };

 QMap<u1, QStringList> diagrams_by_count;


 for(QString key : ks)
 {
  u1 c = key.count('a');
  diagrams_by_count[c].push_back(key);
 }

 for(u1 arrows = 0; arrows <= 8; ++arrows)
 {
  QStringList dc = diagrams_by_count[arrows];
  qDebug() << arrows << " -> " << dc.size(); // << " = " << dc;

  u1 asym_count = 0;

  for(QString dca : dc)
  {
   QString f = flip(dca);

   u2 per = period_8(dca);

   if(f == dca)
     qDebug() << " " << to_digits(dca) << " <" << per << "> " <<  " (symmetric)";
   else
   {
    qDebug() << " " << to_digits(dca) << " <" << per << "> " << " [" << to_digits(flip(dca)) << "]";
    ++asym_count;
   }
  }
  qDebug() << "asym: " << asym_count << "\n";
 }

 return 0;
}





int main1(int argc, char *argv[])
{
 QMap<QString, QPair<u2, QStringList>> diagrams;

 for(u2 i = 0; i < 4096; ++i)
 {
  QString b = bin_to_qs<12>(i);
  check_rotation(diagrams, b);
 }


 QStringList ks = diagrams.keys();

 qDebug() << "total = " << ks.size() << "\n";

 std::sort(ks.begin(), ks.end(), [](const QString& lhs, const QString& rhs) -> bool
 {
  return score(lhs) < score(rhs);
 });

 QMap<QString, QString> to_lowest;

 for(QString key : ks)
 {
  for(QString alt : diagrams[key].second)
  {
   to_lowest[alt] = key;
  }
 }

 auto flip = [&to_lowest](QString qs)
 {
  QString flipped = "xxxxxxxxxxxx";
  flipped[0] = qs[0];
  flipped[11] = qs[1];
  flipped[10] = qs[2];
  flipped[9] = qs[3];
  flipped[8] = qs[4];
  flipped[7] = qs[5];
  flipped[6] = qs[6];
  flipped[5] = qs[7];
  flipped[4] = qs[8];
  flipped[3] = qs[9];
  flipped[2] = qs[10];
  flipped[1] = qs[11];
  return to_lowest[flipped];
 };

 QMap<u1, QStringList> diagrams_by_count;


 for(QString key : ks)
 {
  u1 c = key.count('a');
  diagrams_by_count[c].push_back(key);
 }

 for(u1 arrows = 0; arrows <= 12; ++arrows)
 {
  QStringList dc = diagrams_by_count[arrows];
  qDebug() << arrows << " -> " << dc.size(); // << " = " << dc;

  u1 asym_count = 0;

  for(QString dca : dc)
  {
   QString f = flip(dca);

   u2 per = period_12(dca);

   if(f == dca)
     qDebug() << " " << to_digits12(dca) << " <" << per << "> " <<  " (symmetric)";
   else
   {
    qDebug() << " " << to_digits12(dca) << " <" << per << "> " << " [" << to_digits12(flip(dca)) << "]";
    ++asym_count;
   }
  }
  qDebug() << "asym: " << asym_count << "\n";
 }

 return 0;
}
