#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include <SDL/SDL_mixer.h>
#include <iostream>
#include "music.h"
#include <vector>

class SoundPlayer
{
private:
    std::vector<Music *> musics;
    std::vector<Music *> playlist;
    std::string pathPlaylist;
    static bool initd;
    static SoundPlayer * soundPlayer;
    bool isMusicLoaded;
    int startTime;
    int current;
    SoundPlayer();
    bool isPlaylist;

public:
    ~SoundPlayer();
    static SoundPlayer * startPlayer();
    bool getIsPlaying();
    void loadMusic();
    void playPlayer();
    void pausePlayer();
    void stopPlayer();

    bool getInitd();
    bool getIsMusicLoaded();
    void setIsMusicLoaded(bool _isMusicLoaded);
    void addMusic(std::string path);
    int getCurrent();
    void setCurrent(int _current);

    Music *getCurrentMusic();
    void loadFile();
    void goThroughFolder();
    Music* getMusicById(int id);
    Music* getMusicPlaylistById(int id);
    std::vector<Music *> getAllMusics();
    std::vector<Music *> getAllPlaylist();
    std::string getPathPlaylist();
    void setPathPlaylist(std::string _path);
    void loadPlaylist();
    bool getIsPlaylist();
    void setIsPlaylist(bool _isPlaylist);
    void erasePlaylist();
    void eraseMusic();
};

#endif // SOUNDPLAYER_H
