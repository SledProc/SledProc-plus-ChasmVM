
#           Copyright Nathaniel Christen 2019.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)

PROJECT_NAME = rz-ngml

include(../build-group.pri)


INCLUDEPATH += $$SRC_DIR $$SRC_GROUP_DIR $$SRC_ROOT_DIR

INCLUDEPATH += $$WCM_SRC_GROUP_DIR
INCLUDEPATH += $$NTXH_SRC_GROUP_DIR
INCLUDEPATH += $$NTXH_SRC_GROUP_DIR/ntxh
INCLUDEPATH += $$PHAON_GRAPH_SRC_GROUP_DIR
INCLUDEPATH += $$RELAE_GRAPH_SRC_GROUP_DIR

INCLUDEPATH += $$HTXN_SRC_GROUP_DIR


DEFINES += DEFAULT_WCM_FOLDER=\\\"$$DATA_ROOT_DIR/wcm\\\"

DEFINES += DEFAULT_NTXH_FOLDER=\\\"$$DATA_ROOT_DIR/ntxh\\\"

DEFINES += QT_REPROZIP_BIN_FOLDER=\\\"$$QT_REPROZIP_BIN_DIR\\\"

#DEFINES += CLO43SD_ROOT_FOLDER=\\\"$$ROOT_DIR/../external/data/clo43sd/CLO-43SD\\\"

DEFINES += ROOT_FOLDER=\\\"$$ROOT_DIR\\\"

DEFINES += CAON_DEBUG
DEFINES += RELAE_LABEL_NODES

CONFIG += no_keywords


DEFINES += USE_KANS
DEFINES += USE_RZNS


HEADERS += \
  $$SRC_DIR/kernel/rz-ngml-dominion.h \
  $$SRC_DIR/kernel/rz-ngml-root.h \
  $$SRC_DIR/kernel/document/rz-ngml-document.h \
  $$SRC_DIR/kernel/document/rz-ngml-document-info.h \
  $$SRC_DIR/kernel/document/rz-ngml-document-light-xml.h \
  $$SRC_DIR/kernel/document/rz-ngml-folder.h \
  $$SRC_DIR/kernel/dominion/connectors.h \
  $$SRC_DIR/kernel/dominion/types.h \
  $$SRC_DIR/kernel/frame/rz-ngml-frame.h \
  $$SRC_DIR/kernel/grammar/rz-ngml-grammar.h \
  $$SRC_DIR/kernel/grammar/rz-ngml-graph-build.h \
  $$SRC_DIR/kernel/grammar/rz-ngml-parser.h \
  $$SRC_DIR/kernel/grammar/rz-ngml-parsing-mode.h \
  $$SRC_DIR/kernel/grammar/rz-ngml-parse-context.h \
  $$SRC_DIR/kernel/graph/rz-ngml-graph.h \
  $$SRC_DIR/kernel/graph/rz-ngml-node.h \
  $$SRC_DIR/kernel/graph/rz-ngml-markup-position.h \
  $$SRC_DIR/kernel/query/rz-ngml-query.h \
  $$SRC_DIR/kernel/output/rz-ngml-connection-descriptor.h \
  $$SRC_DIR/kernel/output/rz-ngml-output-bundle.h \
  $$SRC_DIR/tile/rz-ngml-token.h \
  $$SRC_DIR/tile/rz-ngml-tile.h \
  $$SRC_DIR/tile/rz-ngml-attribute-tile.h \
  $$SRC_DIR/tile/rz-ngml-paralex-tile.h \
  $$SRC_DIR/annotation/rz-ngml-annotation-tile.h \
  $$SRC_DIR/annotation/rz-ngml-annotation-filter.h \
  $$SRC_DIR/tag-command/rz-ngml-tag-command.h \
  $$SRC_DIR/tag-command/rz-ngml-command-callback.h \
  $$SRC_DIR/output/rz-ngml-output-base.h \
  $$SRC_DIR/output/rz-ngml-output-event-handler.h \
  $$SRC_DIR/output/rz-ngml-output-event-generator.h \
  $$SRC_DIR/output/rz-ngml-output-latex.h \
  $$SRC_DIR/output/rz-ngml-output-callbacks-common.h \
  $$SRC_DIR/output/rz-ngml-output-html.callbacks.h \
  $$SRC_DIR/output/rz-ngml-output-latex.callbacks.h \
  $$SRC_DIR/output/rz-ngml-output-xml.callbacks.h \
  $$SRC_DIR/output/rz-ngml-output-html.h \
  $$SRC_DIR/output/rz-ngml-output-xml.h \
  $$SRC_DIR/output/rz-ngml-output-htxn.h \
  $$SRC_DIR/output/rz-ngml-output-htxn.callbacks.h \
  $$SRC_DIR/output/rz-ngml-output-infoset.h \
  $$SRC_DIR/output/rz-ngml-output-infoset.callbacks.h \
  $$SRC_DIR/whitespace/rz-ngml-whitespace.h \
  $$SRC_DIR/whitespace/rz-ngml-whitespace-holder.h \
  $$SRC_DIR/whitespace/rz-ngml-whitespace-holder.h \
  $$SRC_DIR/ngml-htxn/ngml-htxn-node.h \


SOURCES += \
  $$SRC_DIR/kernel/rz-ngml-dominion.cpp \
  $$SRC_DIR/kernel/document/rz-ngml-document.cpp \
  $$SRC_DIR/kernel/document/rz-ngml-document-info.cpp \
  $$SRC_DIR/kernel/document/rz-ngml-document-light-xml.cpp \
  $$SRC_DIR/kernel/document/rz-ngml-folder.cpp \
  $$SRC_DIR/kernel/frame/rz-ngml-frame.cpp \
  $$SRC_DIR/kernel/grammar/rz-ngml-grammar.cpp \
  $$SRC_DIR/kernel/grammar/rz-ngml-graph-build.cpp \
  $$SRC_DIR/kernel/grammar/rz-ngml-parser.cpp \
  $$SRC_DIR/kernel/grammar/rz-ngml-parse-context.cpp \
  $$SRC_DIR/kernel/graph/rz-ngml-graph.cpp \
  $$SRC_DIR/kernel/graph/rz-ngml-node.cpp \
  $$SRC_DIR/kernel/graph/rz-ngml-markup-position.cpp \
  $$SRC_DIR/kernel/query/rz-ngml-query.cpp \
  $$SRC_DIR/kernel/output/rz-ngml-connection-descriptor.cpp \
  $$SRC_DIR/kernel/output/rz-ngml-output-bundle.cpp \
  $$SRC_DIR/tile/rz-ngml-token.cpp \
  $$SRC_DIR/tile/rz-ngml-tile.cpp \
  $$SRC_DIR/tile/rz-ngml-attribute-tile.cpp \
  $$SRC_DIR/tile/rz-ngml-paralex-tile.cpp \
  $$SRC_DIR/annotation/rz-ngml-annotation-tile.cpp \
  $$SRC_DIR/annotation/rz-ngml-annotation-filter.cpp \
  $$SRC_DIR/tag-command/rz-ngml-tag-command.cpp \
  $$SRC_DIR/tag-command/rz-ngml-command-callback.cpp \
  $$SRC_DIR/output/rz-ngml-output-base.cpp \
  $$SRC_DIR/output/rz-ngml-output-event-handler.cpp \
  $$SRC_DIR/output/rz-ngml-output-event-generator.cpp \
  $$SRC_DIR/output/rz-ngml-output-html.cpp \
  $$SRC_DIR/output/rz-ngml-output-xml.cpp \
  $$SRC_DIR/output/rz-ngml-output-latex.cpp \
  $$SRC_DIR/output/rz-ngml-output-htxn.cpp \
  $$SRC_DIR/output/rz-ngml-output-infoset.cpp \
  $$SRC_DIR/whitespace/rz-ngml-whitespace.cpp \
  $$SRC_DIR/whitespace/rz-ngml-whitespace-holder.cpp \
  $$SRC_DIR/ngml-htxn/ngml-htxn-node.cpp \


message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)

