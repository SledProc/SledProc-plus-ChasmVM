
#           Copyright Nathaniel Christen 2019.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)

PROJECT_NAME = rz-graph-code

include(../build-group.pri)

DEFINES += USE_RZNS


INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR  \
  $$SRC_GROUP_DIR/rz-graph-token  $$SRC_GROUP_DIR/rz-graph-core \


INCLUDEPATH += $$HGDM_SRC_GROUP_DIR

INCLUDEPATH += $$RELAE_GRAPH_SRC_GROUP_DIR


LIBS += -L$$TARGETSDIR -lrz-graph-visit -lrz-graph-sre  \
 -lrz-graph-core -lrz-graph-token -lrz-graph-run -lrz-graph-valuer  \
 -lrz-code-generators  -lrz-function-def

DEFINES += CAON_DEBUG
DEFINES += RELAE_LABEL_NODES


DEFINES += RELAE_LABEL_NODES
DEFINES += CAON_DEBUG


HEADERS += \
  $$SRC_DIR/prerun/rz-prerun-anticipate.h \
  $$SRC_DIR/prerun/rz-prerun-normalize.h \
  $$SRC_DIR/prerun/rz-prerun-tokens.h \


SOURCES += \
  $$SRC_DIR/prerun/rz-prerun-anticipate.cpp \
  $$SRC_DIR/prerun/rz-prerun-normalize.cpp \
  $$SRC_DIR/prerun/rz-prerun-tokens.cpp \

message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)




