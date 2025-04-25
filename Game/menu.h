#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include"Graphics.h"
#include<SDL_mixer.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<string>

struct Menu {
    SDL_Texture* Title = nullptr;
    SDL_Texture* Start = nullptr;
    SDL_Texture* Quit = nullptr;
    SDL_Texture* BG = nullptr;
    Mix_Music* LobbyTheme = nullptr;

    SDL_Texture* GameOverText = nullptr;
    SDL_Texture* RestartText = nullptr;
    SDL_Texture* MenuText = nullptr;
    SDL_Texture* FinalScoreText = nullptr;

    void LoadAssets(Graphics& G, TTF_Font* font) {
        CleanupAssets();
        SDL_Color white = {255, 255, 255};
        Title = G.renderText("Racing Game", font, white);
        Start = G.renderText("Start", font, white);
        Quit = G.renderText("Quit", font, white);
        BG = G.loadTexture("Background.jpg");
        LobbyTheme = G.loadMusic("LobbyTheme.wav");

        GameOverText = G.renderText("Game Over", font, white);
        RestartText = G.renderText("Restart", font, white);
        MenuText = G.renderText("Menu", font, white);
    }

    void RenderMenu(Graphics &G, TTF_Font* font) {

        if (!Title || !Start || !Quit || !BG) {
            LoadAssets(G, font);
        }

        if (LobbyTheme) {
            G.play(LobbyTheme);
        }

        SDL_RenderClear(G.renderer);
        if (BG) G.renderTexture(BG, 0, 0, G.SCREEN_WIDTH, G.SCREEN_HEIGHT);
        if (Title) G.renderTexture(Title, 400, 200, 400, 100);
        if (Start) G.renderTexture(Start, 500, 500, 200, 100);
        if (Quit) G.renderTexture(Quit, 500, 650, 200, 100);
        G.presentScene();
    }

    void RenderGameOver(Graphics &G, TTF_Font* font, int score) {
        if (!GameOverText || !RestartText || !MenuText) {
            SDL_Color white = {255, 255, 255};
            GameOverText = G.renderText("Game Over", font, white);
            RestartText = G.renderText("Restart", font, white);
            MenuText = G.renderText("Menu", font, white);
        }

        SDL_Color white = {255, 255, 255};
        if (FinalScoreText) {
            SDL_DestroyTexture(FinalScoreText);
        }
        FinalScoreText = G.renderText(("Final Score: " + std::to_string(score)).c_str(), font, white);

        SDL_RenderClear(G.renderer);
        if (BG) G.renderTexture(BG, 0, 0, G.SCREEN_WIDTH, G.SCREEN_HEIGHT);
        if (GameOverText) G.renderTexture(GameOverText, 450, 250, 300, 100);
        if (FinalScoreText) G.renderTexture(FinalScoreText, 450, 350, 300, 50);
        if (RestartText) G.renderTexture(RestartText, 500, 500, 200, 100);
        if (MenuText) G.renderTexture(MenuText, 500, 650, 200, 100);
        G.presentScene();
    }

    void CleanupAssets() {
        if (Title) SDL_DestroyTexture(Title);
        if (Start) SDL_DestroyTexture(Start);
        if (Quit) SDL_DestroyTexture(Quit);
        if (BG) SDL_DestroyTexture(BG);
        if (LobbyTheme) Mix_FreeMusic(LobbyTheme);

        if (GameOverText) SDL_DestroyTexture(GameOverText);
        if (RestartText) SDL_DestroyTexture(RestartText);
        if (MenuText) SDL_DestroyTexture(MenuText);
        if (FinalScoreText) SDL_DestroyTexture(FinalScoreText);

        Title = nullptr;
        Start = nullptr;
        Quit = nullptr;
        BG = nullptr;
        LobbyTheme = nullptr;
        GameOverText = nullptr;
        RestartText = nullptr;
        MenuText = nullptr;
        FinalScoreText = nullptr;
    }

    ~Menu() {
        CleanupAssets();
    }
};
#endif // MENU_H_INCLUDED
