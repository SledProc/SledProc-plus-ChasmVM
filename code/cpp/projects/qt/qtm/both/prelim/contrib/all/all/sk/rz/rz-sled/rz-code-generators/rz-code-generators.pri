
#           Copyright Nathaniel Christen 2019.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)

PROJECT_NAME = rz-code-generators

include(../build-group.pri)

DEFINES += USE_RZNS


INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR \
  $$SRC_GROUP_DIR/rz-graph-token \
  $$SRC_GROUP_DIR/rz-graph-core \


INCLUDEPATH += $$HGDM_SRC_GROUP_DIR

INCLUDEPATH += $$RELAE_GRAPH_SRC_GROUP_DIR
INCLUDEPATH += $$PHR_GRAPH_SRC_GROUP_DIR

DEFINES += CAON_DEBUG
DEFINES += RELAE_LABEL_NODES
DEFINES += RZ_CPP_GEN


TEMPLATE = lib



HEADERS += \
  $$SRC_ROOT_DIR/multistep-token.h \
  $$SRC_DIR/rpi/rpi-output.h \
  $$SRC_DIR/rpi/rpi-block.h \
  $$SRC_DIR/rpi/rpi-stage-form.h \
  $$SRC_DIR/rpi/rpi-stage-element.h \
  $$SRC_DIR/rpi/rpi-assignment-info.h \
  $$SRC_DIR/rpi/rpi-expression.h \
  $$SRC_DIR/rpi/rpi-type-declaration.h \

SOURCES += \
  $$SRC_DIR/rpi/rpi-output.cpp \
  $$SRC_DIR/rpi/rpi-block.cpp \
  $$SRC_DIR/rpi/rpi-stage-form.cpp \
  $$SRC_DIR/rpi/rpi-stage-element.cpp \
  $$SRC_DIR/rpi/rpi-assignment-info.cpp \
  $$SRC_DIR/rpi/rpi-expression.cpp \
  $$SRC_DIR/rpi/rpi-type-declaration.cpp \

LIBS += -L$$TARGETSDIR -lrz-graph-core -lrz-graph-token -lrz-graph-visit \
  -lrz-graph-valuer -lrz-function-def -lrz-code-elements


message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)

