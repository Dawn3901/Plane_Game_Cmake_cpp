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
    void update_bullets(float);
    void render_bullets();
    void spawn_enemy();
    void update_enemies(float);
    void render_enemies();
private:
    Game& game;
    Player* player;
    std::mt19937 gen;
    std::uniform_real_distribution<float> dis;
    Bullet template_bullet;
    std::list<Bullet*> bullets;//子弹列表
    Enemy template_enemy;
    std::list<Enemy*> enemies;
};

