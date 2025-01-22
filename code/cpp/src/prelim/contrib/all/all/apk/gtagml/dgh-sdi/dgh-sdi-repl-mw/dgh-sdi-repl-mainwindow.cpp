
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "dgh-sdi-repl-mainwindow.h"
#include "get-cmdl.h"

#include "dgh-module-ir/module-ir.h"

#include "textio.h"

USING_KANS(GTagML)
USING_KANS(Util)
//USING_KANS(SDI)
USING_KANS(TextIO)

USING_KANS(DGH)


DGH_SDI_REPL_MainWindow::DGH_SDI_REPL_MainWindow()
{
 mir_ = new Module_IR;
// main_layout_ = new QVBoxLayout;

 main_text_edit_ = new QTextEdit(this);

 main_splitter_ = new QSplitter(Qt::Vertical, this);


 main_splitter_->addWidget(main_text_edit_);

 main_notebook_ = new QTabWidget(this);
 cmd_log_text_edit_ = new QTextEdit(this);
 http_log_text_edit_ = new QTextEdit(this);

 main_console_ = QImplConsole::getInstance(this);

 main_console_->set_cb([this](const QString& cmd, int* response, QString data)
 {
  if(cmd.startsWith(';'))
  {
   QStringList qsl = cmd.simplified().split(' ');
   QString cmd_str = qsl.takeFirst();
   u2 cmd_code = command_list_.value(cmd_str);
   if(cmd_code)
   {
    run_command(cmd_code, qsl);
    return;
   }
  }

  QString line;
  if(cmd.endsWith(";."))
  {
   line = lines_acc_ + cmd + "\n";
   lines_acc_.clear();
  }
  else
  {
   lines_acc_ += cmd + "\n";
   return;
  }
  mir_->read_lines(line);
  mir_->run_lines();

//  qDebug() << "cmd = " << cmd;
 });

 main_notebook_->addTab(main_console_, "REPL");
 main_notebook_->addTab(cmd_log_text_edit_, "CMD Logs");
 main_notebook_->addTab(http_log_text_edit_, "HTTP Logs");

 main_splitter_->addWidget(main_notebook_);

// main_layout_->addWidget(main_splitter_);

// main_layout_->addWidget(button_box_);

// setLayout(main_layout_);

 setCentralWidget(main_splitter_);

 setMinimumWidth(600);

 init_command_list();

}

void DGH_SDI_REPL_MainWindow::init_command_list()
{
 command_list_ = {
  {";exit", 1},
  {";x", 1},
  {";save", 2},
  {";s", 2},
  {";redo", 3},
  {";r", 3},
 };
}

void DGH_SDI_REPL_MainWindow::run_command(u2 cmd_code,
  const QStringList& qsl)
{
 switch(cmd_code)
 {
 case 1:
  close();
  break;

 case 2: qDebug() << "save"; break;
 case 3: qDebug() << "redo"; break;


 }
}



