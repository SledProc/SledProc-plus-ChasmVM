
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include <QDebug>
#include <QFile>


#include "ngml-sdi/ngml-sdi-document.h"

#include <QDir>
#include <QDebug>

#include <QCommandLineParser>

#include <QDirIterator>

#include "dgh-sdi/dgh-sdi-document.h"

USING_KANS(DGH)


#include "textio.h"
#include "get-cmdl.h"

USING_KANS(TextIO)
USING_KANS(Util)


int main(int argc, char* argv[])
{
 QString mergefile = "";

 DGH_SDI_Document dsd;
 dsd.load_from_ntxh(mergefile);

 dsd.review_dgh();

 return 0;
}

