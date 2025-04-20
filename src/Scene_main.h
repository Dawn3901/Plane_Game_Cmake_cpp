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
    void init() override;
    void clean() override;
    void render() override;
    void update(float) override;
    void handle_event(SDL_Event* event) override;
    void keyboard_control(float);
    void shoot();
    void double_shoot();
    void shoot(Enemy*);
    void update_player(float);
    void update_bullets(float);
    void update_enemy_bullets(float);
    void update_explosion(float);
    void render_bullets();
    void render_explosion();
    void render_ui();
    void spawn_enemy();
    void update_enemies(float);
    void render_enemies();
    void enemy_explode(Enemy*);
    SDL_FPoint get_direction(Enemy*);
    void item_drop(Enemy*,item_type);
    void update_item(float);
    void player_get_item(Item*);
    void render_item();
private:
    Game& game;
    Player* player;
    Mix_Music* bgm;
    SDL_Texture* health_ui;
    TTF_Font* score_font;
    bool is_dead = false;
    int score = 0;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;
    Bullet template_bullet;
    std::list<Bullet*> bullets;//子弹列表
    Enemy template_enemy;
    Enemy template_monster;
    std::list<Enemy*> enemies;
    std::list<Enemy*> monsters;
    Enemy_Bullet template_enemy_bullet;
    std::list<Enemy_Bullet*> enemy_bullets;
    Explosion template_explosion;
    std::list<Explosion*> explosions;
    Item template_life_item;
    Item template_time_item;
    Item template_shield_item;
    std::list<Item*> items;
    std::map<std::string,Mix_Chunk*> sounds; 
};

