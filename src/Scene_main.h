#pragma once
#include "Object.h"
#include "Scene.h"
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <list>
#include <random>
#include <map>

class Game;

class Scene_main : public Scene
{
public:
    Scene_main();
    ~Scene_main();
    // 基本框架
    void init() override;
    void clean() override;
    void render() override;
    void update(float) override;
    void handle_event(SDL_Event* event) override;
    void keyboard_control(float);
    void change_scene_delay(float,float);
    // render
    void render_bullets();
    void render_explosion();
    void render_ui();
    void render_enemies();
    void render_item();
    // update
    void update_player();
    void update_bullets(float);
    void update_enemy_bullets(float);
    void update_explosion();
    void update_enemies(float);
    void update_item(float);
    // else
    void shoot();
    void double_shoot();
    void shoot(Enemy*);
    void spawn_enemy();
    void enemy_explode(Enemy*);
    SDL_FPoint get_direction(Enemy*);
    void item_drop(Enemy*,item_type);
    void player_get_item(Item*);
private:
    // system
    Mix_Music* bgm;
    SDL_Texture* health_ui;
    TTF_Font* score_font;
    float timer_end = 0.0f;
    // player
    Player* player;
    int score = 0;
    bool is_dead = false;
    // random
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;
    // bullets
    Bullet template_bullet;
    std::list<Bullet*> bullets;//子弹列表
    Enemy_Bullet template_enemy_bullet;
    std::list<Enemy_Bullet*> enemy_bullets;
    // enemy
    Enemy template_enemy;
    Enemy template_monster;
    std::list<Enemy*> enemies;
    std::list<Enemy*> monsters;
    // explosion
    Explosion template_explosion;
    std::list<Explosion*> explosions;
    // item
    Item template_life_item;
    Item template_time_item;
    Item template_shield_item;
    std::list<Item*> items;
    // sounds
    std::map<std::string,Mix_Chunk*> sounds; 
};

