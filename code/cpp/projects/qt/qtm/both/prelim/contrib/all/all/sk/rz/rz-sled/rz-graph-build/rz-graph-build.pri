
#           Copyright Nathaniel Christen 2019.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)

PROJECT_NAME = rz-graph-build

include(../build-group.pri)

DEFINES += USE_RZNS


DEFINES += CAON_DEBUG
DEFINES += RELAE_LABEL_NODES

INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR $$SRC_GROUP_DIR/rz-graph-token \
 $$SRC_ROOT_DIR  $$SRC_GROUP_DIR/rz-graph-core


INCLUDEPATH += $$HGDM_SRC_GROUP_DIR

INCLUDEPATH += $$RELAE_GRAPH_SRC_GROUP_DIR


DEFINES += RELAE_LABEL_NODES
DEFINES += CAON_DEBUG


LIBS += -L$$TARGETSDIR -lrz-graph-token -lrz-graph-core


HEADERS += \
  $$SRC_DIR/rz-asg-document.h \
  $$SRC_DIR/rz-asg-result-holder.h \
  $$SRC_DIR/types/core-types.h \
  $$SRC_DIR/types/run-type-codes.h \
  $$SRC_DIR/types/rz-asg-empty-tuple.h \


SOURCES += \
    $$SRC_DIR/rz-asg-document.cpp \
    $$SRC_DIR/rz-asg-result-holder.cpp \

message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)