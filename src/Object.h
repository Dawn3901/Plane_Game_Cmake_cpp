#pragma once
#include<SDL.h>

struct Player
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint postion = {0,0};
    int width = 0;
    int height = 0;
    int speed = 200;
    int current_health = 3;
    Uint32 last_shot_time = 0;
    Uint32 cool_down = 500;
};
struct Enemy
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint postion = {0,0};
    int width = 0;
    int height = 0;
    int speed = 200;
    int current_health = 2;
    Uint32 last_shot_time = 0;
    Uint32 cool_down = 500;
};
struct Bullet
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0,0};
    int width = 0;
    int height = 0;
    int speed = 400;
    int damage = 1;
};
struct Enemy_Bullet
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint position = {0,0};
    SDL_FPoint direction = {0,0};
    int width = 0;
    int height = 0;
    int speed = 400;
    int damage = 1;
};