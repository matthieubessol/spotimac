#include "music.h"
#include <iostream>
#include <SDL/SDL_mixer.h>

#include <taglib/taglib.h>
#include <taglib/tag.h>
#include <taglib/fileref.h>

Music::Music() {
    albumName = "";
    artistName = "";
    trackName = "";
    length = 0;
    startTime = 0;
    pathFile = "";
    music = NULL;
}

Music::~Music() {

}

Music::Music(std::string path) {
    TagLib::FileRef g(path.c_str());
    this->artistName = g.tag()->artist().toCString();
    this->albumName  = g.tag()->album().toCString();
    this->trackName  = g.tag()->title().toCString();
    this->length     = g.audioProperties()->length();
    this->music      = NULL;
    this->pathFile   = path;
}

void Music::loadMusic() {
    this->music = Mix_LoadMUS( this->pathFile.c_str() );
}

std::string Music::getTrackName() {
    return this->trackName;
}

std::string Music::getAlbumName() {
    return this->albumName;
}

std::string Music::getArtistName() {
    return this->artistName;
}

int Music::getLength() {
    return this->length;
}

int Music::getStartTime() {
    return this->startTime;
}

Mix_Music * Music::getMusic() {
    return this->music;
}

void Music::setStartTime(int _time) {
    this->startTime = _time;
}

std::string Music::getPathFile() {
    return this->pathFile;
}
