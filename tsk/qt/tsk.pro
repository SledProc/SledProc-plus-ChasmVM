

QT += widgets gui webenginewidgets webchannel

INCLUDEPATH += \
  ../utils ../game


HEADERS += \
  ../game/game-board.h \
  ../game/game-driver.h \
  ../game/game-player.h \
  ../game/game-position.h \
  ../game/game-token.h \
  ../game/game-emblem.h \
  ../game/message-display-window.h \
  ../game/token-group.h \
  ../game/variants/game-variant.h \
  ../game/variants/au/au-game-variant.h \
  ../utils/rdsc-qh/JsInterface.h \
  ../utils/rdsc-qh/qh-web-engine-page.h \
  ../utils/rdsc-qh/qh-web-engine-view.h \
  ../utils/rdsc-qh/qh-web-page.h \
  ../utils/rdsc-qh/qh-web-view-dialog.h \



SOURCES += \
  ../main.cpp \
  ../game/game-board.cpp \
  ../game/game-driver.cpp \
  ../game/game-player.cpp \
  ../game/game-position.cpp \
  ../game/message-display-window.cpp \
  ../game/game-token.cpp \
  ../game/game-emblem.cpp \
  ../game/token-group.cpp \
  ../game/variants/game-variant.cpp \
  ../game/variants/au/au-game-variant.cpp \
  ../utils/rdsc-qh/qh-web-engine-page.cpp \
  ../utils/rdsc-qh/qh-web-engine-view.cpp \
  ../utils/rdsc-qh/qh-web-page.cpp \
  ../utils/rdsc-qh/qh-web-view-dialog.cpp \
  ../game/variants/au/move-options/au.bishop.cpp \
  ../game/variants/au/move-options/au.check-move-options.cpp \
  ../game/variants/au/move-options/au.jqk.cpp \
  ../game/variants/au/move-options/au.knight.cpp \
  ../game/variants/au/move-options/au.rook.cpp \


DEFINES += SCREENSHOTS_FOLDER=\\\".\\\"
