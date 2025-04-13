#include "Scene_main.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <chrono>

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
    //加载玩家的材质
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
    //加载子弹的材质(先创建一个子弹模板)
    template_bullet.texture = IMG_LoadTexture(game.get_renderer(),"assets/image/bullet.png");
    SDL_QueryTexture(template_bullet.texture,NULL,NULL,&template_bullet.width,&template_bullet.height);
    template_bullet.width /=4;
    template_bullet.height /=4;
}
void Scene_main::clean()
{
    if(player->texture != nullptr)
    {
        SDL_DestroyTexture(player->texture);
    }
    for(auto& bullet : bullets){
        if(bullet != nullptr)
        {
           delete bullet;
        }
    }
    bullets.clear();
    if(template_bullet.texture != nullptr)
    {
        SDL_DestroyTexture(template_bullet.texture);
    }
}
void Scene_main::render()
{
    //渲染玩家
    SDL_Rect player_rect = {static_cast<int>(player->postion.x),static_cast<int>(player->postion.y),player->width,player->height};
    SDL_RenderCopy(game.get_renderer(),player->texture,NULL,&player_rect);
    //渲染子弹
    render_bullets();
}
void Scene_main::update(float delta_time)
{
    keyboard_control(delta_time);
    update_bullets(delta_time);
}
void Scene_main::handle_event(SDL_Event* event)
{
    
}
// 键盘控制
void Scene_main::keyboard_control(float delta_time)
{
    auto keyboard_state = SDL_GetKeyboardState(NULL);
    // 控制玩家移动
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
    // 控制子弹发射
    if(keyboard_state[SDL_SCANCODE_J])
    {
        // 子弹冷却控制
        auto current_time = SDL_GetTicks();
        if(current_time -  player->last_shot_time > player->cool_down)
        { 
            shoot();
            player->last_shot_time = current_time;
        }
    }
}
void Scene_main::shoot()
{
    Bullet* bullet_player = new Bullet(template_bullet);//拷贝构造创建子弹，防止多次读取文件
    bullet_player->position.x = player->postion.x + player->width/2 - bullet_player->width/2;
    bullet_player->position.y = player->postion.y;
    bullets.push_back(bullet_player);
}
void Scene_main::update_bullets(float delta_time)
{
    int margin = 32;
    // 用迭代器控制子弹飞行渲染
    for(auto it = bullets.begin();it!= bullets.end();)
    {
        auto bullet = *it;
        bullet->position.y -=bullet->speed * delta_time;
        if(bullet->position.y + margin < 0)
        {
            delete bullet;
            it = bullets.erase(it);
            //SDL_Log("Player's bullet destroyed.");
        }
        else{
            ++it;
        }
    }
}

void Scene_main::render_bullets()
{
    for(auto bullet : bullets)
    {
        SDL_Rect bullet_rect = {static_cast<int>(bullet->position.x),static_cast<int>(bullet->position.y),bullet->width,bullet->height};
        SDL_RenderCopy(game.get_renderer(),bullet->texture,NULL,&bullet_rect);
    }
}