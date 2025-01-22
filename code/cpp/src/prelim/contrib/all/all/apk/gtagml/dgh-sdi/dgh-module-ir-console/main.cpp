
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "dgh-module-ir/module-ir.h"


#include <QDebug>

#include "textio.h"

USING_KANS(TextIO)

USING_KANS(DGH)


int main(int argc, char *argv[])
{
 Module_IR mir;

// mir.read_lines(R"(
//gt-compile-manuscript $ /home/nlevisrael/gits/ntxh/wip-sebi/ctg/src/ctg.tex
//  :manuscript /home/nlevisrael/gits/ntxh/wip-sebi/ctg/manuscript
//                ;.
//                )");

 mir.read_lines(QString(R"(
  .; comment ... ;.
  gt-compile-manuscript  $  :no-first  :info-path
    :setup-once  :setup-sdi
    %1/ctg/src/ctg.tex
    :manuscript  %1/ctg/manuscript
    :gt-copy-folder  %1/ctg/gt-copy
                        ;.
                  )").arg(DEMO_DOCUMENT_FOLDER) );


//  mir.read_lines(QString(R"(
//   .; comment ... ;.
//   gt-compile-file $ %1/ctg/src/intro.gt
//     ;.
//                   )").arg(DEMO_DOCUMENT_FOLDER) );


// mir.read_lines(R"(
//.; comment ... ;.
//gt-compile-file $ :setup /home/nlevisrael/lgt/champs/src/proposal.gt
//  :top-level champs.tex ;.
//                )");


 mir.run_lines();
 return 0;
}


