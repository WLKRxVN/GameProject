#ifndef OBSTACLES_H_INCLUDED
#define OBSTACLES_H_INCLUDED
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <random>
#include <algorithm>
#include <ctime>
using namespace std;

struct Obstacle{
    float x, y;
    float w, h;
    SDL_Texture* texture;
    float speed;
    bool active;

    Obstacle(SDL_Texture* tex, float startX, float startY){
        texture = tex;
        x = startX;
        y = startY;
        speed = 0;
        w = 128.0;
        h = 128.0;
        active = true;
    }

    void update(float playerSpeed){
        speed = playerSpeed;
        y += speed;
        if(y > 900){
            active = false;
        }
    }
};

struct ObstacleManager{
    vector<Obstacle> Obstacle_pos;
    SDL_Texture* Obstacle_Texture;
    int ObstacleTimer;
    mt19937 rng;
    int SpawnRate;
    int screenwidth;

    ObstacleManager(SDL_Texture* texture, int width){
        Obstacle_Texture = texture;
        SpawnRate = 120;
        ObstacleTimer = 0;
        rng = mt19937(static_cast<unsigned int>(time(nullptr)));
        screenwidth = width;
    }

    static bool IsInactive(const Obstacle& o)
    {
        return !o.active;
    }

    void update(float Speed)
    {
        ObstacleTimer++;
        if(ObstacleTimer >= SpawnRate){
            ObstacleTimer = 0;
            ObstacleSpawn();
        }

        for(auto& obstacle : Obstacle_pos){
            if(obstacle.active) {
                obstacle.update(Speed);
            }
        }

        Obstacle_pos.erase(
            remove_if(Obstacle_pos.begin(), Obstacle_pos.end(), IsInactive),
            Obstacle_pos.end()
        );
    }

    void ObstacleSpawn()
    {
        int LanePos[3] = {250, 550, 850};
        uniform_int_distribution<int> laneDist(0, 2);
        int LaneIndex = laneDist(rng);
        int Lane = LanePos[LaneIndex];
        Obstacle newObstacle(Obstacle_Texture, Lane, -64);
        Obstacle_pos.push_back(newObstacle);
    }

    void render(SDL_Renderer* renderer)
    {
        for(auto& obstacle : Obstacle_pos){
            if(obstacle.active){
                SDL_Rect dest;
                dest.x = static_cast<int>(obstacle.x);
                dest.y = static_cast<int>(obstacle.y);
                dest.w = static_cast<int>(obstacle.w);
                dest.h = static_cast<int>(obstacle.h);
                SDL_RenderCopy(renderer, obstacle.texture, NULL, &dest);
            }
        }
    }

    bool CheckCollsion(float playerX, float playerY, int playerWidth, int playerHeight){
        for(auto& obstacle : Obstacle_pos){
            if(obstacle.active){
                if( playerX < obstacle.x + obstacle.w &&
                    playerX + playerWidth > obstacle.x &&
                    playerY < obstacle.y + obstacle.h &&
                    playerY + playerHeight > obstacle.y){
                        return true;
                }
            }
        }
        return false;
    }

    void reset()
    {
        Obstacle_pos.clear();
        ObstacleTimer = 0;
    }
};
#endif // OBSTACLES_H
