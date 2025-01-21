

#           Copyright Nathaniel Christen 2020.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)


QT += gui  widgets

include(../build-group.pri)

DEFINES += USE_XCNS


INCLUDEPATH += $$XCSD_DATA_SRC_GROUP_DIR

INCLUDEPATH += $$DHAX_SRC_GROUP_DIR $$DHAX_SRC_GROUP_DIR/dhax-gui


HEADERS += \
  $$SRC_DIR/xcsd-outer-ring-scene-item.h \
  $$SRC_DIR/xcsd-tierbox-scene-item.h \


SOURCES += \
  $$SRC_DIR/xcsd-outer-ring-scene-item.cpp \
  $$SRC_DIR/xcsd-tierbox-scene-item.cpp \



#LIBS += -L$$TARGETSDIR -lxcsd-1d


message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)

