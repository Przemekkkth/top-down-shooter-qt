greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    src/fontmanager.cpp \
    src/gamescene.cpp \
    src/main.cpp \
    src/objects/bullet.cpp \
    src/objects/player.cpp \
    src/objects/zombie.cpp \
    src/pixmapmanager.cpp \
    src/view.cpp

HEADERS += \
    src/fontmanager.h \
    src/gamescene.h \
    src/objects/bullet.h \
    src/objects/player.h \
    src/objects/zombie.h \
    src/pixmapmanager.h \
    src/resourceholder.h \
    src/utils.h \
    src/view.h

RESOURCES += \
    resource.qrc
