
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rz-prerun-anticipate.h"

#include "rz-graph-visit/rz-asg-visitor.h"

#include "rz-graph-core/kernel/graph/chasm-rz-graph.h"

#include "textio.h"

USING_RZNS(RZ_Core)
USING_RZNS(GBuild)

USING_KANS(TextIO)

RZ_Prerun_Anticipate::RZ_Prerun_Anticipate(RZ_ASG_Visitor& visitor
  , QString core_pairs_path)
 : visitor_(visitor), core_pairs_path_(core_pairs_path)
{

}

void RZ_Prerun_Anticipate::scan(std::function<void(RZ_Dynamo_Output&)> fn)
{
 visitor_.anticipate(fn);
}

void RZ_Prerun_Anticipate::write_core_pairs(int generation)
{
 QString text;
 visitor_.write_core_pairs(generation, text);
 if(!text.isEmpty())
   save_file(QString("%1.%2.txt").arg(core_pairs_path_).arg(generation), text);
}

int RZ_Prerun_Anticipate::run_core_pairs(int generation)
{
 return visitor_.run_core_pairs(generation);
}

void RZ_Prerun_Anticipate::run_core_pairs_generations()
{
 int g = 0;
 while(true)
 {
  write_core_pairs(g);
  int sz = run_core_pairs(g);
  ++g;
  if(g >= sz)
    break;
 }
}
