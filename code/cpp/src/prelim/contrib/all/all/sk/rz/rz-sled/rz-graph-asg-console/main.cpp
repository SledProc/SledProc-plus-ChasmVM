
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

int main(int argc, char *argv[])
{
 compile_rz(RZ_DIR "/demo/asg/t1.rz");

 return 0;
}

//#endif // HIDE
