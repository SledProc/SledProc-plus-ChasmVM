
#           Copyright Nathaniel Christen 2020.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)


include(../build-group.pri)


QT -= gui

include($$ROOT_DIR/../preferred/sysr.pri)

#include($$ROOT_DIR/../preferred/sysr-c.pri)

DEFINES += DEFAULT_GTAGML_GEN_FOLDER=\\\"$$ROOT_DIR/../gtagml\\\"

DEFINES += DEFAULT_DEV_DGDB_NGML_FOLDER=\\\"$$ROOT_DIR/../gtagml/dgdb\\\"

DEFINES += DEFAULT_SDI_FOLDER=\\\"$$ROOT_DIR/dev/consoles/gtagml/sdi\\\"

DEFINES += USE_KANS


INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR $$WHITEDB_SRC_GROUP_DIR

#INCLUDEPATH += $$SRC_GROUP_DIR/dgdb-white

INCLUDEPATH += $$SRC_GROUP_DIR/

INCLUDEPATH += $$SRC_ROOT_DIR/apk/gtagml/gtagml

INCLUDEPATH += $$NTXH_SRC_GROUP_DIR $$NTXH_SRC_GROUP_DIR/ntxh

INCLUDEPATH += $$SRC_PROSET_DIR/gtagml


#/home/nlevisrael/gits/ntxh/wip-sebi/ar/code/cpp/src/apk/gtagml/gtagml/ngml-sdi/ngml-sdi-paragraph.h
#INCLUDEPATH +=



INCLUDEPATH += $$RELAE_GRAPH_SRC_GROUP_DIR
INCLUDEPATH += $$PHAON_GRAPH_SRC_GROUP_DIR



HEADERS += \
  $$SRC_DIR/dgh-sdi-document.h \
  $$SRC_DIR/dgh-sdi-sentence.h \
  $$SRC_DIR/dgh-sdi-paragraph.h \



SOURCES += \
  $$SRC_DIR/dgh-sdi-document.cpp \
  $$SRC_DIR/dgh-sdi-sentence.cpp \
  $$SRC_DIR/dgh-sdi-paragraph.cpp \



LIBS += -L$$TARGETSDIR  \# -lwhitedb -ldgdb-white \
  -lntxh  -lntxh-parser -lngml-sdi

