#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include"Graphics.h"
#include"Car.h"
#include<iostream>
#include "obstacles.h"
using namespace std;
Graphics *graphic = nullptr;
int main(int argc, char* argv[])
{
    graphic = new Graphics();
    graphic->init();

    SDL_Event event;
    bool quit = false;
    SDL_Texture *carTexture = graphic->loadTexture("Blue.jpg");
    SDL_Texture *background = graphic->loadTexture("Road.png");
    SDL_Texture *Obstacle = graphic->loadTexture("RUM.jpg");

    Car Player(carTexture, graphic->SCREEN_WIDTH/2, graphic->SCREEN_HEIGHT/2);
    float backgroundY = 0.0f;
    bool GameOver = false;
    ObstacleManager manager(Obstacle,graphic->SCREEN_WIDTH);

    while (!quit){
        SDL_PollEvent(&event);
        if(event.type == SDL_QUIT){
            quit = true;
        }
        const Uint8* keystates = SDL_GetKeyboardState(NULL);
        if (keystates[SDL_SCANCODE_UP]) {
            Player.Accelerate();
        }
        if (keystates[SDL_SCANCODE_DOWN]) {
            Player.Stop();
        }
        if (keystates[SDL_SCANCODE_LEFT]) {
            Player.TurnLeft();
        }
        if (keystates[SDL_SCANCODE_RIGHT]) {
            Player.TurnRight();
        }
        if(!GameOver){
            Player.update();
            manager.update(Player.Speed);
            if(manager.CheckCollsion(Player.x,Player.y,64,64)){
                Player.Speed = 0;
                GameOver = true;
            }
        }

        backgroundY += Player.Speed;
        if (backgroundY >= 900) backgroundY -= 900;

        SDL_RenderClear(graphic->renderer);

        graphic->renderTexture(background, 150, (int)(backgroundY - 900), 900, 900);
        graphic->renderTexture(background, 150, (int)(backgroundY), 900, 900);
        manager.render(graphic->renderer);

        Player.y = 600;

        graphic->renderTexture(carTexture,Player.x,Player.y,64,64);

        graphic->presentScene();

        SDL_Delay(12);
    }
}
