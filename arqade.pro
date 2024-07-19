QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += debug_and_release

SOURCES += \
    main.cpp \
    Arqade.cpp \
    ArqadeWindow.cpp \
    AudioPlayer.cpp \
    Core.cpp \
    CoreData.cpp \
    Database.cpp \
    KeybindWindow.cpp \
    SystemTab.cpp \
    Utils.cpp \
    VideoPlayer.cpp

HEADERS += \
    Arqade.hpp \
    ArqadeWindow.hpp \
    AudioPlayer.hpp \
    ControllerData.hpp \
    Core.hpp \
    CoreData.hpp \
    Database.hpp \
    libretro.hpp \
    KeybindWindow.hpp \
    SystemTab.hpp \
    Utils.hpp \
    VideoPlayer.hpp \

FORMS += \
    ArqadeWindow.ui \
    KeybindWindow.ui \
    SystemTab.ui

INCLUDEPATH += /usr/include/SDL2

LIBS += -L/usr/lib/libSDL2.so -lSDL2

release: DESTDIR = bin/release
debug: DESTDIR = bin/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui
