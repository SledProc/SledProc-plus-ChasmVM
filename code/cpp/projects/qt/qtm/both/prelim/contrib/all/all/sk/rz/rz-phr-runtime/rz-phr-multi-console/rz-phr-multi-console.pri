
#           Copyright Nathaniel Christen 2019.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)

PROJECT_NAME = rz-phr-multi-console

include(../build-group.pri)

TEMPLATE = app
CONFIG += console

DEFINES += CAON_DEBUG
DEFINES += RELAE_LABEL_NODES

DEFINES += RZ_DIR=\\\"$$ROOT_DIR/dev/scripts/rz\\\"


INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR

INCLUDEPATH += $$RELAE_GRAPH_SRC_GROUP_DIR
INCLUDEPATH += $$PHR_GRAPH_SRC_GROUP_DIR
INCLUDEPATH += $$RZ_KAUVIR_SRC_GROUP_DIR
INCLUDEPATH += $$PHAONIR_SRC_GROUP_DIR
INCLUDEPATH += $$PHAONLIB_SRC_GROUP_DIR
INCLUDEPATH += $$PHR_RUNTIME_SRC_GROUP_DIR

CONFIG += no_keywords

DEFINES += USE_RZNS

DEFINES += USE_KANS


DEFINES += DEFAULT_PTR_BYTE_CODE=QT_POINTER_SIZE


HEADERS += \
  $$SRC_DIR/basic-functions.h \
  $$SRC_ROOT_DIR/default-basic-functions.h \
  $$SRC_ROOT_DIR/default-test-functions.h \


SOURCES += \
  $$SRC_DIR/main.cpp \
  $$SRC_DIR/local-program.cpp \
  $$SRC_DIR/basic-functions.cpp \
  $$SRC_ROOT_DIR/default-basic-functions.cpp \
  $$SRC_ROOT_DIR/default-test-functions.cpp \


LIBS += -L$$TARGETSDIR  \
   -lrz-graph-core \
   -lrz-graph-token \
   -lrz-graph-code \
   -lrz-graph-visit \
   -lrz-code-generators \
   -lrz-graph-build \
   -lrz-graph-valuer \
   -lrz-graph-embed \
   -lrz-graph-embed-run \
   -lrz-graph-run \
   -lrz-graph-sre \
   -lrz-function-def \
   -lrz-code-elements \


LIBS += -L$$TARGETSDIR  -lphr-graph-core


LIBS += -L$$TARGETSDIR -lphaon-ir -lphr-direct-eval \
  -lphr-fn-doc   -lphr-env  \
    -lphr-command-runtime


contains(CHOICE_FEATURES, "kph-gen") \#/
{
 message(DEFINE\'ing USING_KPH_GEN)
 DEFINES += USING_KPH_GEN
 LIBS += -L$$TARGETSDIR -lkph-generator
}
