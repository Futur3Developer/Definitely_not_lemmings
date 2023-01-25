QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Lemmings/basherlemming.cpp \
    Lemmings/blockerlemming.cpp \
    Lemmings/bridgebuilderlemming.cpp \
    Lemmings/climberlemming.cpp \
    Lemmings/diggerlemming.cpp \
    Lemmings/joblesslemming.cpp \
    Lemmings/lemming.cpp \
    Lemmings/paratrooperlemming.cpp \
    Lemmings/rampbuilderlemming.cpp \
    MapFiles/MapComponents/block.cpp \
    MapFiles/MapComponents/entrance.cpp \
    MapFiles/MapComponents/exit.cpp \
    MapFiles/MapConversion/mapconversionmanager.cpp \
    MapFiles/MapConversion/mapconverter.cpp \
    MapFiles/MapConversion/mapxmlconverter.cpp \
    MapFiles/map.cpp \
    MapFiles/mapcreator.cpp \
    game.cpp \
    main.cpp  \
    mainwindow.cpp \
    score.cpp

HEADERS += \
    Lemmings/basherlemming.h \
    Lemmings/blockerlemming.h \
    Lemmings/bridgebuilderlemming.h \
    Lemmings/climberlemming.h \
    Lemmings/diggerlemming.h \
    Lemmings/joblesslemming.h \
    Lemmings/lemming.h \
    Lemmings/paratrooperlemming.h \
    Lemmings/rampbuilderlemming.h \
    MapFiles/MapComponents/block.h \
    MapFiles/MapComponents/entrance.h \
    MapFiles/MapComponents/exit.h \
    MapFiles/MapConversion/mapconversionmanager.h \
    MapFiles/MapConversion/mapconverter.h \
    MapFiles/MapConversion/mapxmlconverter.h \
    MapFiles/map.h \
    MapFiles/mapcreator.h \
    game.h \
    mainwindow.h \
    score.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    mainwindow.ui

RESOURCES += \
    graphics.qrc

DISTFILES += \
    Levels/level_1.xml \
    Levels/level_2.xml \
    Levels/level_3.xml \
    UserMaps/user_map_0.xml \
    UserMaps/user_map_1.xml \
    UserMaps/user_map_2.xml \
    UserMaps/user_map_3.xml \
    graphics/BlockerLemming.png \
    graphics/ClimberLemming.png \
    graphics/ParatropperLemming_parachute_closed.png \
    graphics/ParatropperLemming_parachute_open.png \
    graphics/lemming_right.png
