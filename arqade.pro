QT += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += debug_and_release

SOURCES += \
    main.cpp \
    Arqade.cpp \
    AudioPlayer.cpp \
    Core.cpp \
    CoreData.cpp \
    Database.cpp \
    SystemTab.cpp \
    Utils.cpp \
    VideoPlayer.cpp \
    Window.cpp

HEADERS += \
    Arqade.hpp \
    AudioPlayer.hpp \
    Core.hpp \
    CoreData.hpp \
    Database.hpp \
    libretro.hpp \
    SystemTab.hpp \
    Utils.hpp \
    VideoPlayer.hpp \
    Window.hpp

FORMS += \
    SystemTab.ui \
    Window.ui

INCLUDEPATH += /usr/include/SDL2

LIBS += -L/usr/lib/libSDL2.so -lSDL2

release: DESTDIR = bin/release
debug: DESTDIR = bin/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui
