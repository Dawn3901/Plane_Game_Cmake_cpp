#pragma once
#include<SDL.h>

enum class item_type
{
    Life,Shield,Time
};

struct Player
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint postion = {0,0};
    int width = 0;
    int height = 0;
    int speed = 200;
    int current_health = 3;
    int max_health = 5;
    Uint32 last_shot_time = 0;
    Uint32 cool_down = 150;
};
struct Enemy
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint postion = {0,0};
    int width = 0;
    int height = 0;
    int speed = 150;
    int current_health = 2;
    Uint32 last_shot_time = 0;
    Uint32 cool_down = 2000;
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
struct Explosion
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint postion = {0,0};
    int width = 0;
    int height = 0;
    int current_frame = 0;
    int total_frame = 0;
    Uint32 start_time = 0;
    Uint32 FPS = 10;
};
struct Item
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint postion = {0,0};
    SDL_FPoint direction = {0,0};
    int width = 0;
    int height = 0;
    int speed = 200;
    int bounce_count = 3;
    item_type type = item_type::Life;
};
struct Background
{
    SDL_Texture* texture = nullptr;
    SDL_FPoint postion = {0,0};
    float offset = 0;
    int width = 0;
    int height = 0;
    int speed = 50;
};