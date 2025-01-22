
#           Copyright Nathaniel Christen 2019.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)



include(../build-group.pri)

#TEMPLATE = app

CONFIG += console

DEFINES += CAON_DEBUG
DEFINES += RELAE_LABEL_NODES

DEFINES += RZ_DIR=\\\"$$ROOT_DIR/../dev/scripts/rz\\\"

DEFINES += RZ_KPH_DIR=\\\"$$ROOT_DIR/../dev/scripts/rz-kph\\\"

DEFINES += DEFAULT_KPH_FOLDER=\\\"$$ROOT_DIR/../dev/kph\\\"

DEFINES += AR_ROOT_DIR=\\\"$$ROOT_DIR\\\"

INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR \


INCLUDEPATH += $$RELAE_GRAPH_SRC_GROUP_DIR
INCLUDEPATH += $$PHR_GRAPH_SRC_GROUP_DIR
INCLUDEPATH += $$RZ_KAUVIR_SRC_GROUP_DIR
INCLUDEPATH += $$PHAONIR_SRC_GROUP_DIR
INCLUDEPATH += $$PHAONLIB_SRC_GROUP_DIR
INCLUDEPATH += $$PHR_RUNTIME_SRC_GROUP_DIR

INCLUDEPATH += $$TESTS_PHR_SRC_GROUP_DIR


CONFIG += no_keywords

DEFINES += USE_RZNS

DEFINES += USE_KANS


DEFINES += DEFAULT_PTR_BYTE_CODE=QT_POINTER_SIZE


message(T: $$TARGETSDIR)



HEADERS += \
  $$SRC_DIR/test-ffi.h \


SOURCES += \
  $$SRC_DIR/test-ffi.cpp \


message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
