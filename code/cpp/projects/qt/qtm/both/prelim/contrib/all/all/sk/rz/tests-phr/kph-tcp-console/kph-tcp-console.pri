
#           Copyright Nathaniel Christen 2019.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)

PROJECT_NAME = kph-tcp-console

include(../build-group.pri)

QT += network

#

TEMPLATE = app

DEFINES += DEFAULT_KPH_FOLDER=\\\"$$ROOT_DIR/dev/kph\\\"


INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR

INCLUDEPATH += $$PHAONIR_SRC_GROUP_DIR
INCLUDEPATH += $$PHAONLIB_SRC_GROUP_DIR

INCLUDEPATH += $$PHR_RUNTIME_SRC_GROUP_DIR
INCLUDEPATH += $$RELAE_GRAPH_SRC_GROUP_DIR


CONFIG += no_keywords


DEFINES += USE_KANS

DEFINES += DEFAULT_PTR_BYTE_CODE=QT_POINTER_SIZE

SOURCES += \
  $$SRC_DIR/main.cpp \
  $$SRC_ROOT_DIR/default-phr-startup.cpp \


LIBS += -L$$TARGETSDIR -lphaon-ir


CHOICE_FEATURES += kph


contains(CHOICE_FEATURES, "kph") \#/
{
 message(DEFINE\'ing USING_KPH)
 DEFINES += USING_KPH
}


message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
