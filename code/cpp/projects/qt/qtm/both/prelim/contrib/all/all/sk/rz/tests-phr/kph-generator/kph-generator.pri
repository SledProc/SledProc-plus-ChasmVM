
#           Copyright Nathaniel Christen 2019.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)

PROJECT_NAME = kph-generator

include(../build-group.pri)




INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR


INCLUDEPATH += $$HGDM_SRC_GROUP_DIR
INCLUDEPATH += $$PHAONLIB_SRC_GROUP_DIR
INCLUDEPATH += $$PHR_RUNTIME_SRC_GROUP_DIR
INCLUDEPATH += $$PHAONIR_SRC_GROUP_DIR

CONFIG += no_keywords


DEFINES += USE_KANS

HEADERS += \
  $$SRC_DIR/kph-generator.h \
  $$SRC_DIR/kph-generator-substitutions.h \


SOURCES += \
  $$SRC_DIR/kph-generator.cpp \
  $$SRC_DIR/kph-generator-substitutions.cpp \


LIBS += -L$$TARGETSDIR  -lphaon-ir


message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
