#pragma once
#include "Scene.h"
class Game
{
public:
    Game() = default;
    ~Game() = default;
    void init();
    void clean();
    void run();  
    void change_scene(Scene* newScene);
private:
    bool Is_running;
    Scene* current_scene = nullptr;
};