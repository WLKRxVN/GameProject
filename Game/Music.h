#ifndef MUSIC_H_INCLUDED
#define MUSIC_H_INCLUDED
#include <vector>
#include <string>
#include <random>
#include <SDL_mixer.h>
using namespace std;
struct MusicManager
{
    vector<string> MusicList;
    Mix_Music* currentMusic;
    mt19937 rng;
    MusicManager() {
        currentMusic = nullptr;
        random_device rd;
        rng = std::mt19937(rd());
    }
    ~MusicManager() {
        stopMusic();
        MusicList.clear();
    }

    void AddMusic(const string& filename){
        MusicList.push_back(filename);
    }

    void PlayRandomTrack(){
        stopMusic();

        if(MusicList.empty()){
            return;
        }

        uniform_int_distribution<int> dist(0, static_cast<int>(MusicList.size()) - 1);
        int index = dist(rng);
        currentMusic = Mix_LoadMUS(MusicList[index].c_str());
        if (currentMusic != nullptr){
            Mix_PlayMusic(currentMusic, -1);
        }

    }
    void stopMusic(){
         if (Mix_PlayingMusic()) {
            Mix_HaltMusic();
        }
        if (currentMusic != nullptr) {
            Mix_FreeMusic(currentMusic);
            currentMusic = nullptr;
        }
    }
    void cleanup(){
        stopMusic();
        MusicList.clear();
    }
};

#endif // MUSIC_H
