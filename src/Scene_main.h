#pragma once
#include "Object.h"
#include "Scene.h"

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
private:
    Game& game;
    Player* player;
};
