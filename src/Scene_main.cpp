#include "Scene_main.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

Scene_main::Scene_main():game(Game::get_instance()),player(new Player())
{

}
Scene_main::~Scene_main()
{
    clean();
    delete player;
}
void Scene_main::init()
{
    player->texture = IMG_LoadTexture(game.get_renderer(),"assets/image/SpaceShip.png");
    if(player->texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Failed to load player texture: %s\n",SDL_GetError());
    }
    SDL_QueryTexture(player->texture,NULL,NULL,&player->width,&player->height);
    player->height/=4;
    player->width/=4;
    player->postion.x = game.get_window_width() / 2 - player->width / 2;
    player->postion.y = game.get_window_height() - player->height;
}
void Scene_main::clean()
{
    if(player->texture != nullptr)
    {
        SDL_DestroyTexture(player->texture);
    }
}
void Scene_main::render()
{
    SDL_Rect player_rect = {static_cast<int>(player->postion.x),static_cast<int>(player->postion.y),player->width,player->height};
    SDL_RenderCopy(game.get_renderer(),player->texture,NULL,&player_rect);
}
void Scene_main::update(float delta_time)
{
    keyboard_control(delta_time);
}
void Scene_main::handle_event(SDL_Event* event)
{
    
}
//键盘控制飞机移动
void Scene_main::keyboard_control(float delta_time)
{
    auto keyboard_state = SDL_GetKeyboardState(NULL);
    if(keyboard_state[SDL_SCANCODE_W] | keyboard_state[SDL_SCANCODE_UP])
    {
        player->postion.y -= delta_time * player->speed;
    }
    if(keyboard_state[SDL_SCANCODE_S] | keyboard_state[SDL_SCANCODE_DOWN])
    {
        player->postion.y += delta_time * player->speed;
    }
    if(keyboard_state[SDL_SCANCODE_A] | keyboard_state[SDL_SCANCODE_LEFT])
    {
        player->postion.x -= delta_time * player->speed;
    }
    if(keyboard_state[SDL_SCANCODE_D] | keyboard_state[SDL_SCANCODE_RIGHT])
    {
        player->postion.x += delta_time * player->speed;
    }
    if(player->postion.x < 0){ player->postion.x = 0;}
    if(player->postion.x > game.get_window_width() - player->width){ player->postion.x = game.get_window_width() - player->width;}
    if(player->postion.y < 0){ player->postion.y = 0;}
    if(player->postion.y > game.get_window_height() - player->height){ player->postion.y = game.get_window_height() - player->height;}
}