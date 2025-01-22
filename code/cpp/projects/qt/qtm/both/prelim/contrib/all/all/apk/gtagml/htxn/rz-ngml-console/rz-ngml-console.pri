
#           Copyright Nathaniel Christen 2019.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)

PROJECT_NAME = rz-ngml-console

include(../build-group.pri)

TEMPLATE = app

INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR $$SRC_ROOT_DIR
INCLUDEPATH += $$SRC_GROUP_DIR/rz-ngml

INCLUDEPATH += $$WCM_SRC_GROUP_DIR
INCLUDEPATH += $$NTXH_SRC_GROUP_DIR

INCLUDEPATH += $$HTXN_SRC_GROUP_DIR

INCLUDEPATH += $$NTXH_SRC_GROUP_DIR/ntxh
INCLUDEPATH += $$PHAON_GRAPH_SRC_GROUP_DIR
INCLUDEPATH += $$RELAE_GRAPH_SRC_GROUP_DIR


DEFINES += DEFAULT_WCM_FOLDER=\\\"$$DATA_ROOT_DIR/wcm\\\"

DEFINES += DEFAULT_NTXH_FOLDER=\\\"$$DATA_ROOT_DIR/ntxh\\\"

DEFINES += QT_REPROZIP_BIN_FOLDER=\\\"$$QT_REPROZIP_BIN_DIR\\\"

#DEFINES += CLO43SD_ROOT_FOLDER=\\\"$$ROOT_DIR/../external/data/clo43sd/CLO-43SD\\\"

DEFINES += ROOT_FOLDER=\\\"$$ROOT_DIR\\\"

DEFINES += CLO43SD_DB_CODE=\\\"222\\\"


CONFIG += no_keywords

DEFINES += CAON_DEBUG

DEFINES += USE_KANS
DEFINES += USE_RZNS
DEFINES += RELAE_LABEL_NODES


HEADERS += \


SOURCES += \
  $$SRC_DIR/main.cpp \


LIBS += -L$$TARGETSDIR -lrz-ngml -lhtxn


#?
#LIBS += -L$$TARGETSDIR -lwcm -lntxh  \
#  -lntxh-parser -lclo43sd-data -lposit-lib


message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)

