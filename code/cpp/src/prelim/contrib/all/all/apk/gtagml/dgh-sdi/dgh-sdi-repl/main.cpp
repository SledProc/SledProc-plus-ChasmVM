
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "dgh-sdi-repl-mw/dgh-sdi-repl-mainwindow.h"

USING_KANS(DGH)

#include <QDebug>
#include <QApplication>

int main(int argc, char* argv[])
{
 QApplication qapp(argc, argv);

 DGH_SDI_REPL_MainWindow mw;

 mw.show();

 return qapp.exec();
}

