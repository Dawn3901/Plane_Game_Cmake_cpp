#pragma once
#include "Object.h"
#include "Scene.h"
#include <list>
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
private:
    Game& game;
    Player* player;
    Bullet template_bullet;
    std::list<Bullet*> bullets;//子弹列表
};

