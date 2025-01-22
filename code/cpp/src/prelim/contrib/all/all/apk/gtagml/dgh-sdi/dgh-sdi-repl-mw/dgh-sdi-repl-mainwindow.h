
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef DGH_SDI_REPL_MAINWINDOW__H
#define DGH_SDI_REPL_MAINWINDOW__H


#include <QMainWindow>

#include <QDebug>

#include <QSplitter>
#include <QTextEdit>
#include <QVBoxLayout>

#include "qconsole.h"
#include "qimplconsole.h"

#include "kans.h"

#include "global-types.h"

KANS_CLASS_DECLARE(GTagML ,GTagML_Folder)

USING_KANS(GTagML)



// KANS_(DGH)

namespace KA {  namespace DGH {

class Module_IR;


class DGH_SDI_REPL_MainWindow : public QMainWindow
{
 Q_OBJECT

 QMap<QString, u2> command_list_;

 QTextEdit* main_text_edit_;
 QSplitter* main_splitter_;

 QTabWidget* main_notebook_;
 QImplConsole* main_console_;
 QTextEdit* cmd_log_text_edit_;
 QTextEdit* http_log_text_edit_;

 void init_command_list();
 void run_command(u2 cmd_code, const QStringList& qsl);

 Module_IR* mir_;

 QString lines_acc_;


// QVBoxLayout* main_layout_;

public:

 DGH_SDI_REPL_MainWindow();
};

}} //_KANS(DGH)

#endif // DGH_SDI_REPL_MAINWINDOW__H
