#include "mainwindow.h"
#include "soundplayer.h"

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>

#include <QApplication>
#include <QUrl>
#include <QFile>
#include <QResource>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
