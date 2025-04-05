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
    bool up, down;

};
Graphics *graphic = nullptr;
int main(int argc, char* argv[])
{
    graphic = new Graphics();
    graphic->init();

    SDL_Event event;
    bool quit = false;
    SDL_Texture *carTexture = graphic->loadTexture("Blue.jpg");
    SDL_Texture *background = graphic->loadTexture("Road.png");

    Car Player(carTexture, graphic->SCREEN_WIDTH/2, graphic->SCREEN_HEIGHT/2);
    float backgroundY = 0.0f;
    while (!quit){
        SDL_PollEvent(&event);
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
        Player.update();
        backgroundY += Player.Speed;
        if (backgroundY >= 900) backgroundY -= 900;

        SDL_RenderClear(graphic->renderer);

        graphic->renderTexture(background, 150, (int)(backgroundY - 900), 900, 900);
        graphic->renderTexture(background, 150, (int)(backgroundY), 900, 900);

        Player.y = 600;

        cerr << Player.Speed;
        graphic->renderTexture(carTexture,Player.x,Player.y,64,64);

        graphic->presentScene();

        SDL_Delay(8);
    }
}
