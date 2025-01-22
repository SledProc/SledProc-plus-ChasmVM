
#           Copyright Nathaniel Christen 2020.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)


include(../build-group.pri)


QT += gui widgets

include($$ROOT_DIR/../preferred/sysr.pri)

#include($$ROOT_DIR/../preferred/sysr-c.pri)

DEFINES += DEFAULT_GTAGML_GEN_FOLDER=\\\"$$ROOT_DIR/../gtagml\\\"

DEFINES += DEFAULT_DEV_DGDB_NGML_FOLDER=\\\"$$ROOT_DIR/../gtagml/dgdb\\\"

DEFINES += DEFAULT_SDI_FOLDER=\\\"$$ROOT_DIR/dev/consoles/gtagml/sdi\\\"

DEFINES += USE_KANS

DEFINES += ROOT_FOLDER=\\\"$$ROOT_DIR\\\"


INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR $$WHITEDB_SRC_GROUP_DIR

INCLUDEPATH += $$SRC_GROUP_DIR/dgdb-white

INCLUDEPATH += $$SRC_GROUP_DIR/

INCLUDEPATH += $$SRC_ROOT_DIR/apk/gtagml/gtagml
INCLUDEPATH += $$SRC_ROOT_DIR/apk/hgdm/ntxh
INCLUDEPATH += $$SRC_ROOT_DIR/apk/hgdm/ntxh/ntxh

#/home/nlevisrael/gits/ntxh/wip-sebi/ar/code/cpp/src/apk/gtagml/gtagml/ngml-sdi/ngml-sdi-paragraph.h
#INCLUDEPATH +=

INCLUDEPATH += $$GTAGML_SRC_GROUP_DIR/gtagml


INCLUDEPATH += $$RELAE_GRAPH_SRC_GROUP_DIR
INCLUDEPATH += $$PHAON_GRAPH_SRC_GROUP_DIR



HEADERS += \
  $$SRC_DIR/dgh-sdi-repl-mainwindow.h \
  $$SRC_DIR/qconsole.h \
  $$SRC_DIR/qimplconsole.h \


SOURCES += \
  $$SRC_DIR/dgh-sdi-repl-mainwindow.cpp \
  $$SRC_DIR/qconsole.cpp \
  $$SRC_DIR/qimplconsole.cpp \



LIBS += -L$$TARGETSDIR \# -lwhitedb -ldgdb-white \
  -lntxh  -lntxh-parser -lngml-sdi

