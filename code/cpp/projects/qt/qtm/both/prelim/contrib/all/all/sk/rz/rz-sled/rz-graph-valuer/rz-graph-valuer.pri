
#           Copyright Nathaniel Christen 2019.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)

PROJECT_NAME = rz-graph-valuer

include(../build-group.pri)

DEFINES += USE_RZNS


DEFINES += CAON_DEBUG
DEFINES += RELAE_LABEL_NODES


INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR $$SRC_GROUP_DIR/rz-graph-build \
  $$SRC_GROUP_DIR/rz-clasp-code  $$SRC_GROUP_DIR/rz-graph-token  $$SRC_GROUP_DIR/rz-graph-core


message($$SRC_GROUP_DIR/rz-graph-visit)



INCLUDEPATH += $$HGDM_SRC_GROUP_DIR

INCLUDEPATH += $$RELAE_GRAPH_SRC_GROUP_DIR
INCLUDEPATH += $$PHR_GRAPH_SRC_GROUP_DIR


LIBS += -L$$TARGETSDIR -lrz-graph-token -lrz-graph-build \
 -lrz-graph-embed -lrz-graph-core -lrz-function-def -lrz-code-elements


HEADERS += \
   $$SRC_DIR/valuer/rz-asg-valuer.h \
   $$SRC_DIR/valuer/rz-asg-symbol.h \
   $$SRC_DIR/scope/rz-asg-scope-token.h \
   $$SRC_DIR/scope/rz-asg-lexical-scope.h \
   $$SRC_DIR/scope/rz-asg-logical-scope.h \
   $$SRC_DIR/scope/rz-asg-block-info.h \
   $$SRC_DIR/scope/rz-phaon-user-type.h \
   $$SRC_DIR/scope/rz-phaon-precycle.h \
   $$SRC_DIR/rz-asg-rvalue.h \
   $$SRC_DIR/valuer/rz-opaque-call.h \
   $$SRC_DIR/valuer/rz-opaque-type-symbol.h \
   $$SRC_DIR/vector/rz-asg-vector.h \
   $$SRC_DIR/vector/rz-asg-map.h \
   $$SRC_DIR/vector/rz-string-plex.h \
   $$SRC_DIR/string/rz-string.h \
   $$SRC_DIR/string/rz-keyword.h \
   $$SRC_DIR/string/rz-match-literal.h \


SOURCES += \
   $$SRC_DIR/valuer/rz-asg-valuer.cpp \
   $$SRC_DIR/valuer/rz-asg-symbol.cpp \
   $$SRC_DIR/scope/rz-asg-scope-token.cpp \
   $$SRC_DIR/scope/rz-asg-lexical-scope.cpp \
   $$SRC_DIR/scope/rz-asg-logical-scope.cpp \
   $$SRC_DIR/scope/rz-asg-block-info.cpp \
   $$SRC_DIR/scope/rz-phaon-user-type.cpp \
   $$SRC_DIR/scope/rz-phaon-precycle.cpp \
   $$SRC_DIR/rz-asg-rvalue.cpp \
   $$SRC_DIR/valuer/rz-opaque-call.cpp \
   $$SRC_DIR/valuer/rz-opaque-type-symbol.cpp \
   $$SRC_DIR/vector/rz-asg-vector.cpp \
   $$SRC_DIR/vector/rz-asg-map.cpp \
   $$SRC_DIR/vector/rz-string-plex.cpp \
   $$SRC_DIR/string/rz-string.cpp \
   $$SRC_DIR/string/rz-keyword.cpp \
   $$SRC_DIR/string/rz-match-literal.cpp \



message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
