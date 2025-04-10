#pragma once
#include "Scene.h"
#include <SDL.h>
class Game
{
public:
    Game() = default;
    ~Game();
    void init();
    void clean();
    void run();  
    void change_scene(Scene* newScene);
    void handle_event(SDL_Event* event);
    void update();
    void render();
private:
    bool Is_running = true;
    Scene* current_scene = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int window_width = 600;
    int window_height = 800;
};