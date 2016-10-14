#-------------------------------------------------
#
# Project created by QtCreator 2011-04-23T18:56:38
#
#-------------------------------------------------

QT       += core gui

TARGET = MyClient_System_Editing
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    encrypt/crypt.cpp \
    locale/localegame.cpp \
    encrypt/encrypt.cpp \
    locale/item_proto.cpp \
    manager/itemprotomanager.cpp \
    common/tga.cpp \
    common/syslog.cpp \
    common/IO.cpp \
    tabs/items.cpp \
    tabs/server_attr.cpp \
    tabs/fishing.cpp \
    tabs/group.cpp \
    tabs/mobdropitem.cpp \
    manager/common.cpp \
    manager/newitems.cpp \
    tabs/MDATR.cpp \
    tabs/special_item_group.cpp

HEADERS  += mainwindow.h \
    encrypt/crypt.h \
    locale/localegame.h \
    encrypt/encrypt.h \
    locale/item_proto.h \
    manager/itemprotomanager.h \
    common/tga.h \
    common/syslog.h \
    common/IO.h \
    tabs/items.h \
    tabs/server_attr.h \
    tabs/fishing.h \
    tabs/group.h \
    tabs/mobdropitem.h \
    manager/common.h \
    global.h \
    manager/newitems.h \
    tabs/MDATR.h \
    tabs/special_item_group.h

FORMS    += mainwindow.ui

LIBS     += $$_PRO_FILE_PWD_\encrypt\lzo.lib

RESOURCES += \
    resources.qrc

OTHER_FILES +=
	win32:RC_FILE = myapp.rc

QMAKE_LFLAGS_RELEASE = /NODEFAULTLIB:"MSVCRT"


