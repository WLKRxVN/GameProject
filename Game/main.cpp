#include<SDL.h>
#include<SDL_image.h>
#include<SDL_mixer.h>
#include<SDL_ttf.h>
#include"Graphics.h"
#include"Car.h"
#include"obstacles.h"
#include"menu.h"
#include"Highscore.h"
#include<iostream>
#include"Music.h"
using namespace std;

Graphics *graphic = nullptr;
enum GameState{STATE_MENU, STATE_PLAYING, STATE_LOSE, STATE_QUIT};

int main(int argc, char* argv[])
{

    graphic = new Graphics();
    graphic->init();

    TTF_Font *font = graphic->loadFont("assets/Pixellettersfull-BnJ5.ttf", 48);

    SDL_Event event;
    bool quit = false;
    GameState CurrentState = STATE_MENU;
    int score = 0;
    int highscore = loadHighscore();

    Menu menu;
    menu.LoadAssets(*graphic, font);

    MusicManager musicManager;
    musicManager.AddMusic("assets/NEO-WINGS.wav");
    musicManager.AddMusic("assets/Gas.wav");
    musicManager.AddMusic("assets/Berry Delight.wav");

    Mix_Chunk *gAccel = nullptr;
    Mix_Chunk *gBrake = nullptr;
    SDL_Texture *carTexture = nullptr;
    SDL_Texture *background = nullptr;
    SDL_Texture *Obstacle = nullptr;
    SDL_Texture *BG = nullptr;
    Car *Player = nullptr;
    ObstacleManager *manager = nullptr;
    float backgroundY = 0.0f;

    GameState PreviousState = STATE_MENU;

    while (!quit) {

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                CurrentState = STATE_QUIT;
            }
            if (CurrentState == STATE_MENU) {
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_RETURN ||
                        event.key.keysym.sym == SDLK_SPACE) {
                        CurrentState = STATE_PLAYING;
                    } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        CurrentState = STATE_QUIT;
                    }
                }
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    if (mouseX >= 600 && mouseX <= 800 &&
                        mouseY >= 500 && mouseY <= 700) {
                        CurrentState = STATE_PLAYING;
                    }
                    else if (mouseX >= 600 && mouseX <= 800 &&
                             mouseY >= 700 && mouseY <= 800) {
                        CurrentState = STATE_QUIT;
                    }
                }
            }
            if (CurrentState == STATE_LOSE) {
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_RETURN ||
                        event.key.keysym.sym == SDLK_SPACE) {
                        musicManager.PlayRandomTrack();
                        if (manager != nullptr) {
                            manager->reset();
                        }
                        score = 0;
                        CurrentState = STATE_PLAYING;
                    } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                        CurrentState = STATE_MENU;
                    }
                }
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;
                    if (mouseX >= 500 && mouseX <= 700 &&
                        mouseY >= 500 && mouseY <= 600) {
                        score = 0;
                        musicManager.PlayRandomTrack();
                        if (manager != nullptr) {
                            manager->reset();
                        }
                        CurrentState = STATE_PLAYING;
                    }
                    else if (mouseX >= 500 && mouseX <= 700 &&
                             mouseY >= 700 && mouseY <= 800) {
                        CurrentState = STATE_MENU;
                    }
                }
            }
        }
        if (CurrentState == STATE_QUIT) {
            quit = true;
        }
        else if (CurrentState == STATE_MENU) {
            if (gAccel != nullptr) {
                Mix_FreeChunk(gAccel);
                gAccel = nullptr;
            }
            if (gBrake != nullptr) {
                Mix_FreeChunk(gBrake);
                gBrake = nullptr;
            }
            if (carTexture != nullptr) {
                SDL_DestroyTexture(carTexture);
                carTexture = nullptr;
            }
            if (background != nullptr) {
                SDL_DestroyTexture(background);
                background = nullptr;
            }
            if (Obstacle != nullptr) {
                SDL_DestroyTexture(Obstacle);
                Obstacle = nullptr;
            }
            delete Player;
            Player = nullptr;
            delete manager;
            manager = nullptr;

            menu.RenderMenu(*graphic, font);
        }
        else if (CurrentState == STATE_PLAYING) {
            if (!Mix_PlayingMusic()) {
                musicManager.PlayRandomTrack();
            }
            if (gAccel == nullptr) gAccel = graphic->loadSound("assets/accelerate.mp3");
            if (gBrake == nullptr) gBrake = graphic->loadSound("assets/brake-6315.wav");
            if (carTexture == nullptr) carTexture = graphic->loadTexture("assets/Blue.jpg");
            if (BG == nullptr) BG = graphic->loadTexture("assets/Background.jpg");
            if (background == nullptr) background = graphic->loadTexture("assets/Road.png");
            if (Obstacle == nullptr) Obstacle = graphic->loadTexture("assets/carImage.jpg");
            if (Player == nullptr) Player = new Car(carTexture, graphic->SCREEN_WIDTH/2, 700);
            if (manager == nullptr) manager = new ObstacleManager(Obstacle, graphic->SCREEN_WIDTH);

            const Uint8* keystates = SDL_GetKeyboardState(NULL);
            if (keystates[SDL_SCANCODE_UP]) {
                Player->Accelerate();
                if (!Mix_Playing(-1)) {
                    graphic->play(gAccel);
                }
            }
            if (keystates[SDL_SCANCODE_DOWN]) {
                Player->Stop();
                if (!Mix_Playing(-1)) {
                    graphic->play(gBrake);
                }
            }
            if (keystates[SDL_SCANCODE_LEFT]) {
                Player->TurnLeft();

                if (Player->x < 150) Player->x = 150;
            }
            if (keystates[SDL_SCANCODE_RIGHT]) {
                Player->TurnRight();

                if (Player->x > graphic->SCREEN_WIDTH - 214) Player->x = graphic->SCREEN_WIDTH - 214;
            }

            Player->update();
            manager->update(Player->Speed);
            score += static_cast<int>(Player->Speed * 0.1f);

            if (manager->CheckCollsion(Player->x, Player->y, 64, 64)) {
                Player->Speed = 0;
                Mix_HaltMusic();
                CurrentState = STATE_LOSE;
            }


            backgroundY += Player->Speed;
            if (backgroundY >= 900) backgroundY -= 900;

            SDL_RenderClear(graphic->renderer);
            graphic->renderTexture(BG,0,0,1200,900);
            graphic->renderTexture(background, 150, (int)(backgroundY - 900), 900, 900);
            graphic->renderTexture(background, 150, (int)(backgroundY), 900, 900);
            manager->render(graphic->renderer);


            Player->y = 700;
            graphic->renderTexture(carTexture, Player->x, Player->y, 64, 64);


            SDL_Color white = {255, 255, 255};
            SDL_Texture* scoreText = graphic->renderText(("Score: " + to_string(score)).c_str(), font, white);

            if (score > highscore) {
            highscore = score;
            }
            graphic->renderTexture(scoreText, 50, 50, 200, 50);
            SDL_DestroyTexture(scoreText);

            graphic->presentScene();
        }
        else if (CurrentState == STATE_LOSE) {
            menu.RenderGameOver(*graphic, font, score, highscore);
        }
        if (PreviousState != CurrentState) {
            if (CurrentState == STATE_PLAYING) {
                Mix_HaltMusic();
                musicManager.PlayRandomTrack();
            }else if (CurrentState == STATE_MENU) {
                Mix_HaltMusic();
             }
        PreviousState = CurrentState;
        }
        SDL_Delay(16);
    }


    if (gAccel != nullptr) Mix_FreeChunk(gAccel);
    if (gBrake != nullptr) Mix_FreeChunk(gBrake);
    if (carTexture != nullptr) SDL_DestroyTexture(carTexture);
    if (background != nullptr) SDL_DestroyTexture(background);
    if (Obstacle != nullptr) SDL_DestroyTexture(Obstacle);
    if (BG != nullptr) SDL_DestroyTexture(BG);
    delete Player;
    delete manager;
    menu.CleanupAssets();
    musicManager.cleanup();
    if (font != nullptr) TTF_CloseFont(font);
    saveHighscore(highscore);

    delete graphic;

    return 0;
}
