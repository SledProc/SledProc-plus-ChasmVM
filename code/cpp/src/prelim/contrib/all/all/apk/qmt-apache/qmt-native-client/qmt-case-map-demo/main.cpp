
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include <QApplication>
#include <QtDebug>
#include <QMainWindow>
#include <QPushButton>

#include "main-window.h"


#include <QGeoCodingManager>

#include <QGeoCodingManagerEngine>

#include <QGeoServiceProvider>

#include "global-types.h"


#include <iostream>
#include <sstream>

// Qt header:
#include <QtWidgets>
#include <QGeoAddress>
#include <QGeoCodingManager>
#include <QGeoCoordinate>
#include <QGeoLocation>
#include <QGeoServiceProvider>

#include "lanternfly/lanternfly-frame.h"

void log(QTextEdit &qTxtLog, const QString &qString)
{
  qTxtLog.setPlainText(qTxtLog.toPlainText() + qString);
  qTxtLog.moveCursor(QTextCursor::End);
}
void log(QTextEdit &qTxtLog, const char *text)
{
  log(qTxtLog, QString::fromUtf8(text));
}
void log(QTextEdit &qTxtLog, const std::string &text)
{
  log(qTxtLog, text.c_str());
}

int main5(int argc, char **argv)
{
  qDebug() << "Qt Version:" << QT_VERSION_STR;
  // main application
  QApplication app(argc, argv);
  // setup GUI
  QWidget qWin;
  QVBoxLayout qBox;
  QFormLayout qForm;
  QLabel qLblCountry(QString::fromUtf8("Country:"));
  QLineEdit qTxtCountry;
  qForm.addRow(&qLblCountry, &qTxtCountry);
  QLabel qLblZipCode(QString::fromUtf8("Postal Code:"));
  QLineEdit qTxtZipCode;
  qForm.addRow(&qLblZipCode, &qTxtZipCode);
  QLabel qLblCity(QString::fromUtf8("City:"));
  QLineEdit qTxtCity;
  qForm.addRow(&qLblCity, &qTxtCity);
  QLabel qLblStreet(QString::fromUtf8("Street:"));
  QLineEdit qTxtStreet;
  qForm.addRow(&qLblStreet, &qTxtStreet);
  QLabel qLblProvider(QString::fromUtf8("Provider:"));
  QComboBox qLstProviders;
  qForm.addRow(&qLblProvider, &qLstProviders);
  qBox.addLayout(&qForm);
  QPushButton qBtnFind(QString::fromUtf8("Find Coordinates"));
  qBox.addWidget(&qBtnFind);
  QLabel qLblLog(QString::fromUtf8("Log:"));
  qBox.addWidget(&qLblLog);
  QTextEdit qTxtLog;
  qTxtLog.setReadOnly(true);
  qBox.addWidget(&qTxtLog);
  qWin.setLayout(&qBox);
  qWin.show();

  // initialize Geo Service Providers
  std::vector<QGeoServiceProvider*> pQGeoProviders;
  {
   std::ostringstream out;
    QStringList qGeoSrvList
      = QGeoServiceProvider::availableServiceProviders();
    for (QString entry : qGeoSrvList) {
      out << "Try service: " << entry.toStdString() << '\n';
      // choose provider
      QGeoServiceProvider *pQGeoProvider = new QGeoServiceProvider(entry);
      if (!pQGeoProvider) {
        out
          << "ERROR: GeoServiceProvider '" << entry.toStdString()
          << "' not available!\n";
        continue;
      }
      QGeoCodingManager *pQGeoCoder = pQGeoProvider->geocodingManager();
      if (!pQGeoCoder) {
        out
          << "ERROR: GeoCodingManager '" << entry.toStdString()
          << "' not available!\n";
        delete pQGeoProvider;
        continue;
      }
      QLocale qLocaleC(QLocale::C, QLocale::AnyCountry);
      pQGeoCoder->setLocale(qLocaleC);
      qLstProviders.addItem(entry);
      pQGeoProviders.push_back(pQGeoProvider);
      out << "Service " << entry.toStdString() << " available.\n";
    }
    log(qTxtLog, out.str());
  }
  if (pQGeoProviders.empty()) qBtnFind.setEnabled(false);
  // install signal handlers
  QObject::connect(&qBtnFind, &QPushButton::clicked,
    [&]() {
      // get current geo service provider
      QGeoServiceProvider *pQGeoProvider
        = pQGeoProviders[qLstProviders.currentIndex()];
      // fill in request
      QGeoAddress *pQGeoAddr = new QGeoAddress;
      pQGeoAddr->setCountry(qTxtCountry.text());
      pQGeoAddr->setPostalCode(qTxtZipCode.text());
      pQGeoAddr->setCity(qTxtCity.text());
      pQGeoAddr->setStreet(qTxtStreet.text());
      QGeoCodeReply *pQGeoCode
        = pQGeoProvider->geocodingManager()->geocode(*pQGeoAddr);
      if (!pQGeoCode) {
        delete pQGeoAddr;
        log(qTxtLog, "GeoCoding totally failed!\n");
        return;
      }
      { std::ostringstream out;
        out << "Sending request for:\n"
          << pQGeoAddr->country().toUtf8().data() << "; "
          << pQGeoAddr->postalCode().toUtf8().data() << "; "
          << pQGeoAddr->city().toUtf8().data() << "; "
          << pQGeoAddr->street().toUtf8().data() << "...\n";
        log(qTxtLog, out.str());
      }
      // install signal handler to process result later
      QObject::connect(pQGeoCode, &QGeoCodeReply::finished,
        [&qTxtLog, pQGeoAddr, pQGeoCode]() {
          // process reply
          std::ostringstream out;
          out << "Reply: " << pQGeoCode->errorString().toStdString() << '\n';
          switch (pQGeoCode->error()) {
            case QGeoCodeReply::NoError: {
              // eval result
              QList<QGeoLocation> qGeoLocs = pQGeoCode->locations();
              out << qGeoLocs.size() << " location(s) returned.\n";
              for (QGeoLocation &qGeoLoc : qGeoLocs) {
                qGeoLoc.setAddress(*pQGeoAddr);
                QGeoCoordinate qGeoCoord = qGeoLoc.coordinate();
                out
                  << "Lat.:  " << qGeoCoord.latitude() << '\n'
                  << "Long.: " << qGeoCoord.longitude() << '\n'
                  << "Alt.:  " << qGeoCoord.altitude() << '\n';
              }
            } break;
#define CASE(ERROR) \
            case QGeoCodeReply::ERROR: out << #ERROR << '\n'; break
            CASE(EngineNotSetError);
            CASE(CommunicationError);
            CASE(ParseError);
            CASE(UnsupportedOptionError);
            CASE(CombinationError);
            CASE(UnknownError);
#undef CASE
            default: out << "Undocumented error!\n";
          }
          // log result
          log(qTxtLog, out.str());
          // clean-up
          delete pQGeoAddr;
          /* delete sender in signal handler could be lethal
           * Hence, delete it later...
           */
          pQGeoCode->deleteLater();
        });
    });
  // fill in a sample request with a known address initially
  qTxtCountry.setText(QString::fromUtf8("Germany"));
  qTxtZipCode.setText(QString::fromUtf8("88250"));
  qTxtCity.setText(QString::fromUtf8("Weingarten"));
  qTxtStreet.setText(QString::fromUtf8("Danziger Str. 3"));
  // runtime loop
  app.exec();
  // done
  return 0;
}

#include <QEventLoop>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

int main4(int argc, char *argv[])
{
 QApplication qapp(argc, argv);

 QGeoServiceProvider gsp("osm");
 QGeoCodingManager* gcm = gsp.geocodingManager();

// QGeoServiceProvider gsp("esri");
// QGeoCodingManager* gcm = gsp.geocodingManager();

 QGeoAddress* pQGeoAddr = new QGeoAddress;

 pQGeoAddr->setCountry("USA");
 pQGeoAddr->setPostalCode("07024");
 pQGeoAddr->setCity("Fort Lee");
 pQGeoAddr->setState("NJ");
 pQGeoAddr->setStreet("320 Main Street");

// pQGeoAddr->setStreetNumber("320MainStreet");
// pQGeoAddr->setText("1A");


// pQGeoAddr->setCountry("Germany");
// pQGeoAddr->setPostalCode("88250");
// pQGeoAddr->setCity("Weingarten");
// pQGeoAddr->setStreet("Danziger");


 QGeoCodeReply* pQGeoCode
   = gcm->geocode(*pQGeoAddr);

// QGeoCodeReply* pQGeoCode
//   = gcm->geocode("320MainStreet,1A,FortLee,NJ,USA");


// QString ss = QSslSocket::sslLibraryBuildVersionString();
// qDebug() << "sslLibraryBuildVersionString = " << ss;

// if(QSslSocket::supportsSsl())
//   qDebug() << "supportsSsl";
// else
//   qDebug() << "!supportsSsl";


// QNetworkRequest qnr;
// qnr.setUrl(QUrl(
//   "http://geocode.arcgis.com/arcgis/rest/services/World/GeocodeServer/findAddressCandidates?singleLine=320MainStreet,1A,FortLee,NJ,USA&f=json&outFields=*"));

// QNetworkAccessManager qnam;

 QEventLoop qel;

// QNetworkReply* reply = qnam.get(qnr);

// QObject::connect(reply, &QNetworkReply::finished, [reply, &qel]()
// {
//  QByteArray qba = reply->readAll();
//  qDebug() << "qba = " << qba;

//  qel.exit();
// });

 QList<QGeoLocation> reply_locations;


 QObject::connect(pQGeoCode, &QGeoCodeReply::finished,
   [pQGeoCode, &qel, &reply_locations]
 {
  qDebug() << "Reply: " << pQGeoCode->errorString();

  switch (pQGeoCode->error())
  {
   case QGeoCodeReply::NoError:
     reply_locations = pQGeoCode->locations();
     break;
#define CASE(ERROR) \
    case QGeoCodeReply::ERROR: qDebug() << #ERROR << '\n'; break;
    CASE(EngineNotSetError)
    CASE(CommunicationError)
    CASE(ParseError)
    CASE(UnsupportedOptionError)
    CASE(CombinationError)
    CASE(UnknownError)
#undef CASE
    default: qDebug() << "Undocumented error!\n";
  }

  qel.exit();
 });

 qel.exec();

 if(reply_locations.isEmpty())
   return 0;

 QGeoLocation loc1 = reply_locations.first();
 r8 lat1 = loc1.coordinate().latitude();
 r8 lon1 = loc1.coordinate().longitude();

 qDebug() << "lat 1 = " << lat1 << " and lon 1 = " << lon1;

 r8 lat = -74.0271;
 r8 lon = 40.8896;


 QGeoLocation loc2;
 loc2.setCoordinate(QGeoCoordinate(lat, lon));


 QGeoCodeReply* gcr
   = gcm->reverseGeocode(loc2.coordinate());

 QObject::connect(gcr, &QGeoCodeReply::finished,
   [&qel, gcr]
 {

  switch (gcr->error())
  {
   case QGeoCodeReply::NoError:
     qDebug() << "address: " << gcr->locations().first().address().text();
     break;
#define CASE(ERROR) \
    case QGeoCodeReply::ERROR: qDebug() << #ERROR << '\n'; break;
    CASE(EngineNotSetError)
    CASE(CommunicationError)
    CASE(ParseError)
    CASE(UnsupportedOptionError)
    CASE(CombinationError)
    CASE(UnknownError)
#undef CASE
    default: qDebug() << "Undocumented error!\n";
  }

  qDebug() << gcr->errorString();

  qel.exit();
 });



 qel.exec();


// QGeoCodingManagerEngine gcme()
// QGeoCodingManager m();
// QGeoCodeReply* gcr = m.geocode();

//    QApplication a(argc, argv);

//    Lanternfly_Main_Window w;
//    w.show();

//    return a.exec();
}

// // initialize Geo Service Providers
//std::vector<QGeoServiceProvider*> pQGeoProviders;

//{
// std::ostringstream out;

// QStringList qGeoSrvList
//   = QGeoServiceProvider::availableServiceProviders();

// for (QString entry : qGeoSrvList)
// {
//  out << "Try service: " << entry.toStdString() << '\n';
//  // choose provider
//  QGeoServiceProvider *pQGeoProvider = new QGeoServiceProvider(entry);
//  if (!pQGeoProvider)
//  {
//   out
//     << "ERROR: GeoServiceProvider '" << entry.toStdString()
//     << "' not available!\n";
//   continue;
//  }
//  QGeoCodingManager *pQGeoCoder = pQGeoProvider->geocodingManager();
//  if (!pQGeoCoder)
//  {
//   out
//     << "ERROR: GeoCodingManager '" << entry.toStdString()
//     << "' not available!\n";
//   delete pQGeoProvider;
//   continue;
//  }
//  QLocale qLocaleC(QLocale::C, QLocale::AnyCountry);
//  pQGeoCoder->setLocale(qLocaleC);
////?   qLstProviders.addItem(entry);
//  pQGeoProviders.push_back(pQGeoProvider);
//  out << "Service " << entry.toStdString() << " available.\n";
// }
//}


void address_to_lat_lon(QGeoCodingManager* gcm,
  QString street, QString muni, int& size, r8& lat, r8& lon, QString& str)
{

// // install signal handlers
// auto cb = [&]()
// {
  // get current geo service provider
  //?QGeoServiceProvider* pQGeoProvider;
    //= pQGeoProviders[qLstProviders.currentIndex()];

  // fill in request
 QGeoAddress* pQGeoAddr = new QGeoAddress;

 pQGeoAddr->setCountry("USA");
 //? pQGeoAddr->setPostalCode(qTxtZipCode.text());

 pQGeoAddr->setStreet(street);
 pQGeoAddr->setCity(muni);
 pQGeoAddr->setState("NJ");


 QGeoCodeReply *pQGeoCode
   = gcm->geocode(*pQGeoAddr);

 if (!pQGeoCode)
 {
  delete pQGeoAddr;
  qDebug() << "GeoCoding totally failed!";
  return;
 }
 {
  std::ostringstream out;
  out << "Sending request for:\n"
      << pQGeoAddr->country().toUtf8().data() << "; "
      << pQGeoAddr->postalCode().toUtf8().data() << "; "
      << pQGeoAddr->city().toUtf8().data() << "; "
      << pQGeoAddr->street().toUtf8().data() << "...\n";
  qDebug() << QString::fromStdString( out.str() );
 }

 QEventLoop qel;

 // QNetworkReply* reply = qnam.get(qnr);

 // QObject::connect(reply, &QNetworkReply::finished, [reply, &qel]()
 // {
 //  QByteArray qba = reply->readAll();
 //  qDebug() << "qba = " << qba;

 //  qel.exit();
 // });

 QList<QGeoLocation> reply_locations;

  // install signal handler to process result later
 QObject::connect(pQGeoCode, &QGeoCodeReply::finished,
                  [pQGeoAddr, pQGeoCode, &reply_locations, &qel]()
 {

  // process reply
  std::ostringstream out;
  out << "Reply: " << pQGeoCode->errorString().toStdString() << '\n';
  switch (pQGeoCode->error())
  {
  case QGeoCodeReply::NoError:
   {
   // eval result
    reply_locations = pQGeoCode->locations();
//     out << qGeoLocs.size() << " location(s) returned.\n";
//     for (QGeoLocation &qGeoLoc : qGeoLocs)
//     {
//      qGeoLoc.setAddress(*pQGeoAddr);
//      QGeoCoordinate qGeoCoord = qGeoLoc.coordinate();
//      out
//        << "Lat.:  " << qGeoCoord.latitude() << '\n'
//        << "Long.: " << qGeoCoord.longitude() << '\n'
//        << "Alt.:  " << qGeoCoord.altitude() << '\n';
//     }
   }
   break;
#define CASE(ERROR) \
   case QGeoCodeReply::ERROR: out << #ERROR << '\n'; break
    CASE(EngineNotSetError);
    CASE(CommunicationError);
    CASE(ParseError);
    CASE(UnsupportedOptionError);
    CASE(CombinationError);
    CASE(UnknownError);
#undef CASE
    default: out << "Undocumented error!\n";
  }
   // log result

  qDebug() << QString::fromStdString( out.str() );
  // clean-up
  delete pQGeoAddr;
   /* delete sender in signal handler could be lethal
         * Hence, delete it later...
         */
  pQGeoCode->deleteLater();
  qel.exit();

 });

 qel.exec();

 if(reply_locations.isEmpty())
 {
  lat = 0;
  lon = 0;
  str = QString {};
  return;
 }


 size = reply_locations.size();

 QGeoLocation loc1 = reply_locations.first();
 lat = loc1.coordinate().latitude();
 lon = loc1.coordinate().longitude();

 for(QGeoLocation loc : reply_locations)
 {
  str += "%1/%2;"_qt.arg(loc.coordinate().latitude()).arg(loc.coordinate().longitude());
 }

// qDebug() << "lat 1 = " << lat1 << " and lon 1 = " << lon1;
// };

}



#include "qtcsv/stringdata.h"
#include "qtcsv/reader.h"
#include "qtcsv/writer.h"

int main(int argc, char *argv[])
{
 QApplication qapp(argc, argv);

 QGeoServiceProvider gsp("osm");
 QGeoCodingManager* gcm = gsp.geocodingManager();

 QString path = "/home/nlevisrael/docker/tox/KCSNJ/active-m-m.csv";
 QString new_path_f = "/home/nlevisrael/docker/tox/KCSNJ/active-m-m-f.csv";
 QString new_path_m = "/home/nlevisrael/docker/tox/KCSNJ/active-m-m-m.csv";

// QString path = "/home/nlevisrael/docker/tox/KCSNJ/active-missing.csv";
// QString new_path_f = "/home/nlevisrael/docker/tox/KCSNJ/active-m-f.csv";
// QString new_path_m = "/home/nlevisrael/docker/tox/KCSNJ/active-m-m.csv";

 QList<QStringList> lines = QtCSV::Reader::readToList(path);

 QtCSV::StringData new_lines_f;
 QtCSV::StringData new_lines_m;

 QStringList header = lines.takeFirst();

 new_lines_f.addRow(header);
 new_lines_f.addEmptyRow();

 new_lines_m.addRow(header);
 new_lines_m.addEmptyRow();

 int i = 0;

 for(QStringList line : lines)
 {
  ++i;

  if(i % 25 == 0)
  {
   QtCSV::Writer::write(new_path_f, new_lines_f);
   QtCSV::Writer::write(new_path_m, new_lines_m);
  }

  if(line.isEmpty())
  {
   new_lines_f.addEmptyRow();
   new_lines_m.addEmptyRow();
   continue;
  }


  QString street = line.value(3);
  QString muni = line.value(5);

  muni.replace("Twp", "Township");

  if(muni == "Kearny Town")
  {
   muni.chop(5);
  }

  else if(muni.endsWith(" Boro"))
  {
   muni.chop(5);
  }
  else if(muni.endsWith(" Village"))
  {
   muni.chop(8);
  }
  else if(muni.endsWith(" City"))
  {
   if(muni == "Union City")
     goto after_cities;
   if(muni == "Jersey City")
     goto after_cities;
   if(muni == "Perth Amboy City")
     goto after_cities;
   if(muni == "Atlantic City")
     goto after_cities;
   if(muni == "Gloucester City")
     goto after_cities;

   muni.chop(5);
  }

 after_cities:



  int loc_size = 0;
  r8 lat, lon;

  QString str;


  address_to_lat_lon(gcm, street, muni, loc_size, lat, lon, str);

  if(loc_size)
  {
   line[7] = QString::number(lat);
   line[8] = QString::number(lon);
   line[9] = QString::number(loc_size);
   line[10] = str;
   line[11] = "Active (found)";
   new_lines_f.addRow(line);
  }
  else
  {
   new_lines_m.addRow(line);
  }
 }

 QtCSV::Writer::write(new_path_f, new_lines_f);
 QtCSV::Writer::write(new_path_m, new_lines_m);

}




int main30(int argc, char *argv[])
{
 QString path = "/home/nlevisrael/docker/tox/KCSNJ/active-ll.csv";
 QString new_path_f = "/home/nlevisrael/docker/tox/KCSNJ/active-found.csv";
 QString new_path_m = "/home/nlevisrael/docker/tox/KCSNJ/active-missing.csv";

 QList<QStringList> lines = QtCSV::Reader::readToList(path);

 QtCSV::StringData new_lines_f;
 QtCSV::StringData new_lines_m;

 QStringList header = lines.takeFirst();

 header.push_back("Source");

 new_lines_f.addRow(header);
 new_lines_f.addEmptyRow();

 new_lines_m.addRow(header);
 new_lines_m.addEmptyRow();

 int i = 0;

 for(QStringList line : lines)
 {
  ++i;

  if(line.isEmpty())
  {
   new_lines_f.addEmptyRow();
   new_lines_m.addEmptyRow();
   continue;
  }

  int sz = line.value(9).toInt();

  if(sz)
  {
   line.push_back("Active (found)");
   new_lines_f.addRow(line);
  }
  else
  {
   line.push_back("Active (missing)");
   new_lines_m.addRow(line);
  }
 }

 QtCSV::Writer::write(new_path_f, new_lines_f);
 QtCSV::Writer::write(new_path_m, new_lines_m);

}

int main25(int argc, char *argv[])
{
 QApplication qapp(argc, argv);

 QGeoServiceProvider gsp("osm");
 QGeoCodingManager* gcm = gsp.geocodingManager();




 QString path = "/home/nlevisrael/docker/tox/KCSNJ/pending-clean.csv";
 QString new_path = "/home/nlevisrael/docker/tox/KCSNJ/pending-ll.csv";


// QString path = "/home/nlevisrael/docker/tox/KCSNJ/active-clean.csv";
// QString new_path = "/home/nlevisrael/docker/tox/KCSNJ/active-ll.csv";

 QList<QStringList> lines = QtCSV::Reader::readToList(path);

 QtCSV::StringData new_lines;

 QStringList header = lines.takeFirst();

 header.push_back("Latitude");
 header.push_back("Longitude");
 header.push_back("loc Size");
 header.push_back("loc String");

 new_lines.addRow(header);

 int i = 0;

 for(QStringList line : lines)
 {
  ++i;
//  if(i > 7)
//    break;

  if(line.isEmpty())
  {
   new_lines.addEmptyRow();
   continue;
  }

  QString street = line.value(3);

  // //  5 is for "active"; 6 for "pending"
   //?  QString muni = line.value(5);
  QString muni = line.value(6);

  muni.replace("Twp", "Township");

  if(muni == "Kearny Town")
  {
   muni.chop(5);
  }

  else if(muni.endsWith(" Boro"))
  {
   muni.chop(5);
  }
  else if(muni.endsWith(" Village"))
  {
   muni.chop(8);
  }
  else if(muni.endsWith(" City"))
  {
   if(muni == "Union City")
     goto after_cities;
   if(muni == "Jersey City")
     goto after_cities;
   if(muni == "Perth Amboy City")
     goto after_cities;
   if(muni == "Atlantic City")
     goto after_cities;
   if(muni == "Gloucester City")
     goto after_cities;

   muni.chop(5);
  }

 after_cities:




  qDebug() << street << " " << muni;

  int loc_size = 0;
  r8 lat, lon;

  QString str;


  address_to_lat_lon(gcm, street, muni, loc_size, lat, lon, str);

  line.push_back(QString::number(lat));
  line.push_back(QString::number(lon));
  line.push_back(QString::number(loc_size));
  line.push_back(str);

  new_lines.addRow(line);
 }

 QtCSV::Writer::write(new_path, new_lines);


 return 0;

}


int main24(int argc, char *argv[])
{
// QString path = "/home/nlevisrael/docker/tox/KCSNJ/active.csv";
// QString new_path = "/home/nlevisrael/docker/tox/KCSNJ/active-clean.csv";

 QString path = "/home/nlevisrael/docker/tox/KCSNJ/pending.csv";
 QString new_path = "/home/nlevisrael/docker/tox/KCSNJ/pending-clean.csv";

 QString current_county;
 QString current_muni;

 QMap<QString, int> site_counts;


 // read data from file
 QList<QStringList> lines = QtCSV::Reader::readToList(path);

 QtCSV::StringData new_lines;

 for (QStringList line : lines)
 {
  QString l1 = line.value(1);
  if(l1.isEmpty())
    continue;

  int index = l1.indexOf(" -- ");
  if(index != -1)
  {
   current_county = l1.left(index);
   current_muni = l1.mid(index + 4);
   continue;
  }

  if(l1 == "Site ID")
  {
   // //  check to see if this is our first header ...
   if(new_lines.isEmpty())
   {
    line.takeFirst();

    line.append("Muni");
    line.append("County");
    new_lines.addRow(line);
    new_lines.addEmptyRow();
   }
   continue;
  }

  QString l3 = line.value(3);
  if(l3.isEmpty())
  {
   site_counts[current_muni] = l1.toInt();
   new_lines.addEmptyRow();
   continue;
  }

  QString first = line.takeFirst();

  if(!first.isEmpty())
    qDebug() << "Unexpeceted nonempty field: " << first;


  line.append(current_muni);
  line.append(current_county);

  new_lines.addRow(line);
 }

 QtCSV::Writer::write(new_path, new_lines);



 return 0;
}




int main23(int argc, char *argv[])
{
// QString path = "/home/nlevisrael/docker/tox/KCSNJ/active.csv";
// QString new_path = "/home/nlevisrael/docker/tox/KCSNJ/active-clean.csv";

 QString path = "/home/nlevisrael/docker/tox/KCSNJ/pending.csv";
 QString new_path = "/home/nlevisrael/docker/tox/KCSNJ/pending-clean.csv";

 QString current_county;
 QString current_muni;

 QString held;

 enum Read_State {
  Start, County_Start, Muni_Start, Muni_Data, Muni_End,
  Muni_End_Held
 };

 Read_State read_state = Start;

 QMap<QString, int> site_counts;


 // read data from file
 QList<QStringList> lines = QtCSV::Reader::readToList(path);

 QtCSV::StringData new_lines;

// strData.addRow(strList);
// strData.addEmptyRow();
// strData << strList << "this is the last row";



 for (QStringList line : lines)
 {
  QString l1 = line.value(1);
  if(l1.isEmpty())
    continue;

  if(read_state == Muni_End)
  {
   held = l1;
   read_state = Muni_End_Held;
   continue;
  }


  if(read_state == Start)
  {
   current_county = l1;
   read_state = County_Start;
   continue;
  }

  if(read_state == County_Start)
  {
   current_muni = l1;
   read_state = Muni_Start;
   continue;
  }

  if(l1 == "Site ID")
  {
   // //  check to see if this is our first header ...
   if(new_lines.isEmpty())
   {
    line.takeFirst();
    line.append("Muni");
    line.append("County");
    new_lines.addRow(line);
    new_lines.addEmptyRow();
   }


   if(read_state == Muni_End_Held)
     // //  new muni in same county
     current_muni = held;
   read_state = Muni_Data;
   continue;
  }

  // //  do this after Site Id check
  if(read_state == Muni_End_Held)
  {
   current_county = held;
   current_muni = l1;
   read_state = Muni_Data;
   new_lines.addEmptyRow();
   continue;
  }

  QString l2 = line.value(2);
  if(l2 == "Site Count")
  {
   site_counts[current_muni] = l1.toInt();
   read_state = Muni_End;
   new_lines.addEmptyRow();
   continue;
  }

  QString first = line.takeFirst();

  if(!first.isEmpty())
    qDebug() << "Unexpeceted nonempty field: " << first;


  line.append(current_muni);
  line.append(current_county);

  new_lines.addRow(line);
 }

 QtCSV::Writer::write(new_path, new_lines);



 return 0;
}




int main11(int argc, char *argv[])
{
 QApplication a(argc, argv);

 Lanternfly_Main_Window w;

 QGeoLocation loc;
 loc.setCoordinate(QGeoCoordinate(40.8782, -74.1096));

 w.lanternfly_frame()->handle_location_marker_request(loc);


 w.show();

 return a.exec();


//    QGeoLocation loc;
//    loc.setCoordinate(QGeoCoordinate(latlon.first, latlon.second));
//    Q_EMIT(location_marker_requested(loc));

}








int main1(int argc, char *argv[])
{
 QApplication qapp(argc, argv);


}



