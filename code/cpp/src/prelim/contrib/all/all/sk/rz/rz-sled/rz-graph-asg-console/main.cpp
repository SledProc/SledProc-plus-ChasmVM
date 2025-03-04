
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifdef HIDE
#include <iostream>

#include <QDebug>

int main()
{
 std::cout << "ok";

 qDebug() << "qok";

 return 0;
}
#endif



#include "rz-graph-visit/rz-graph-visitor-phaon.h"


#include <QtEndian>

#include <QtGlobal>
#include <QtCore>

#include <QDebug>

#include "rzns.h"

#include "rz-graph-core/kernel/document/chasm-rz-document.h"
#include "rz-graph-core/output/chasm-rz-pre-normal-asg.h"

#include "rz-graph-code/prerun/rz-prerun-tokens.h"
#include "rz-graph-code/prerun/rz-prerun-normalize.h"
#include "rz-graph-code/prerun/rz-prerun-anticipate.h"

#include "rz-graph-code/traverser/rz-prerun-setup.h"



#include "rz-graph-visit/rz-asg-visitor.h"
#include "rz-code-generators/rpi/rpi-output.h"

#include "phr-graph-core/kernel/graph/phr-graph.h"
#include "phr-graph-core/kernel/graph/phr-graph-build.h"

#include "phr-graph-core/output/phr-graph-phr-output.h"
#include "phr-graph-core/kernel/document/phr-graph-document.h"

#include "phr-graph-core/kernel/graph/pgb-ir-build.h"

//?#include "phr-graph-core/kernel/graph/pgb-ir-run.h"

#include "kans.h"

#include <functional>
#include <QDebug>

USING_RZNS(RZ_Core)

void compile_rz(QString file_name)
{
 QString result;

 ChasmRZ_Document* doc = new ChasmRZ_Document(file_name);
 doc->parse();

 doc->report_graph(file_name + ".txt");

 RZ_Prerun_Setup setup(doc);
 setup.output("..setup.txt");

 ChasmRZ_Pre_Normal_ASG prenorm(doc);
 prenorm.output("..prenorm.txt");

 RZ_Prerun_Tokens tokens(doc);
 tokens.output("..prenorm2.txt");

 RZ_Prerun_Normalize normalize(*doc->graph());

 caon_ptr<RZ_ASG_Visitor> visitor = normalize.scan();

 visitor->set_document_directory(doc->local_directory());

 RZ_Graph_Visitor_Phaon visitor_phaon(*visitor);
 RPI_Output rpo(visitor_phaon);

 doc->report_graph(file_name + ".re1.txt");

 ChasmRZ_Pre_Normal_ASG prenorm1(doc);
 prenorm1.output("..prenorm1.txt");

 RZ_Prerun_Anticipate anticipate(*visitor, doc->local_path() + ".cprs");

 anticipate.scan(nullptr);//[](RZ_Dynamo_Output& rzdo){rzdo.init_top_level_block();});

 anticipate.run_core_pairs_generations();

 PGB_IR_Build pgb(file_name, file_name + ".gen.pgb");

 rpo.build_phaon_graph(pgb);

 pgb.generate_premacro_file(rpo.step_forms());
 pgb.expand_macros(rpo.step_forms());
 pgb.generate_file(rpo.step_forms());

 PHR_Graph phg;
 PHR_Graph_Build phgb(phg);

 phgb.load_from_pgb_file(pgb.out_file());
 PHR_Graph_PHR_Output pgo(pgb.out_file() + ".phr");
 pgo.document()->set_graph(&phg);
 pgo.generate();
}

#include "textio.h"

struct Page_Ref_Pair;



struct Page_Ref {
 ushort number;
 uint note;
 QString roman;

 QString to_code() const;

 static Page_Ref_Pair from(QString qs);
 static Page_Ref _from(QString qs);


 bool is_valid()
 {
  return number || !roman.isEmpty();
 }
};

struct Page_Ref_Pair : QPair<Page_Ref, Page_Ref> {

 static Page_Ref_Pair from_vector(const QVector<Page_Ref>& vec)
 {
  Page_Ref_Pair result;
  result.first = vec.value(0);
  result.second = vec.value(1);
  return result;
 }

 QVector<Page_Ref> to_vector()
 {
  if(first.is_valid())
  {
   if(second.is_valid())
     return {first, second};
   return {first};
  }
  return {};
 }

 QString to_code()
 {
  if(first.is_valid())
  {
   if(second.is_valid())
     return "^^%1--%2"_qt.arg(first.to_code()).arg(second.to_code());
   return "^%1"_qt.arg(first.to_code());
  }
  return "??";
 }

};


QString Page_Ref::to_code() const
{
 if(roman.isEmpty())
 {
  if(note)
  {
   QString n = QString::number(note);
   int ix = n.lastIndexOf("000");
   if(ix != -1)
     n.replace(ix, 3, ";");
   return "@%1/%2"_qt.arg(number).arg(n);
  }
  return "%%1"_qt.arg(number);
 }
 return "$%1"_qt.arg(roman);
}


Page_Ref Page_Ref::_from(QString qs)
{
 ushort number = 0, note = 0;
 QString roman;

 QRegularExpression qre ("^(\\d+)n(\\d+)");
 QRegularExpressionMatch match = qre.match(qs);
 if(match.hasMatch())
 {
  number = match.captured(1).toInt();
  note = match.captured(2).toInt();
 }
 else
 {
  QRegularExpression qre1 ("^[xvi]+$");
  QRegularExpressionMatch match1 = qre1.match(qs);
  if(match1.hasMatch())
    roman = qs;
  else
    number = qs.toInt();
 }

 return {number, note, roman};
}

Page_Ref_Pair Page_Ref::from(QString qs)
{
 QVector<Page_Ref> result;

 QStringList qsl = qs.split("--");

 for(QString qs : qsl)
 {
  Page_Ref pr = Page_Ref::_from(qs.simplified());
  if(pr.is_valid())
    result.push_back(pr);
 }
 return Page_Ref_Pair::from_vector(result);
}

struct Index_Entry {

 Index_Entry* parent;
 int count_in_parent;
 QString key;
 QVector<Page_Ref_Pair> refs;
 QString carried;
 int sub_count;
 QString sub_carried;

 static void add_redirect(QString text, QString supplement, QVector<Index_Entry>& entries);

};


void Index_Entry::add_redirect(QString text, QString supplement, QVector<Index_Entry>& entries)
{
 if(text.isEmpty())
   text = supplement;
 else
   text += " " + supplement;

 int ix = text.indexOf(".");
 if(ix != -1)
 {
  QString key = text.mid(0, ix);
  QString carried = text.mid(ix + 1).simplified();

  entries.push_back({nullptr, 0, key, {}, carried, 0});
 }
 else
 {
  entries.push_back({nullptr, 0, text, {}, {}, 0});
 }
}

void write_index_entries(const QVector<Index_Entry>& index_entries, QTextStream& qts)
{
 for(const Index_Entry& ie : index_entries)
 {
  qts << "\n ";

  if(ie.parent)
    qts << "[[  \"" << ie.key << "\"";
  else
    qts << "[[ \"" << ie.key << "\"";



  if(ie.parent)
    qts << " *" << ie.count_in_parent;

  if(ie.sub_count)
    qts << " #" << ie.sub_count;

  if(!ie.carried.isEmpty())
    qts << " +{" << ie.carried << "}";

  if(!ie.sub_carried.isEmpty())
    qts << " -{" << ie.sub_carried << "}";

  qts <<  " => ";// << ie.refs.count();

  int count = 0;
  for(Page_Ref_Pair pr: ie.refs)
  {
   if(count > 0)
     qts << " ->";
   ++count;
   qts << " " << pr.to_code();
  }

  qts << "\n ]] \n";

 }

}


int main(int argc, char *argv[])
{
 QString file = "/home/nlevisrael/Downloads/m2m/index.txt";
 QString ofile = "/home/nlevisrael/Downloads/m2m/oindex.txt";
 QString dfile = "/home/nlevisrael/Downloads/m2m/dindex.txt";

 QString text = KA::TextIO::load_file(file);

 QStringList chunks = text.split(',');

 QString otext;
 QString dtext;

 QTextStream oqts (&otext);
 QTextStream dqts (&dtext);


 QVector<Index_Entry> index_entries;

 QString current_key;
 QString carried_see;


 int ccount = 0;
 int lcount = 0;
 for(QString chunk : chunks)
 {
  ++ccount;
  QStringList _lines = chunk.split("\n");

  QStringList lines;

  for(QString _line : _lines)
  {
   QString _ls = _line.simplified();
   if(_ls.isEmpty())
     continue;

   QString ls;

   std::copy_if(_ls.begin(), _ls.end(), std::back_inserter(ls),
     [](QChar qc)
   {
    if(qc.isPrint())
      return true;
    ushort uni = qc.unicode();
    if(uni >= 63280 && uni <= 63289)
      return true;
   });

   std::transform(ls.begin(), ls.end(), ls.begin(),
     [](QChar qc)
   {
    ushort uni = qc.unicode();
    if(uni >= 63280 && uni <= 63289)
    {
     ushort digit = uni - 63280;
     QString str = QString::number(digit);
     return QChar(str[0]);
    }
    return QChar(uni);
   });


   if(ls.isEmpty())
     continue;

   lines.push_back(ls);
  }

  int lc = 0;

  int _lc = 0;

  bool seen_clear = false;
  for(QString line : lines)
  {
   ++_lc;

   if(line == "Anderson% Karen")
     CAON_DEBUG_NOOP;

   if(line.contains("Leon"))
     CAON_DEBUG_NOOP;

   if(line.contains("Bridget"))
     CAON_DEBUG_NOOP;

   if(line.contains("Billings"))
     CAON_DEBUG_NOOP;

   if(line.endsWith("//"))
   {
    seen_clear = true;
    Index_Entry::add_redirect(current_key, line.chopped(2), index_entries);
    current_key.clear();
    continue;
   }

   if(!carried_see.isEmpty())
   {
    if(carried_see.startsWith("!!"))
    {
     index_entries.last().carried += carried_see.mid(2);
     carried_see.clear();
    }
    else if(_lc == lines.size())
      carried_see.clear();
    else
    {
     index_entries.last().carried += " " + line;
     continue;
    }
   }

   bool first_match = false;
   {
    QRegularExpression qre("^((?:\\d+--)?\\d+(?:n\\d+)?|[xvi-]+)[.](.*)");
    QRegularExpressionMatch match = qre.match(line);
    if(match.hasMatch())
    {
     first_match = true;
     line = match.captured(1);
     carried_see = match.captured(2).simplified();
    }
   }


   if(!first_match)
   {
    QRegularExpression qre("^(\\d+|[xvi-]+)\\s+([(].*)");
    QRegularExpressionMatch match = qre.match(line);
    if(match.hasMatch())
    {
     line = match.captured(1);
     carried_see = match.captured(2).simplified();
    }
   }

   ++lcount;

   Page_Ref_Pair pr = Page_Ref::from(line);
   QVector<Page_Ref> refs = pr.to_vector();
   if(refs.isEmpty())
   {
    if(!carried_see.isEmpty())
    {
     carried_see += " " + line;
     continue;
    }
    ++lc;
    if(lc == 1)
      current_key = line;
    else if(seen_clear)
    {
     seen_clear = false;
     if(current_key.isEmpty())
       current_key = line;
     else
       current_key += " " + line;
    }
    else
      current_key += " " + line;

    oqts << "\n" << ccount << " -> " << lcount << " :" << lc << " => " << line;
    continue;
   }
   oqts << "\n" << ccount << " -> " << lcount << " :" << lc; // << " => " << line;

   oqts << " ->> ";

   if(!current_key.isEmpty())
   {
    index_entries.push_back({nullptr, 0, current_key, {}, carried_see, 0});
    current_key.clear();
    //?carried_see.clear();
   }

//   else if(!carried_see.isEmpty())
//   {
//    append_carried = carried_see;
//    carried_see.clear();
//   }

   Index_Entry& ie = index_entries.last();

//   if(!append_carried.isEmpty())
//     ie.carried += " " + carried_see;


   ie.refs.push_back(pr);

   int count = 0;
   for(const Page_Ref& ref : refs)
   {
    if(count)
      oqts << " # ";

    ++count;

    oqts << ref.to_code();

   }

  }
 }

 Index_Entry* last_non_sub = nullptr;

 for(Index_Entry& ie : index_entries)
 {
  QString key = ie.key;
  if(key.startsWith(";_"))
  {
   ie.parent = last_non_sub;
   ie.key = ie.key.mid(2);
   ++last_non_sub->sub_count;
   ie.count_in_parent = last_non_sub->sub_count;
   if(!ie.carried.isEmpty())
   {
    last_non_sub->sub_carried = ie.carried;
    ie.carried.clear();
   }
  }
  else
    last_non_sub = &ie;
 }


 QVector<Index_Entry> index_entries_adj;

 Index_Entry* ie_for_sub_increment;

 for(Index_Entry& ie : index_entries)
 {
  if(ie.parent && ie.parent == ie_for_sub_increment)
    ++ie.count_in_parent;

  QString key = ie.key;
  key.replace("%", ",");

  if(ie.sub_count && ie.key.contains(":"))
  {
   ie_for_sub_increment = &ie;
   QStringList split = ie.key.split(":");

   Index_Entry ie1, ie2;
   ie1.carried = ie.carried;
   ie1.count_in_parent = ie.count_in_parent; // should be 0
   ie1.key = split.value(0).simplified();
   ie1.parent = ie.parent; // should be nullptr
   //ie1.refs =
   ie1.sub_carried = ie.sub_carried;
   ie1.sub_count = ie.sub_count;

   ie2.count_in_parent = 1; // should be 0
   ie2.key = split.value(1).simplified();
   ie2.parent = &ie; // should be nullptr
   ie2.refs = ie.refs;
   ie2.sub_count = 0;

   index_entries_adj.push_back(ie1);
   index_entries_adj.push_back(ie2);
  }
  else
  {
   ie.key = key;
   index_entries_adj.push_back(ie);
  }
 }

 write_index_entries(index_entries_adj, dqts);

 KA::TextIO::save_file(ofile, otext);
 KA::TextIO::save_file(dfile, dtext);

 return 0;
}


int main1(int argc, char *argv[])
{
 QString file = "/home/nlevisrael/Downloads/pic/Publications.html";
 QString ofile = "/home/nlevisrael/Downloads/pic/Publications.temp.html";

 QString text = KA::TextIO::load_file(file);

 int pos = 0;
 int pos2 = 0;
 int pos3 = 0;
 int pos4 = 0;

 int count = 0;
 int count2 = 0;
 int count3 = 0;
 int count4 = 0;

 int count0 = 0;

 QMap<QString, int> venues;

 QMap<QString, int> titles;

 while ((pos = text.indexOf("!####!", pos)) != -1)
 {
  ++count;

  text.replace(pos, 6, "(#%1)"_qt.arg(count, 3, 10, QLatin1Char('0')));
 }

 while ((pos2 = text.indexOf("!%%%!", pos2)) != -1)
 {
  ++count2;

  text.replace(pos2, 5, "(+%1)"_qt.arg(count2, 2, 10, QLatin1Char('0')));
 }

 while ((pos3 = text.indexOf("=== /##/", pos3)) != -1)
 {
  int pos4 = text.indexOf("<", pos3);

  if(pos4 != -1)
  {
   QString venue = text.mid(pos3 + 9, pos4 - (pos3 + 9));

   if(++venues[venue] == 1)
   {
    ++count3;
    text.replace(pos3, 8, "==> /%1/"_qt.arg(count3, 2, 10, QLatin1Char('0')));

    qDebug() << "%1 (%2)"_qt.arg(venue).arg(count3);
   }

  }

  pos3 += 8;
 }

 while ((pos4 = text.indexOf("<-###>", pos4)) != -1)
 {
  int pos5 = text.indexOf("</", pos4);

  QString title = text.mid(pos4 + 6, pos5 - pos4 - 6);

  if(++titles[title] == 1)
  {
   ++count0;
   text.replace(pos4 + 2, 3, "000");
//?   qDebug() << "%1  %2"_qt.arg(count0).arg(title);
  }
  else
  {
   ++count4;
   text.replace(pos4 + 2, 3, "%1"_qt.arg(count4, 3, 10, QLatin1Char('0')));
  }

  //text.replace(pos2, 5, "(+%1)"_qt.arg(count2, 2, 10, QLatin1Char('0')));
 }




 KA::TextIO::save_file(ofile, text);
}


int main2(int argc, char *argv[])
{
 compile_rz(RZ_DIR "/demo/asg/t1.rz");

 return 0;
}

//#endif // HIDE
