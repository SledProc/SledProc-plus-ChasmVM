
#           Copyright Nathaniel Christen 2020.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)


include(../build-group.pri)

QT -= gui

TEMPLATE = app

include($$ROOT_DIR/../preferred/sysr.pri)

#include($$ROOT_DIR/../preferred/sysr-c.pri)

DEFINES += DEFAULT_GTAGML_GEN_FOLDER=\\\"$$ROOT_DIR/../gtagml\\\"

DEFINES += DEFAULT_DEV_DGDB_NGML_FOLDER=\\\"$$ROOT_DIR/../gtagml/dgdb\\\"

DEFINES += DEFAULT_SDI_FOLDER=\\\"$$ROOT_DIR/dev/consoles/gtagml/sdi\\\"

DEFINES += USE_KANS


INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR $$WHITEDB_SRC_GROUP_DIR

INCLUDEPATH += $$SRC_GROUP_DIR/dgdb-white

INCLUDEPATH += $$SRC_GROUP_DIR/

INCLUDEPATH += $$SRC_ROOT_DIR/apk/gtagml/gtagml


INCLUDEPATH += $$SRC_PROSET_DIR/gtagml

INCLUDEPATH += $$NTXH_SRC_GROUP_DIR $$NTXH_SRC_GROUP_DIR/ntxh


#/home/nlevisrael/gits/ntxh/wip-sebi/ar/code/cpp/src/apk/gtagml/gtagml/ngml-sdi/ngml-sdi-paragraph.h
#INCLUDEPATH +=



INCLUDEPATH += $$RELAE_GRAPH_SRC_GROUP_DIR
INCLUDEPATH += $$PHAON_GRAPH_SRC_GROUP_DIR



HEADERS += \



SOURCES += \
  $$SRC_DIR/main.cpp \



LIBS += -L$$TARGETSDIR \
  -lntxh  -lntxh-parser -lngml-sdi  -ldgh-sdi \
  -lquazip

