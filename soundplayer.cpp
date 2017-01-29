#include "soundplayer.h"
#include <iostream>
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

#include <taglib/taglib.h>
#include <taglib/tag.h>
#include <taglib/tstring.h>
#include <taglib/fileref.h>
#include <QDirIterator>
#include <QtDebug>
#include <fstream>

bool SoundPlayer::initd = false;
SoundPlayer * SoundPlayer::soundPlayer = 0;

using namespace TagLib;

SoundPlayer::SoundPlayer() {
    SDL_Init(SDL_INIT_AUDIO);
    Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
    Mix_VolumeMusic(100);
    this->setIsMusicLoaded(false);
    this->current = 0;
    this->isPlaylist = false;
}

SoundPlayer * SoundPlayer::startPlayer() {
    if(!initd) {
        soundPlayer = new SoundPlayer();
        initd = true;
    }
    return soundPlayer;
}

SoundPlayer::~SoundPlayer(){
    Mix_CloseAudio();
}

void SoundPlayer::loadMusic() {
    this->addMusic(this->getCurrentMusic()->getPathFile());
}

void SoundPlayer::playPlayer() {
    if(Mix_PausedMusic()) {
        Mix_ResumeMusic();
    } else if(!Mix_PlayingMusic()) {
        this->loadFile();
        this->setIsMusicLoaded(true);
        Mix_PlayMusic(this->getCurrentMusic()->getMusic(),1);
    } else {
        Mix_PauseMusic();
    }

}

bool SoundPlayer::getInitd() {
    return this->initd;
}

void SoundPlayer::pausePlayer() {
    Mix_PauseMusic();
}

Music* SoundPlayer::getCurrentMusic() {
    if(this->isPlaylist == true)
        return this->playlist.at(this->current);
    else
        return this->musics.at(this->current);
}

void SoundPlayer::stopPlayer() {
    Mix_FreeMusic( this->getCurrentMusic()->getMusic() );
    this->musics.clear();
    Mix_Quit();
}

bool SoundPlayer::getIsMusicLoaded() {
    return this->isMusicLoaded;
}

void SoundPlayer::setIsMusicLoaded(bool _isMusicLoaded) {
    this->isMusicLoaded = _isMusicLoaded;
}

void SoundPlayer::addMusic(std::string path) {
    this->musics.push_back(new Music(path));
}

void SoundPlayer::loadFile() {
    this->getCurrentMusic()->loadMusic();
}

void SoundPlayer::goThroughFolder() {
    QDir dir("./musics/");
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QFileInfoList list = dir.entryInfoList();
    QFileInfoList::iterator FileInfoIterator;

    // Iterator.
    for(FileInfoIterator = list.begin(); FileInfoIterator != list.end(); FileInfoIterator++){
        QFileInfo fileInfo = *FileInfoIterator;
        std::string filePath = fileInfo.absoluteFilePath().toUtf8().constData();
        this->addMusic(filePath);
    }
}

std::vector<Music *> SoundPlayer::getAllMusics() {
    return this->musics;
}

Music* SoundPlayer::getMusicById(int id) {
    if(this->isPlaylist == true)
        return this->playlist.at(id);
    else
        return this->musics.at(id);
}

Music* SoundPlayer::getMusicPlaylistById(int id) {
    return this->playlist.at(id);
}

int SoundPlayer::getCurrent() {
    return this->current;
}

void SoundPlayer::setCurrent(int _current) {
    this->current = _current;
}

std::vector<Music *> SoundPlayer::getAllPlaylist() {
    return this->playlist;
}

std::string SoundPlayer::getPathPlaylist() {
    return this->pathPlaylist;
}

void SoundPlayer::setPathPlaylist(std::string _path){
    this->pathPlaylist = _path;
}

void SoundPlayer::loadPlaylist() {
    std::ifstream input(this->pathPlaylist.c_str());
    if(!input)
        std::cout << "Pas d'ouverture de fichier" << std::endl;
    for( std::string line; getline( input, line ); ){
        if(QString(line.c_str()).startsWith('/'))
            this->playlist.push_back(new Music(line));
    }
}

bool SoundPlayer::getIsPlaylist() {
    return this->isPlaylist;
}

void SoundPlayer::setIsPlaylist(bool _isPlaylist) {
    this->isPlaylist = _isPlaylist;
}

void SoundPlayer::erasePlaylist() {
    this->playlist.clear();
}

void SoundPlayer::eraseMusic() {
    this->musics.clear();
}
