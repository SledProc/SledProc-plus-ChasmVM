
#           Copyright Nathaniel Christen 2020.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)


PROJECT_NAME = qh-cpp-parser


include(../build-group.pri)

QT -= gui


exists($$ROOT_DIR/../preferred/sysr.pri): include($$ROOT_DIR/../preferred/sysr.pri)
exists($$ROOT_DIR/../preferred/sysr-c.pri): include($$ROOT_DIR/../preferred/sysr-c.pri)
exists($$ROOT_DIR/../preferred/compiler.pri): include($$ROOT_DIR/../preferred/compiler.pri)

CONFIG += c++17

equals(QT_MAJOR_VERSION, 5){
DEFINES += USING_QT5
}
equals(QT_MAJOR_VERSION, 6) {
DEFINES += USING_QT6
}




INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR $$SRC_ROOT_DIR

INCLUDEPATH += $$RELAE_GRAPH_SRC_GROUP_DIR
INCLUDEPATH += $$SRC_GROUP_DIR/qh-cpp
INCLUDEPATH += $$PHAON_GRAPH_SRC_GROUP_DIR

message($$RELAE_GRAPH_SRC_GROUP_DIR)

CONFIG += no_keywords

DEFINES += USE_KANS


HEADERS += \
  $$SRC_DIR/grammar/qh-cpp-graph-build.h \
  $$SRC_DIR/grammar/qh-cpp-grammar.h \
  $$SRC_DIR/grammar/qh-cpp-parse-context.h \
  $$SRC_DIR/grammar/qh-cpp-parser.h \
  $$SRC_DIR/qh-cpp-document.h \
  $$SRC_DIR/qh-cpp-source-type.h \
  $$SRC_DIR/qh-cpp-source-file.h \



SOURCES += \
  $$SRC_DIR/grammar/qh-cpp-graph-build.cpp \
  $$SRC_DIR/grammar/qh-cpp-grammar.cpp \
  $$SRC_DIR/grammar/qh-cpp-parse-context.cpp \
  $$SRC_DIR/grammar/qh-cpp-parser.cpp \
  $$SRC_DIR/qh-cpp-document.cpp \
  $$SRC_DIR/qh-cpp-source-type.cpp \
  $$SRC_DIR/qh-cpp-source-file.cpp \




LIBS += -L$$TARGETSDIR -lqh-cpp  -lrdsc-runtime


message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)

