
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

#include "rz-graph-code/traverser/rz-prerun-check.h"



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

 RZ_Prerun_Check check(doc);
 check.output("..check.txt");

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

// PGB_IR_Build pgb(file_name + ".gen.pgb");

// rpo.build_phaon_graph(pgb);

// pgb.generate_premacro_file(rpo.step_forms());
// pgb.expand_macros(rpo.step_forms());
// pgb.generate_file(rpo.step_forms());

// PHR_Graph phg;
// PHR_Graph_Build phgb(phg);

// phgb.load_from_pgb_file(pgb.out_file());
// PHR_Graph_PHR_Output pgo(pgb.out_file() + ".phr");
// pgo.document()->set_graph(&phg);
// pgo.generate();
}

#include "textio.h"

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


int main(int argc, char *argv[])
{
 compile_rz(RZ_DIR "/demo/asg/t1.rz");

 return 0;
}

//#endif // HIDE
