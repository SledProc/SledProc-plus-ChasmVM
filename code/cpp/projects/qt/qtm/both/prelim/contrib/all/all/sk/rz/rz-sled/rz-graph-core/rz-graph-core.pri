
#           Copyright Nathaniel Christen 2019.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)

PROJECT_NAME = rz-graph-core

include(../build-group.pri)

DEFINES += USE_RZNS


INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR

INCLUDEPATH += $$HGDM_SRC_GROUP_DIR

INCLUDEPATH += $$RELAE_GRAPH_SRC_GROUP_DIR

INCLUDEPATH += $$SRC_GROUP_DIR/rz-graph-token


DEFINES += RELAE_LABEL_NODES
DEFINES += CAON_DEBUG


HEADERS += \
  $$SRC_DIR/kernel/chasm-rz-dominion.h \
  $$SRC_DIR/kernel/chasm-rz-root.h \
  $$SRC_DIR/kernel/dominion/types.h \
  $$SRC_DIR/kernel/dominion/connectors.h \
  $$SRC_DIR/token/chasm-rz-token.h \
  $$SRC_DIR/kernel/graph/chasm-rz-node.h \
  $$SRC_DIR/kernel/graph/chasm-rz-node-proxy.h \
  $$SRC_DIR/kernel/graph/chasm-rz-graph.h \
  $$SRC_DIR/kernel/graph/chasm-rz-asg-position.h \
  $$SRC_DIR/kernel/graph/chasm-rz-connection.h \
  $$SRC_DIR/kernel/query/chasm-rz-query.h \
  $$SRC_DIR/kernel/frame/chasm-rz-frame.h \
  $$SRC_DIR/kernel/document/chasm-rz-document.h \
  $$SRC_DIR/kernel/grammar/chasm-rz-parser.h \
  $$SRC_DIR/kernel/grammar/chasm-rz-grammar.h \
  $$SRC_DIR/kernel/grammar/chasm-rz-graph-build.h \
  $$SRC_DIR/kernel/grammar/chasm-rz-parse-context.h \
  $$SRC_DIR/kernel/grammar/chasm-rz-string-plex-builder.h \
  $$SRC_DIR/code/chasm-rz-call-entry.h \
  $$SRC_DIR/code/chasm-rz-casement-call-entry.h \
  $$SRC_DIR/code/chasm-rz-block-entry.h \
  $$SRC_DIR/code/chasm-rz-casement-block-entry.h \
  $$SRC_DIR/code/chasm-rz-function-def-entry.h \
  $$SRC_DIR/code/chasm-rz-function-def-kinds.h \
  $$SRC_DIR/code/chasm-rz-code-representation.h \
  $$SRC_DIR/code/chasm-rz-code-representation.special-tokens.h \
  $$SRC_DIR/tuple/chasm-rz-tuple-info.h \
  $$SRC_DIR/output/chasm-rz-asg-output.h \
  $$SRC_DIR/output/chasm-rz-pre-init-asg.h \
  $$SRC_DIR/output/chasm-rz-pre-normal-asg.h \
  $$SRC_DIR/output/chasm-rz-pre-run-asg.h \

DEFINES += CAON_DEBUG
DEFINES += RELAE_LABEL_NODES



SOURCES += \
  $$SRC_DIR/kernel/chasm-rz-dominion.cpp \
  $$SRC_DIR/kernel/chasm-rz-root.cpp \
  $$SRC_DIR/token/chasm-rz-token.cpp \
  $$SRC_DIR/kernel/graph/chasm-rz-node.cpp \
  $$SRC_DIR/kernel/graph/chasm-rz-node-proxy.cpp \
  $$SRC_DIR/kernel/graph/chasm-rz-graph.cpp \
  $$SRC_DIR/kernel/graph/chasm-rz-asg-position.cpp \
  $$SRC_DIR/kernel/graph/chasm-rz-connection.cpp \
  $$SRC_DIR/kernel/query/chasm-rz-query.cpp \
  $$SRC_DIR/kernel/frame/chasm-rz-frame.cpp \
  $$SRC_DIR/kernel/document/chasm-rz-document.cpp \
  $$SRC_DIR/kernel/grammar/chasm-rz-parser.cpp \
  $$SRC_DIR/kernel/grammar/chasm-rz-grammar.cpp \
  $$SRC_DIR/kernel/grammar/chasm-rz-graph-build.cpp \
  $$SRC_DIR/kernel/grammar/chasm-rz-parse-context.cpp \
  $$SRC_DIR/kernel/grammar/chasm-rz-string-plex-builder.cpp \
  $$SRC_DIR/code/chasm-rz-code-representation.cpp \
  $$SRC_DIR/code/chasm-rz-call-entry.cpp \
  $$SRC_DIR/code/chasm-rz-casement-call-entry.cpp \
  $$SRC_DIR/code/chasm-rz-block-entry.cpp \
  $$SRC_DIR/code/chasm-rz-casement-block-entry.cpp \
  $$SRC_DIR/code/chasm-rz-function-def-entry.cpp \
  $$SRC_DIR/tuple/chasm-rz-tuple-info.cpp \
  $$SRC_DIR/output/chasm-rz-asg-output.cpp \
  $$SRC_DIR/output/chasm-rz-pre-init-asg.cpp \
  $$SRC_DIR/output/chasm-rz-pre-normal-asg.cpp \
  $$SRC_DIR/output/chasm-rz-pre-run-asg.cpp \

message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
