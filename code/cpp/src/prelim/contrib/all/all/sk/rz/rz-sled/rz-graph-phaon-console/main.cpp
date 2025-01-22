
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rz-graph-visit/rz-graph-visitor-phaon.h"


#include <QtEndian>

#include <QtGlobal>
#include <QtCore>

#include <QDebug>

#include "rzns.h"

#ifdef HIDE
#include "rz-graph-core/kernel/document/rz-re-document.h"
#include "rz-graph-core/output/rz-re-pre-normal-lisp.h"

#include "rz-graph-code/prerun/rz-re-prerun-tokens.h"
#include "rz-graph-code/prerun/rz-re-prerun-normalize.h"
#include "rz-graph-code/prerun/rz-re-prerun-anticipate.h"

#include "rz-graph-visit/rz-lisp-graph-visitor.h"
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

 RE_Document* doc = new RE_Document(file_name);
 doc->parse();

 doc->report_graph(file_name + ".txt");

 RE_Pre_Normal_Lisp prenorm(doc);
 prenorm.output("..prenorm.txt");

 RE_Prerun_Tokens tokens(doc);
 tokens.output("..prenorm2.txt");

 RE_Prerun_Normalize normalize(*doc->graph());

 caon_ptr<RZ_Lisp_Graph_Visitor> visitor = normalize.scan();

 visitor->set_document_directory(doc->local_directory());

 RZ_Graph_Visitor_Phaon visitor_phaon(*visitor);
 RPI_Output rpo(visitor_phaon);

 doc->report_graph(file_name + ".re1.txt");

 RE_Pre_Normal_Lisp prenorm1(doc);
 prenorm1.output("..prenorm1.txt");

 RE_Prerun_Anticipate anticipate(*visitor, doc->local_path() + ".cprs");

 anticipate.scan(nullptr);//[](RZ_Dynamo_Output& rzdo){rzdo.init_top_level_block();});

 anticipate.run_core_pairs_generations();

 PGB_IR_Build pgb(file_name + ".gen.pgb");

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
#endif


int main(int argc, char *argv[])
{
 //compile_rz(RZ_DIR "/demo/phaon/t1.rz");

 return 0;
}
