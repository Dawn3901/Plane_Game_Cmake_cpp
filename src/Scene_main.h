#pragma once
#include "Object.h"
#include "Scene.h"
#include <list>
#include <random>

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
    void update_bullets(float);
    void update_enemy_bullets(float);
    void render_bullets();
    void spawn_enemy();
    void update_enemies(float);
    void render_enemies();
    void enemy_explode(Enemy*);
    SDL_FPoint get_direction(Enemy*);
private:
    Game& game;
    Player* player;
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
};

