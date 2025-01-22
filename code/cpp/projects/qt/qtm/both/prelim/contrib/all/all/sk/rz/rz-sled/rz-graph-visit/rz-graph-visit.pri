
#           Copyright Nathaniel Christen 2019.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)

PROJECT_NAME = rz-graph-visit

include(../build-group.pri)

DEFINES += USE_RZNS


DEFINES += CAON_DEBUG
DEFINES += RELAE_LABEL_NODES


INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR $$SRC_GROUP_DIR/rz-graph-core \
  $$SRC_GROUP_DIR/rz-graph-token $$SRC_GROUP_DIR/rz-graph-run \
  $$SRC_GROUP_DIR/rz-graph-build  \


INCLUDEPATH += $$HGDM_SRC_GROUP_DIR

INCLUDEPATH += $$RELAE_GRAPH_SRC_GROUP_DIR
INCLUDEPATH += $$PHR_GRAPH_SRC_GROUP_DIR


LIBS += -L$$TARGETSDIR -lrz-graph-build -lrz-graph-valuer -lrz-graph-core  \
 -lrz-graph-token -lrz-graph-run -lrz-graph-embed \
 -lrz-graph-embed-run



HEADERS += \
    $$SRC_DIR/rz-asg-visitor.core-function-list.h \
    $$SRC_DIR/rz-asg-visitor.h \
    $$SRC_DIR/rz-asg-visitor-dynamo.h \
    $$SRC_DIR/rz-asg-visitor-run-state.h \
    $$SRC_DIR/rz-asg-visitor-run-plugin.h \
    $$SRC_DIR/rz-block-entry-run-plugin.h \
    $$SRC_DIR/rz-embed-branch-run-plugin.h \
    $$SRC_DIR/rz-graph-visitor-phaon.h \

SOURCES += \
    $$SRC_DIR/rz-asg-visitor.cpp \
    $$SRC_DIR/rz-asg-visitor-dynamo.cpp \
    $$SRC_DIR/rz-asg-visitor-run-state.cpp \
    $$SRC_DIR/rz-asg-visitor-run-plugin.cpp \
    $$SRC_DIR/rz-block-entry-run-plugin.cpp \
    $$SRC_DIR/rz-embed-branch-run-plugin.cpp \
    $$SRC_DIR/rz-graph-visitor-phaon.cpp \


LIBS += -L$$TARGETSDIR -lrz-code-elements -lrz-graph-valuer

message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
