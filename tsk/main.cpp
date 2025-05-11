
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)



#include "rdsc-qh/qh-web-view-dialog.h"

#include "game/game-driver.h"

#include "rdsc-2d/mat2d.templates.h"
#include "rdsc-1d/vec1d.h"

#include "kans.h"

#include <QJsonArray>

#include <QNetworkReply>
#include <QStatusBar>
#include <QLabel>

#include "game/message-display-window.h"

USING_KANS(RdSC)



int mai1(int argc, char *argv[])
{
 u1 test = 8;

 qDebug() << "test = " << test << ": " << (test >> 4) << ", " << (test & 15);

// Game_Board board;
//#define folder "/home/nlevisrael/gits/dcr-wip/qmt/server-files/public"
////? board.debug_board("/home/nlevisrael/gits/dcr-wip/game/" "test.txt");
// board.to_svg(folder "/board-svg", folder "/board.svg");

// return 0;
// QApplication qapp(argc, argv);
// QH_Web_View_Dialog dlg("file:///home/nlevisrael/gits/dcr-wip/qmt/server-files/public/test.html");

// dlg.show();

// return qapp.exec();
}

#include <QDir>
#include <QDirIterator>

#include "textio.h"



QString process_file(QDir qd, QString path)
{
 QString contents = load_file(path);

 QRegularExpression rx("%%([\\w-]+)%%");

 QRegularExpressionMatchIterator it(rx.globalMatch(contents));

 QVector<QPair<QString, QString>> inserts;

 while(it.hasNext())
 {
  QRegularExpressionMatch m = it.next();

  QString key = m.captured(1);

  qDebug() << "key = " << key;

  QString insert = load_file(qd.absoluteFilePath(key + ".htm"));

  inserts.push_back({m.captured(), insert});
 }

 for(QPair<QString, QString> pr : inserts)
 {
  contents.replace(pr.first, pr.second);
 }

 return contents;
}


int main(int argc, char *argv[])
{
 QDir qd("/home/nlevisrael/gits/baltimore/ar/site/");

 QStringList types;
 types << "*.html" << "*.htm";

 QDir iqd(qd.absoluteFilePath("in"));
 QDir oqd(qd.absolutePath());

 QDirIterator it(iqd.path(), types);
 while (it.hasNext())
 {
  QString path = it.next();

  QString c = process_file(QDir(qd.absoluteFilePath("inserts")), path);

  QFileInfo qfi(path);

  QString o_path = oqd.absoluteFilePath(qfi.fileName());

  save_file(o_path, c);

 }


}



void make_svg(QString& result)
{
 u2 h_offset = 10;
 u2 v_offset = 4;

 u2 wh = 20;


 result += QString(R"(
<svg viewBox="0 0 340 340" xmlns="http://www.w3.org/2000/svg">
)");


 for(u2 row = 0; row < 16; ++row)
 {
  for(u2 col = 0; col < 16; ++col)
  {
   result += QString(R"(
<rect x="%1" y="%2" width="%3" height="%3" id="r%4c%5"
 fill="none" stroke="black" stroke-width=".5"/>)")
     .arg(row * wh + h_offset).arg(col * wh + v_offset).arg(wh)
     .arg(row + 1).arg(col + 1);


  }
 }

 result += "\n\n</svg>";

}


int main1(int argc, char *argv[])
{
// QString t_file = "/home/nlevisrael/gits/tsk/board.svg";
// QString contents;
// make_svg(contents);
// KA::TextIO::save_file(t_file, contents);

//#define folder "/home/nlevisrael/gits/dcr-wip/qmt/server-files/public"



 QDir qd(".");

 qd.cdUp();
 qd.cd("html");

 QString game_html = qd.absoluteFilePath("test05.html");
 QUrl game_url = QUrl::fromLocalFile(game_html);

//  // // //? board.debug_board("/home/nlevisrael/gits/dcr-wip/game/" "test.txt");
// #define folder "/home/nlevisrael/gits/dcr-wip/qmt/server-files/public"
// driver.board().to_svg(folder "/board-svg", folder "/board70.svg");

//   // return 0;

// QUrl game_url = QUrl("http://amyneustein.com/-prv/sahana/emails/spreadsheet.htm");


 Game_Driver driver;

 driver.board().to_svg(qd.absoluteFilePath("board-svg"),
                       qd.absoluteFilePath("board1.svg"));


// return 0;

 QApplication qapp(argc, argv);

 QH_Web_View_Dialog dlg(game_url.toString());
// QH_Web_View_Dialog dlg("file:///home/nlevisrael/gits/tsk/html/test05.html");


 dlg.setWindowTitle("Index/Browse");

// dlg.showMinimized();
// dlg.showNormal();
// dlg.setParent(&mw);

 //dlg.stat

 driver.start_game(dlg);

 dlg.set_js_callback([&driver, &dlg](QH_Web_View_Dialog* _dlg, QString key, const QJsonValue& msg)
 {  
  qDebug() << "key = " << key;

  QString element_id;
  if(msg.isArray())
    element_id = msg.toArray().first().toString();
  else
    element_id = msg.toString();

  if(key == "setup-tokens")
    driver.handle_setup_tokens(*_dlg);

  if(key == "position-clicked")
    driver.handle_position_clicked(*_dlg, element_id);

  else if(key == "token-clicked")
    driver.handle_token_clicked(*_dlg,  element_id);

  else if(key == "text-indicator-clicked")
    driver.handle_text_indicator_clicked(*_dlg,  element_id);

  else if(key == "move-indicator-clicked")
    driver.handle_move_indicator_clicked(*_dlg,  element_id);


  else if(key == "position-context-menu")
  {
   QPoint qp(msg.toArray().at(1).toInt(), msg.toArray().at(2).toInt());
   //QPoint qp1(msg.toArray().at(3).toInt(), msg.toArray().at(4).toInt());

   //   u2 x = msg.toArray().at(1).toInt();
//   u2 y = msg.toArray().at(2).toInt();
   qDebug() << "pos id = " <<  element_id << " qp = " << qp << "ey = " << msg.toArray().at(3).toInt()
            << " pY = " << msg.toArray().at(4).toInt() << " == " << qp.y() + msg.toArray().at(4).toInt();

//?   QMenu* menu = new QMenu;
//?   menu->addAction("test1");
//?   menu->addAction("test2");

   QPoint gp = dlg.map_point_for_context_menu(qp);

   driver.handle_position_context_menu(dlg, element_id, gp);

//?   menu->popup(np);
  }

  else if(key == "token-context-menu")
  {
   QPoint qp(msg.toArray().at(1).toInt(), msg.toArray().at(2).toInt());
//   u2 x = msg.toArray().at(1).toInt();
//   u2 y = msg.toArray().at(2).toInt();
   //qDebug() << "token id = " <<  element_id << " x = " << x << " y = " << y;

   QPoint gp = dlg.map_point_for_context_menu(qp);

//   driver.handle_position_clicked()
   driver.handle_token_context_menu(dlg, element_id, gp);
  }

 });

   //"file:///home/nlevisrael/gits/dcr-wip/qmt/server-files/public/test.html");

 dlg.show();

 dlg.resize(1000, 810);


 Message_Display_Window mdw;

 //mw.setParent(&dlg);

 //mw.setWindowFlags(Qt::WindowStaysOnTopHint);

  // mw.statusBar()->showMessage("game starting ...");
  // mdw.set_status_bar(mw.statusBar());
 //mw.show();

 mdw.add_message("game starting ...");

 driver.set_message_display_window(&mdw);

 dlg.connect(&dlg, &QDialog::finished, [&mdw](int diallg_result)
 {
  mdw.close();
 });

 mdw.show();


 return qapp.exec();
}



int mai32(int argc, char *argv[])
{
QApplication qapp(argc, argv);
  QNetworkRequest qnr;
  qnr.setUrl(QUrl(
    "http://localhost:6600/qmt-l/rs~svg/board"));

  QNetworkAccessManager qnam;

  QEventLoop qel;

  QNetworkReply* reply = qnam.get(qnr);

  QObject::connect(reply, &QNetworkReply::finished, [reply, &qel]()
  {
   QByteArray qba = reply->readAll();
   qDebug() << "qba = " << qba;

   qel.exit();
  });

  qel.exec();

  qapp.exit();
}


//template<typename T>
//struct modp { T t1; T t2; modp(T _t1, T _t2) : t1(_t1), t2(_t2) {}
//  friend bool operator %= (modp lhs, u1 i) {return lhs.t1 % i == lhs.t2 % i;} };
//#define Loop_over(...) [](){ __VA_ARGS__ result;
//#define Loop_result(...) return result; }();

//#define Loop_(x, ...)  Loop_##x(__VA_ARGS__)

