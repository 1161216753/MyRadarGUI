QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++13
TARGET = MyRadarGUI
TEMPLATE = app

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    package/shape/dbfopen.c \
    package/shape/safileio.c \
    package/shape/shpopen.c \
    src/map/china_loader.cpp \
    src/map/dbf.cpp \
    src/map/loader.cpp \
    src/map/shp.cpp \
    src/radarGUI/main.cpp \
    src/radarGUI/mainwindow.cpp

HEADERS += \
    include/map/adj_entry.h \
    include/map/base_objects.h \
    include/map/china_loader.h \
    include/map/dbf.h \
    include/map/loader.h \
    include/map/shp.h \
    include/radarGUI/mainwindow.h \
    package/shape/shapefil.h

FORMS += \
    mainwindow.ui


INCLUDEPATH += \
    $$PWD/package \
    $$PWD/include/map \
    $$PWD/include/radarGUI \
    $$PWD/include/setParameter \

win32-msvc* {
    QMAKE_CXXFLAGS += /wd4996
    QMAKE_CFLAGS += /wd4996
}

QMAKE_PROJECT_DEPTH = 0
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

