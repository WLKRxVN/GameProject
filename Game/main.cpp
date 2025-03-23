#include<SDL.h>
#include<SDL_image.h>
#include<SDl_mixer.h>
#include"Graphics.h"
#include"Car.h"
#include<iostream>
using namespace std;
struct Game
{
    bool running;
    Car* Pcar;

};
Graphics *game = nullptr;
int main(int argc, char* argv[])
{
    game = new Graphics();
    game->init();
    SDL_Event event;
    bool quit = false;
    while (!quit){
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT){
                quit = true;
                break;
        }else if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
            case SDLK_UP:
                cout << "easy";
                break;
            case SDLK_DOWN:
                cout << "skibidi";
                break;
            case SDLK_RIGHT:
                cout << "lvl";
                break;
            case SDLK_LEFT:
                cout << "1";
                break;
            default:
                break;
            }
        }
    }
}
