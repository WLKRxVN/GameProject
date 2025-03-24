#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include"Graphics.h"
#include"Car.h"
#include<iostream>
using namespace std;
struct Game
{
    bool running;
    Car* Pcar;

};
Graphics *graphic = nullptr;
int main(int argc, char* argv[])
{
    graphic = new Graphics();
    graphic->init();

    SDL_Event event;
    bool quit = false;
    SDL_Texture *carTexture = graphic->loadTexture("Blue.jpg");

    Car Player(carTexture, graphic->SCREEN_WIDTH/2, graphic->SCREEN_HEIGHT/2);

    while (!quit){
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT){
                quit = true;
                break;
        }else if(event.type == SDL_KEYDOWN){
            switch(event.key.keysym.sym){
            case SDLK_UP:
                Player.Accelerate();
                break;
            case SDLK_DOWN:
                Player.Stop();
                break;
            case SDLK_RIGHT:
                Player.TurnRight();
                break;
            case SDLK_LEFT:
                Player.TurnLeft();
                break;
            default:
                break;
            }
        }
        SDL_SetRenderDrawColor(graphic->renderer, 255, 255, 255, 255); // White background
        SDL_RenderClear(graphic->renderer);
        Player.update();
        Player.Destination.x = Player.x / 2;
        Player.Destination.y = Player.y / 2;
        SDL_RenderClear(graphic->renderer);

        graphic->renderTexture(carTexture,Player.x,Player.y);

        graphic->presentScene();

        SDL_Delay(16);  // Approximately 60 FPS
    }
}
