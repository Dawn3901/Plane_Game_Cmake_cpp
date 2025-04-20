#pragma once
#include "Scene.h"
#include "Object.h"
#include <SDL.h>
#include <chrono>
class Game
{
public:
    static Game& get_instance(){
        static Game instance;
        return instance;
    }
    ~Game();
    void init();
    void clean();
    void run();  
    void change_scene(Scene*);
    void handle_event(SDL_Event*);
    void update_background(float);
    void render_background();
    void update(float);
    void render();
    SDL_Window* get_window() { return window;}
    SDL_Renderer* get_renderer() { return renderer;}
    int get_window_width();
    int get_window_height();
private:
    Game() = default;
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
private:
    bool Is_running = true;
    bool Is_paused = false;
    Scene* current_scene = nullptr;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    int window_width = 600;
    int window_height = 800;
    int FPS = 60;
    float frame_time;
    float delta_time;
    Background near_stars;
    Background far_stars;
};