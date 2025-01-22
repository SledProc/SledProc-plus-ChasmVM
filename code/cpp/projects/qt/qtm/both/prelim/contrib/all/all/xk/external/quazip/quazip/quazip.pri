

#           Copyright Nathaniel Christen 2019.
#  Distributed under the Boost Software License, Version 1.0.
#     (See accompanying file LICENSE_1_0.txt or copy at
#           http://www.boost.org/LICENSE_1_0.txt)

include(../build-group.pri)

QT -= gui

include($$ROOT_DIR/../preferred/sysr.pri)


INCLUDEPATH += $$SRC_DIR

#INCLUDEPATH += $$SRC_GROUP_DIR

HEADERS += \
  $$SRC_DIR/minizip_crypt.h \
  $$SRC_DIR/ioapi.h \
  $$SRC_DIR/JlCompress.h \
  $$SRC_DIR/quaadler32.h \
  $$SRC_DIR/quachecksum32.h \
  $$SRC_DIR/quacrc32.h \
  $$SRC_DIR/quagzipfile.h \
  $$SRC_DIR/quaziodevice.h \
  $$SRC_DIR/quazipdir.h \
  $$SRC_DIR/quazipfile.h \
  $$SRC_DIR/quazipfileinfo.h \
  $$SRC_DIR/quazip_global.h \
  $$SRC_DIR/quazip.h \
  $$SRC_DIR/quazipnewinfo.h \
  $$SRC_DIR/unzip.h \
  $$SRC_DIR/zip.h


SOURCES += \
  $$SRC_DIR/qioapi.cpp \
  $$SRC_DIR/JlCompress.cpp \
  $$SRC_DIR/quaadler32.cpp \
  $$SRC_DIR/quacrc32.cpp \
  $$SRC_DIR/quagzipfile.cpp \
  $$SRC_DIR/quaziodevice.cpp \
  $$SRC_DIR/quazip.cpp \
  $$SRC_DIR/quazipdir.cpp \
  $$SRC_DIR/quazipfile.cpp \
  $$SRC_DIR/quazipfileinfo.cpp \
  $$SRC_DIR/quazipnewinfo.cpp \


SOURCES += \
  $$SRC_DIR/unzip.c \
  $$SRC_DIR/zip.c


message(choice: $$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
mkpath($$CPP_ROOT_DIR/targets/$$CHOICE_CODE/$$PROJECT_SET--$$PROJECT_GROUP--$$PROJECT_NAME)
