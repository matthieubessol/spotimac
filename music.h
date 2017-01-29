#ifndef MUSIC_H
#define MUSIC_H

#include <iostream>
#include <SDL/SDL_mixer.h>

class Music{
private:
    Mix_Music *music;
    std::string trackName;
    std::string albumName;
    std::string artistName;
    std::string pathFile;
    int length;
    int startTime;

public:
    Music();
    ~Music();
    Music(std::string path);
    std::string getTrackName();
    std::string getAlbumName();
    std::string getArtistName();
    std::string getPathFile();
    int getLength();
    int getStartTime();
    Mix_Music *getMusic();
    void loadMusic();
    void setStartTime(int _time);
};


#endif // MUSIC_H
