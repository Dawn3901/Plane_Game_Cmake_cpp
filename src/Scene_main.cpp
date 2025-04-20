#include "Scene_main.h"
#include "Scene_title.h"
#include "Game.h"
#include "Scene_end.h"
#include <SDL.h>
#include <SDL_image.h>
#include <chrono>
#include <random>
#include <string>

const float pi = 3.14159f;
const float drop_possibility = 0.5f;
Scene_main::Scene_main():player(new Player())
{

}
Scene_main::~Scene_main()
{
    clean();
    delete player;
}
// 基本框架
void Scene_main::init()
{
    // 加载音乐
    bgm = Mix_LoadMUS("assets/music/03_Racing_Through_Asteroids_Loop.ogg");
    if(bgm == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Failed to load music! %s\n",Mix_GetError());
    }
    Mix_PlayMusic(bgm,-1);
    sounds["player_shoot"] = Mix_LoadWAV("assets/sound/laser_shoot4.wav");
    sounds["enemy_shoot"] = Mix_LoadWAV("assets/sound/xs_laser.wav");
    sounds["player_explosion"] = Mix_LoadWAV("assets/sound/explosion1.wav");
    sounds["enemy_explosion"] = Mix_LoadWAV("assets/sound/explosion3.wav");
    sounds["get_item"] = Mix_LoadWAV("assets/sound/eff5.wav");
    sounds["hit"] = Mix_LoadWAV("assets/sound/eff11.wav");
    // 生成随机数
    std::random_device rd;// 生成随机数种子
    gen = std::mt19937(rd());// 生成随机数引擎
    dis = std::uniform_real_distribution<float>(0.0f,1.0f);// 指定随机分布
    //auto r = dis(gen); // 获取随机数

    // 加载玩家的材质
    player->texture = IMG_LoadTexture(game.get_renderer(),"assets/image/SpaceShip.png");
    if(player->texture == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Failed to load player texture: %s\n",SDL_GetError());
    }
    SDL_QueryTexture(player->texture,NULL,NULL,&player->width,&player->height);
    player->height/=5;
    player->width/=5;
    player->postion.x = game.get_window_width() / 2 - player->width / 2;
    player->postion.y = game.get_window_height() - player->height;
    // 加载生命值ui材质
    health_ui = IMG_LoadTexture(game.get_renderer(),"assets/image/Health UI Black.png");
    // 加载字体
    score_font = TTF_OpenFont("assets/font/VonwaonBitmap-12px.ttf",24);
    // 加载敌机的材质(敌机模板)
    template_enemy.texture = IMG_LoadTexture(game.get_renderer(),"assets/image/insect-1.png");
    SDL_QueryTexture(template_enemy.texture,NULL,NULL,&template_enemy.width,&template_enemy.height);
    template_enemy.width /=4;
    template_enemy.height /=4;
    template_monster.texture = IMG_LoadTexture(game.get_renderer(),"assets/image/insect-2.png");
    SDL_QueryTexture(template_monster.texture,NULL,NULL,&template_monster.width,&template_monster.height);
    template_monster.width /=4;
    template_monster.height /=4;
    // 加载我方子弹的材质(先创建一个子弹模板)
    template_bullet.texture = IMG_LoadTexture(game.get_renderer(),"assets/image/bullet.png");
    SDL_QueryTexture(template_bullet.texture,NULL,NULL,&template_bullet.width,&template_bullet.height);
    template_bullet.width /=4;
    template_bullet.height /=4;
    // 加载敌方子弹的材质模板
    template_enemy_bullet.texture = IMG_LoadTexture(game.get_renderer(),"assets/image/bullet-1.png");
    SDL_QueryTexture(template_enemy_bullet.texture,NULL,NULL,&template_enemy_bullet.width,&template_enemy_bullet.height);
    template_enemy_bullet.width /= 4;
    template_enemy_bullet.height /= 4;
    // 加载爆炸特性模板
    template_explosion.texture = IMG_LoadTexture(game.get_renderer(),"assets/effect/explosion.png");
    SDL_QueryTexture(template_explosion.texture,NULL,NULL,&template_explosion.width,&template_explosion.height);
    template_explosion.total_frame = template_explosion.width/template_explosion.height;
    template_explosion.height *= 2;
    template_explosion.width = template_explosion.height;
    // 加载道具的模板
    template_life_item.texture = IMG_LoadTexture(game.get_renderer(),"assets/image/bonus_life.png");
    SDL_QueryTexture(template_life_item.texture,NULL,NULL,&template_life_item.width,&template_life_item.height);
    template_life_item.width /= 4;
    template_life_item.height /= 4;
    template_life_item.type = item_type::Life;
    template_time_item.texture = IMG_LoadTexture(game.get_renderer(),"assets/image/bonus_time.png");
    SDL_QueryTexture(template_time_item.texture,NULL,NULL,&template_time_item.width,&template_time_item.height);
    template_time_item.width /=4;
    template_time_item.height /=4;
    template_time_item.type = item_type::Time;
    template_shield_item.texture = IMG_LoadTexture(game.get_renderer(),"assets/image/bonus_shield.png");
    SDL_QueryTexture(template_shield_item.texture,NULL,NULL,&template_shield_item.width,&template_shield_item.height);
    template_shield_item.width /=4;
    template_shield_item.height /=4;
    template_shield_item.type = item_type::Shield;

}
void Scene_main::clean()
{
    if(template_explosion.texture != nullptr)
    {
        SDL_DestroyTexture(template_explosion.texture);
    }
    // 清理玩家材质
    if(player->texture != nullptr)
    {
        SDL_DestroyTexture(player->texture);
    }
    // 清理生命值ui材质
    if(health_ui != nullptr)
    {
        SDL_DestroyTexture(health_ui);
    }
    // 清理字体
    if(score_font != nullptr)
    {
        TTF_CloseFont(score_font);
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
    // 清理敌方子弹
    for(auto& bullet:enemy_bullets)
    {
        if(bullet != nullptr)
        {
            delete bullet;
        }
    }
    enemy_bullets.clear();
    if (template_enemy_bullet.texture != nullptr)
    {
        SDL_DestroyTexture(template_enemy_bullet.texture);
    }
    // 清理道具
    for(auto& item : items)
    {
        if(item != nullptr)
        {
            delete item;
        }
    }
    items.clear();
    if(template_life_item.texture != nullptr)
    {
        SDL_DestroyTexture(template_life_item.texture);
    }
    if(template_time_item.texture != nullptr)
    {
        SDL_DestroyTexture(template_time_item.texture);
    }
    if(template_shield_item.texture != nullptr)
    {
        SDL_DestroyTexture(template_shield_item.texture);
    }
    // 清理音乐
    if(bgm != nullptr)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
    }
    for(auto sound : sounds)
    {
        if(sound.second != nullptr)
        {
            Mix_FreeChunk(sound.second);
        }
    }
    sounds.clear();
}
void Scene_main::render()
{
    // 渲染玩家
    if(!is_dead)
    {
        SDL_Rect player_rect = {static_cast<int>(player->postion.x),static_cast<int>(player->postion.y),player->width,player->height};
        SDL_RenderCopy(game.get_renderer(),player->texture,NULL,&player_rect);
    }
    // 渲染子弹
    render_bullets();
    // 渲染敌机
    render_enemies();
    // 渲染道具
    render_item();
    // 渲染爆炸特性
    render_explosion();
    // 渲染ui
    render_ui();
}
void Scene_main::update(float delta_time)
{
    keyboard_control(delta_time);
    update_bullets(delta_time);
    spawn_enemy();
    update_enemies(delta_time);
    update_enemy_bullets(delta_time);
    update_player();
    update_explosion();
    update_item(delta_time);
    if(is_dead)
    {
        change_scene_delay(delta_time,1.5f);
    }
}
void Scene_main::handle_event(SDL_Event* event)
{
    if(event->type == SDL_KEYDOWN)
    {
        if(event->key.keysym.scancode == SDL_SCANCODE_ESCAPE)
        {
            Scene_title* scene_title = new Scene_title();
            game.change_scene(scene_title);
        }
    }
}
// 键盘控制
void Scene_main::keyboard_control(float delta_time)
{
    if(is_dead) return;
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
void Scene_main::change_scene_delay(float delta_time,float delay)
{
    timer_end += delta_time;
    if(timer_end >= delay)
    {
        game.set_final_score(score);
        Scene_end* scene_end = new Scene_end();
        game.change_scene(scene_end);
    }
}

// render
void Scene_main::render_bullets()
{
    // 渲染玩家子弹
    for(auto bullet : bullets)
    {
        SDL_Rect bullet_rect = {static_cast<int>(bullet->position.x),static_cast<int>(bullet->position.y),bullet->width,bullet->height};
        SDL_RenderCopy(game.get_renderer(),bullet->texture,NULL,&bullet_rect);
    }
    // 渲染敌机子弹
    for(auto bullet : enemy_bullets)
    {
        SDL_Rect bullet_rect = {static_cast<int>(bullet->position.x),static_cast<int>(bullet->position.y),bullet->width,bullet->height};
        float angle = atan2(bullet->direction.y,bullet->direction.x) * 180 / pi - 90.0f;
        SDL_RenderCopyEx(game.get_renderer(),bullet->texture,NULL,&bullet_rect,angle,NULL,SDL_FLIP_NONE);
    }
}
void Scene_main::render_explosion()
{
    for(auto explosion : explosions)
    {
        SDL_Rect explosion_src = {explosion->current_frame * explosion->width,0,explosion->width / 2,explosion->width / 2};
        SDL_Rect explosion_dst = {static_cast<int>(explosion->postion.x),static_cast<int>(explosion->postion.y),explosion->width,explosion->height};
        SDL_RenderCopy(game.get_renderer(),explosion->texture,&explosion_src,&explosion_dst);
    }
}
void Scene_main::render_ui()
{
    // 渲染声明值
    int x = 10,y = 10;
    int offset = 40;
    int size = 32;
    SDL_SetTextureColorMod(health_ui,100,100,100);
    for(int i = 0;i < player->max_health; ++i)
    {
        SDL_Rect rect = {x + i * offset,y,size,size};
        SDL_RenderCopy(game.get_renderer(),health_ui,NULL,&rect);

    }
    SDL_SetTextureColorMod(health_ui,255,255,255); //还原颜色
    for(int i = 0;i < player->current_health;++i)
    {
        SDL_Rect rect = {x + i * offset,y,size,size};
        SDL_RenderCopy(game.get_renderer(),health_ui,NULL,&rect);
    }
    // 渲染得分
    std::string score_text = "SCORE: " + std::to_string(score);
    SDL_Color  score_color = {255,255,255,255};
    SDL_Surface* surface = TTF_RenderText_Solid(score_font,score_text.c_str(),score_color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(game.get_renderer(),surface);
    SDL_Rect score_rect = {game.get_window_width() - surface->w,10,surface->w,surface->h};
    SDL_RenderCopy(game.get_renderer(),texture,NULL,&score_rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
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

void Scene_main::render_item()
{
    for(auto item : items)
    {
        SDL_Rect item_rect = {static_cast<int>(item->postion.x),static_cast<int>(item->postion.y),item->width,item->height};
        SDL_RenderCopy(game.get_renderer(),item->texture,NULL,&item_rect);
    }
}

// update
void Scene_main::update_player()
{
    if(is_dead) return;
    if(player->current_health <= 0)
    {
        auto current_time = SDL_GetTicks();
        Explosion* explosion = new Explosion(template_explosion);
        explosion->postion.x = player->postion.x + player->width/2 - explosion->width/2;
        explosion->postion.y = player->postion.y + player->height/2 - explosion->height/2;
        explosion->start_time = current_time;
        explosions.push_back(explosion);
        Mix_PlayChannel(-1,sounds["player_explosion"],0);
        is_dead = true;
        return;
    }
}
void Scene_main::update_bullets(float delta_time)
{
    int margin = 32;
    // 用迭代器控制子弹飞行渲染
    for(auto it = bullets.begin();it!= bullets.end();)
    {
        auto bullet = *it;
        bullet->position.y -= bullet->speed * delta_time;
        if(bullet->position.y + margin < 0)
        {
            delete bullet;
            it = bullets.erase(it);
        }
        else{
            bool hit = false;
            for(auto enemy : enemies)
            {
                SDL_Rect bullet_rect = {static_cast<int>(bullet->position.x),static_cast<int>(bullet->position.y),bullet->width,bullet->height};
                SDL_Rect enemy_rect = {static_cast<int>(enemy->postion.x),static_cast<int>(enemy->postion.y),enemy->width,enemy->height};
                if(SDL_HasIntersection(&bullet_rect,&enemy_rect))
                {
                    enemy->current_health -= bullet->damage;
                    Mix_PlayChannel(-1,sounds["hit"],0);
                    delete bullet;
                    it = bullets.erase(it);
                    hit = true;
                    break;
                }
            }
            for(auto monster : monsters)
            {
                SDL_Rect bullet_rect = {static_cast<int>(bullet->position.x),static_cast<int>(bullet->position.y),bullet->width,bullet->height};
                SDL_Rect monster_rect = {static_cast<int>(monster->postion.x),static_cast<int>(monster->postion.y),monster->width,monster->height};
                if(SDL_HasIntersection(&bullet_rect,&monster_rect))
                {
                    monster->current_health -= bullet->damage;
                    Mix_PlayChannel(-1,sounds["hit"],0);
                    delete bullet;
                    it = bullets.erase(it);
                    hit = true;
                    break;
                }
            }
            if(!hit)
            {
                ++it;
            }
        }
    }
}
void Scene_main::update_enemies(float delta_time)
{
    auto current_time = SDL_GetTicks();
    SDL_Rect player_rect = {static_cast<int>(player->postion.x),static_cast<int>(player->postion.y),player->width,player->height};
    for(auto it = enemies.begin();it != enemies.end();)
    {
        auto enemy = *it;
        enemy->postion.y += enemy->speed * delta_time;
        SDL_Rect enemy_rect = {static_cast<int>(enemy->postion.x),static_cast<int>(enemy->postion.y),enemy->width,enemy->height};
        if(SDL_HasIntersection(&enemy_rect,&player_rect)  && !is_dead)
        {
            player->current_health -= 1;
            enemy->current_health = 0;
        }
        if(enemy->postion.y > game.get_window_height()){
            delete enemy;
            it = enemies.erase(it);
        }
        else 
        {
            if(current_time - enemy->last_shot_time > enemy->cool_down && !is_dead)
            {
                shoot(enemy);
                enemy->last_shot_time = current_time;
            }
            if(enemy->current_health <= 0)
            {
                enemy_explode(enemy);
                score += 10;
                it = enemies.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
    for(auto it = monsters.begin();it!= monsters.end();)
    {
        auto monster = *it;
        monster->postion.y += monster->speed * delta_time;
        SDL_Rect monster_rect = {static_cast<int>(monster->postion.x),static_cast<int>(monster->postion.y),monster->width,monster->height};
        if(SDL_HasIntersection(&monster_rect,&player_rect) && !is_dead)
        {
            monster->current_health = 0;
            player->current_health -=1;
        }
        if(monster->postion.y > game.get_window_height())
        {
            delete monster;
            it = monsters.erase(it);
        }
        else
        {
            if(current_time - monster->last_shot_time > monster->cool_down && !is_dead)
            {
                shoot(monster);
                monster->last_shot_time = current_time;
            }
            if(monster->current_health <= 0)
            {
                enemy_explode(monster);
                score += 20;
                it = monsters.erase(it);
            }
            else it++;
        }
    }
}
void Scene_main::update_enemy_bullets(float delta_time)
{
    int margin = 32;
    for(auto it = enemy_bullets.begin();it != enemy_bullets.end();)
    {
        auto bullet = *it;
        bullet->position.y += bullet->speed * bullet->direction.y * delta_time;
        bullet->position.x += bullet->speed * bullet->direction.x * delta_time;
        if(bullet->position.x + margin < 0 || bullet->position.x - margin > game.get_window_width()
        || bullet->position.y + margin < 0 || bullet->position.y - margin > game.get_window_height())
        {
            delete bullet;
            it = enemy_bullets.erase(it);
        }
        else{
            SDL_Rect player_rect = {static_cast<int>(player->postion.x),static_cast<int>(player->postion.y),player->width,player->height};
            SDL_Rect bullet_rect = {static_cast<int>(bullet->position.x),static_cast<int>(bullet->position.y),bullet->width,bullet->height};
            if(SDL_HasIntersection(&player_rect,&bullet_rect) && !is_dead)
            {
                Mix_PlayChannel(-1,sounds["hit"],0);
                player->current_health -= bullet->damage;
                delete bullet;
                it = enemy_bullets.erase(it);
            }
            else ++it;
        }
    }
}

void Scene_main::update_explosion()
{
    auto current_time = SDL_GetTicks();
    for(auto it = explosions.begin();it != explosions.end();)
    {
        auto explosion = *it;
        explosion->current_frame = (current_time - explosion->start_time) * explosion->FPS / 1000;
        if(explosion->current_frame > explosion->total_frame)
        {
            delete explosion;
            it = explosions.erase(it);
        }
        else ++it;
    }
}

void Scene_main::update_item(float delta_time)
{
    for(auto it = items.begin();it != items.end();)
    {
        auto item = *it;
        item->postion.x += item->speed * item->direction.x * delta_time;
        item->postion.y += item->speed * item->direction.y * delta_time;
        if(item->bounce_count > 0)
        {
            if(item->postion.x < 0 || item->postion.x + item->width > game.get_window_width())
            {
                item->direction.x = - item->direction.x;
                item->bounce_count --;
            }
            else if(item->postion.y < 0 || item->postion.y + item->height > game.get_window_height())
            {
                item->direction.y = - item->direction.y;
                item->bounce_count --;
            }
        }
        if(item->postion.x + item->width < 0 || item->postion.x > game.get_window_width() || item->postion.y + item->height < 0 || item->postion.y > game.get_window_height())
        {
            delete item;
            it = items.erase(it);
        }
        else
        {
            SDL_Rect item_rect = {static_cast<int>(item->postion.x),static_cast<int>(item->postion.y),item->width,item->height};
            SDL_Rect player_rect = {static_cast<int>(player->postion.x),static_cast<int>(player->postion.y),player->width,player->height};
            if(SDL_HasIntersection(&item_rect,&player_rect) && !is_dead)
            {
                player_get_item(item);
                delete item;
                it = items.erase(it);
            }
            else ++it;
        }
    }    
}

// else
void Scene_main::shoot()
{
    Bullet* bullet_player = new Bullet(template_bullet);//拷贝构造创建子弹，防止多次读取文件
    bullet_player->position.x = player->postion.x + player->width/2 - bullet_player->width/2;
    bullet_player->position.y = player->postion.y;
    bullets.push_back(bullet_player);
    Mix_PlayChannel(0,sounds["player_shoot"],0);
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
    Mix_PlayChannel(0,sounds["player_shoot"],0);
}
void Scene_main::shoot(Enemy* enemy)
{
    Enemy_Bullet* bullet_enemy = new Enemy_Bullet(template_enemy_bullet);
    bullet_enemy->position.x = enemy->postion.x + enemy->width/2 - bullet_enemy->width/2;
    bullet_enemy->position.y = enemy->postion.y + enemy->height;
    bullet_enemy->direction = get_direction(enemy);
    enemy_bullets.push_back(bullet_enemy);
    Mix_PlayChannel(-1,sounds["enemy_shoot"],0);
}
void Scene_main::player_get_item(Item* item)
{
    score += 50;
    if(item->type == item_type::Life)
    {
        player->current_health = player->current_health <= player->max_health ? player->current_health + 1 : player->max_health;
    }
    else if(item->type == item_type::Time)
    {
        score += 100;
    }
    else if(item->type == item_type::Shield)
    {
        player->current_health = player->current_health <= player->max_health ? player->current_health + 1 : player->max_health;
    }
    Mix_PlayChannel(-1,sounds["get_item"],0);
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
        monster->cool_down = 1500;
        monster->current_health = 3;
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
SDL_FPoint Scene_main::get_direction(Enemy* enemy)
{
    float x = (player->postion.x + player->width/2) - (enemy->postion.x + enemy->width/2);
    float y = (player->postion.y + player->height/2) - (enemy->postion.y + enemy->height/2);
    float length = sqrt(x*x+y*y);
    return SDL_FPoint{x/length,y/length};
}
void Scene_main::enemy_explode(Enemy* enemy)
{
    auto current_time = SDL_GetTicks();
    Explosion* explosion = new Explosion(template_explosion);
    explosion->postion.x = enemy->postion.x + enemy->width/2 - explosion->width/2;
    explosion->postion.y = enemy->postion.y + enemy->height/2 - explosion->height/2;
    explosion->start_time = current_time;
    explosions.push_back(explosion);
    Mix_PlayChannel(-1,sounds["enemy_explosion"],0);
    if(dis(gen) > drop_possibility)
    {
        if(dis(gen) < drop_possibility/3) item_drop(enemy,item_type::Life);
        else if(dis(gen) > drop_possibility * 2 /3) item_drop(enemy,item_type::Shield);
        else item_drop(enemy,item_type::Time);
    }
    delete enemy;
}
void Scene_main::item_drop(Enemy* enemy,item_type drop_type)
{
    Item* item = nullptr;
    if(drop_type == item_type::Life) item = new Item(template_life_item);
    else if(drop_type == item_type::Time) item = new Item(template_time_item);
    else if(drop_type == item_type::Shield) item = new Item(template_shield_item);
    item->postion.x = enemy->postion.x + enemy->width/2 - item->width/2;
    item->postion.y = enemy->postion.y + enemy->height/2 - item->height/2;
    float angle = dis(gen) * 2 * pi;
    item->direction.x = cos(angle);
    item->direction.y = sin(angle);
    items.push_back(item);
}