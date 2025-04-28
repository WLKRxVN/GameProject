#include<SDL.h>
#include<SDL_image.h>

struct Car
{
    float x,y;
    float Speed;
    float Accel;
    float MaxSpeed;
    float Turn;
    float Brake;

    SDL_Texture* Texture;
    SDL_Rect Sprite;
    SDL_Rect Destination;

    Car(SDL_Texture* texture, float StartX, float StartY){
         x = StartX;
         y = StartY;
         Speed = 0.0f;
         MaxSpeed = 30.0f;
         Accel = 0.2f;
         Turn = 10.0f;
         Brake = 0.3f;

         Sprite = {0,0,70,60};
         Destination = {x,y,70,60};
    }
    void update()
    {
        y -= Speed;

        Destination.w = 64;
        Destination.h = 64;
    }
    void Accelerate()
    {
        Speed += Accel;
        if(MaxSpeed < Speed){
            Speed = MaxSpeed;
        }
    }
    void Stop()
    {
        if(Speed >= 0.0){
            Speed -= Brake;
            if(MaxSpeed < Speed){
                Speed = MaxSpeed;
            }
        }
    }
    void TurnLeft()
    {
        x -= Turn;
    }
    void TurnRight()
    {
        x += Turn;
    }

};
