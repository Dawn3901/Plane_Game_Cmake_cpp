#pragma once
#include "Scene.h"
#include "Object.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <fstream>
#include <chrono>
#include <map>
class Game
{
public:
    static Game& get_instance(){
        static Game instance;
        return instance;
    }
    ~Game();
    // 基本框架
    void init();
    void clean();
    void run();  
    void change_scene(Scene*);
    void handle_event(SDL_Event*);
    // render
    SDL_Point render_text_center(std::string&,float,TTF_Font*);
    SDL_Point render_text(std::string&,int,int,TTF_Font*,bool is_left = true);
    void render_background();
    void render();
    // update
    void update(float);
    void update_background(float);
    // get
    SDL_Window* get_window() { return window;}
    SDL_Renderer* get_renderer() { return renderer;}
    TTF_Font* get_title_font() { return title_font;}
    TTF_Font* get_text_font() { return text_font;}
    int get_window_width();
    int get_window_height();
    int get_final_score() {return final_score;}
    // set
    void set_final_score(int score) {final_score = score;}
    // leader_board
    void insert_leader_board(int,std::string&);
    std::multimap<int,std::string,std::greater<int>>& get_leader_board() {return leader_board;}
    void save_data();
    void load_data();
    // else
private:
    Game() = default;
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;
private:
    bool Is_running = true;
    bool Is_paused = false;
    bool Is_fullscreen = false;
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
    TTF_Font* title_font;
    TTF_Font* text_font;
    int final_score = 0;
    std::multimap<int,std::string,std::greater<int>> leader_board;
};