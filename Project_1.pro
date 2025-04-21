QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bag.cpp \
    battlescene.cpp \
    grassland.cpp \
    laboratory.cpp \
    main.cpp \
    mainwindow.cpp \
    player.cpp \
    titlescreen.cpp \
    town.cpp

HEADERS += \
    bag.h \
    battlescene.h \
    grassland.h \
    laboratory.h \
    mainwindow.h \
    player.h \
    titlescreen.h \
    town.h

FORMS += \
    mainwindow.ui \
    player.ui \
    titlescreen.ui

TRANSLATIONS += \
    Project_1_zh_TW.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    data.qrc
