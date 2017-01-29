#-------------------------------------------------
# LIBS += -I/usr/local/include/SDL -D_GNU_SOURCE=1 -D_THREAD_SAFE -L/usr/local/lib -lSDLmain -lSDL -Wl,-framework,Cocoa
# Project created by QtCreator 2016-11-18T16:31:37
#
#LIBS +=   -I/usr/local/include/SDL -D_GNU_SOURCE=1 -D_THREAD_SAFE \
#          -L/usr/local/lib/     \
#          -lSDLmain -lSDL -lSDL_mixer -Wl
#
#-------------------------------------------------

QT       += core gui

QT += widgets

TARGET = spotimac
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    soundplayer.cpp \
    music.cpp \
    dialog.cpp

HEADERS  += mainwindow.h \
    soundplayer.h \
    music.h \
    dialog.h

INCLUDEPATH += /usr/local/include

LIBS +=   -I/Library/Frameworks/SDL.framework/Headers -I/Library/Frameworks/SDL_image.framework/Headers -I/Library/Frameworks/SDL_mixer.framework/Headers -I/opt/local/include -L/usr/local/lib -lSDLmain -lSDL -Wl,-framework,Cocoa -framework Cocoa -framework OpenGL -lSDL_image -lSDL_mixer -framework GLUT -framework OpenGL, -I/usr/local/include/taglib -ltag \
          -I/usr/local/include/SDL -D_GNU_SOURCE=1 -D_THREAD_SAFE -D OSX \


FORMS    += mainwindow.ui \
    dialog.ui

RESOURCES += \
    musics.qrc \

DISTFILES += \
    assets/grizzlybear.mp3
