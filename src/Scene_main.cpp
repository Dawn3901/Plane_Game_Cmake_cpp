#include "Scene_main.h"
#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <chrono>
#include <random>

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
    // 生成随机数
    std::random_device rd;// 生成随机数种子
    gen = std::mt19937(rd());// 生成随机数引擎
    dis = std::uniform_real_distribution<float>(0.0f,1.0f);// 指定随机分布
    auto r = dis(gen); // 获取随机数

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
    
    // 加载敌机的材质(敌机模板)
    template_enemy.texture = IMG_LoadTexture(game.get_renderer(),"assets/image/insect-1.png");
    SDL_QueryTexture(template_enemy.texture,NULL,NULL,&template_enemy.width,&template_enemy.height);
    template_enemy.width /=4;
    template_enemy.height /=4;
    template_monster.texture = IMG_LoadTexture(game.get_renderer(),"assets/image/insect-2.png");
    SDL_QueryTexture(template_monster.texture,NULL,NULL,&template_monster.width,&template_monster.height);
    template_monster.width /=4;
    template_monster.height /=4;
    //加载子弹的材质(先创建一个子弹模板)
    template_bullet.texture = IMG_LoadTexture(game.get_renderer(),"assets/image/bullet.png");
    SDL_QueryTexture(template_bullet.texture,NULL,NULL,&template_bullet.width,&template_bullet.height);
    template_bullet.width /=4;
    template_bullet.height /=4;
}
void Scene_main::clean()
{
    // 清理玩家材质
    if(player->texture != nullptr)
    {
        SDL_DestroyTexture(player->texture);
    }
    // 清理子弹
    for(auto& bullet : bullets)
    {
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
    // 清理敌机
    for(auto& enemy:enemies)
    {
        if(enemy != nullptr)
        {
            delete enemy;
        }
    }
    enemies.clear();
    if(template_enemy.texture != nullptr)
    {
        SDL_DestroyTexture(template_enemy.texture);
    }
    for(auto& monster:monsters)
    {
        if(monster!= nullptr)
        {
            delete monster;
        }
    }
    monsters.clear();
    if(template_monster.texture != nullptr)
    {
        SDL_DestroyTexture(template_monster.texture);
    }
}
void Scene_main::render()
{
    // 渲染玩家
    SDL_Rect player_rect = {static_cast<int>(player->postion.x),static_cast<int>(player->postion.y),player->width,player->height};
    SDL_RenderCopy(game.get_renderer(),player->texture,NULL,&player_rect);
    // 渲染子弹
    render_bullets();
    // 渲染敌机
    render_enemies();
}
void Scene_main::update(float delta_time)
{
    keyboard_control(delta_time);
    update_bullets(delta_time);
    spawn_enemy();
    update_enemies(delta_time);
}
void Scene_main::handle_event(SDL_Event* event)
{
    
}
// 键盘控制
void Scene_main::keyboard_control(float delta_time)
{
    auto keyboard_state = SDL_GetKeyboardState(NULL);
    // 控制玩家移动
    if(keyboard_state[SDL_SCANCODE_LSHIFT])
        player->speed = 300;
    else player->speed = 200;
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
    if(keyboard_state[SDL_SCANCODE_J] | keyboard_state[SDL_SCANCODE_U])
    {
        // 子弹冷却控制
        auto current_time = SDL_GetTicks();
        if(current_time -  player->last_shot_time > player->cool_down)
        { 
            if(keyboard_state[SDL_SCANCODE_J])
            {
                shoot();
            }
            else{
                double_shoot();
            }
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
void Scene_main::double_shoot()
{
    Bullet* bullet_player_1 = new Bullet(template_bullet);
    bullet_player_1->position.x = player->postion.x + player->width/2 - bullet_player_1->width/2 - 10;
    bullet_player_1->position.y = player->postion.y;
    bullets.push_back(bullet_player_1);
    Bullet* bullet_player_2 = new Bullet(template_bullet);
    bullet_player_2->position.x = player->postion.x + player->width/2 - bullet_player_2->width/2 + 10;
    bullet_player_2->position.y = player->postion.y;
    bullets.push_back(bullet_player_2);
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
void Scene_main::spawn_enemy()
{
    // 每帧调用, 但是需要判断是否生成敌机
    if(dis(gen) > 1.0f/60.0f) return;
    if(dis(gen) > 4.0f/6.0f) 
    {
        Enemy* monster = new Enemy(template_monster);
        monster->postion.x = dis(gen) * (game.get_window_width() - monster->width);
        monster->postion.y = - monster->height;
        monster->speed = monster->speed/2.0 +(monster->speed/2.0) * dis(gen);
        monsters.push_back(monster);
    }
    else 
    {
        Enemy* enemy = new Enemy(template_enemy);
        enemy->postion.x = dis(gen) * (game.get_window_width() - enemy->width);
        enemy->postion.y = -enemy->height;
        enemy->speed = enemy->speed/2.0 + (enemy->speed/3.0) * dis(gen); 
        enemies.push_back(enemy);
    }
}
void Scene_main::update_enemies(float delta_time)
{
    for(auto it = enemies.begin();it != enemies.end();)
    {
        auto enemy = *it;
        enemy->postion.y += enemy->speed * delta_time;
        if(enemy->postion.y > game.get_window_height()){
            delete enemy;
            it = enemies.erase(it);
        }
        else it++;
    }
    for(auto it = monsters.begin();it!= monsters.end();)
    {
        auto monster = *it;
        monster->postion.y += monster->speed * delta_time;
        if(monster->postion.y > game.get_window_width())
        {
            delete monster;
            it = monsters.erase(it);
        }
        else it++;
    }
}
void Scene_main::render_enemies()
{
    for(auto enemy:enemies)
    {
        SDL_Rect enemy_rect = {static_cast<int>(enemy->postion.x),static_cast<int>(enemy->postion.y),enemy->width,enemy->height};
        SDL_RenderCopy(game.get_renderer(),enemy->texture,NULL,&enemy_rect);
    }
    for(auto monster:monsters)
    {
        SDL_Rect monster_rect = {static_cast<int>(monster->postion.x),static_cast<int>(monster->postion.y),monster->width,monster->height};
        SDL_RenderCopy(game.get_renderer(),monster->texture,NULL,&monster_rect);
    }
}