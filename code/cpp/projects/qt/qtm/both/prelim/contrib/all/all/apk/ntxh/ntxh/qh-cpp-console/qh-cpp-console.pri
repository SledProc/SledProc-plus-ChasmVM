
#           Copyright Nathaniel Christen 2020.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)


PROJECT_NAME = qh-cpp-console

include(../build-group.pri)

include($$ROOT_DIR/../preferred/poppler.pri)




TEMPLATE = app

QT += widgets

#QT -= gui

exists($$ROOT_DIR/../preferred/sysr.pri): include($$ROOT_DIR/../preferred/sysr.pri)
exists($$ROOT_DIR/../preferred/sysr-c.pri): include($$ROOT_DIR/../preferred/sysr-c.pri)
exists($$ROOT_DIR/../preferred/compiler.pri): include($$ROOT_DIR/../preferred/compiler.pri)



INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR $$SRC_ROOT_DIR

INCLUDEPATH += $$SRC_GROUP_DIR/qh-cpp

INCLUDEPATH += $$RELAE_GRAPH_SRC_GROUP_DIR
INCLUDEPATH += $$PHAON_GRAPH_SRC_GROUP_DIR



CONFIG += no_keywords

DEFINES += USE_KANS

DEFINES += ROOT_FOLDER=\\\"$$ROOT_DIR\\\"


HEADERS += \
#  $$SRC_DIR/pdf-viewer/pdf-document-controller.h \
#  $$SRC_DIR/pdf-viewer/pdf-document-info.h \
#  $$SRC_DIR/pdf-viewer/pdf-document-state.h \
#  $$SRC_DIR/json/qh-json-file-reader.h \
#  $$SRC_DIR/json/pseudo-jpath.h \
#  $$SRC_DIR/sdi/sdi-block-element.h \
#  $$SRC_DIR/sdi/sdi-block-element-collection.h \





SOURCES += \
  $$SRC_DIR/main.cpp \



# $$SRC_DIR/pdf-viewer/pdf-document-controller.cpp \
#  $$SRC_DIR/pdf-viewer/pdf-document-info.cpp \
#  $$SRC_DIR/pdf-viewer/pdf-document-state.cpp \
#  $$SRC_DIR/json/qh-json-file-reader.cpp \
#  $$SRC_DIR/json/pseudo-jpath.cpp \
#  $$SRC_DIR/sdi/sdi-block-element.cpp \
#  $$SRC_DIR/sdi/sdi-block-element-collection.cpp \



#LIBS += -L$$TARGETSDIR -lqh-cpp   -lqh-cpp-parser
#LIBS += -L$$POPPLER_LIB_DIR  -lpoppler -lpoppler-qt5
#LIBS += -rdynamic -ldl




message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
