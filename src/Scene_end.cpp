#include "Scene_end.h"
#include "Scene_main.h"
#include <string>
#include <iostream>
void Scene_end::init()
{
    bgm = Mix_LoadMUS("assets/music/06_Battle_in_Space_Intro.ogg");
    if(bgm != nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Failed to load music. Mixer Error: %s\n",Mix_GetError());
    }
    Mix_PlayMusic(bgm,-1);
    if(!SDL_IsTextInputActive())
    {
        SDL_StartTextInput();
        if(SDL_IsTextInputActive())
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Failed to open the text_input. SDL Error: %s\n",SDL_GetError());
        }
        is_typing = true;
    }
}
void Scene_end::render()
{
    if(is_typing)
    {
        render_phase_1();
    }
    else 
    {
        render_phase_2();
    }
}
void Scene_end::clean()
{
    if(bgm != nullptr)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
    }
}
void Scene_end::update(float delta_time)
{
    blink_timer -= delta_time;
    if(blink_timer <= 0)
    {
        blink_timer += 1.0f;
    }
}
void Scene_end::handle_event(SDL_Event* event)
{
    if(is_typing)
    {
        if(event->type == SDL_TEXTINPUT)
        {
            name += event->text.text;
        }
        if(event->type == SDL_KEYDOWN)
        {
            if(event->key.keysym.scancode == SDL_SCANCODE_RETURN)
            {
                is_typing = false;
                SDL_StopTextInput();
                if(name == "")
                {
                    name = "anonymous";
                }
                game.insert_leader_board(game.get_final_score(),name);
            }
            if(event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
            {
                if(name != "") name.pop_back();
            }
        }
    }
    else
    {
        if(event->type == SDL_KEYDOWN)
        {
            if(event->key.keysym.scancode == SDL_SCANCODE_L)
            {
                auto scene_main = new Scene_main();
                game.change_scene(scene_main);
            }
        }
    }
}
void Scene_end::render_phase_1()
{
    std::string score_text = "YOUR SCORE IS : " + std::to_string(game.get_final_score());
    std::string game_over_text = "GAME OVER";
    std::string instruction = "Please type your name:";
    std::string cursor = "_";
    game.render_text_center(score_text,0.2f,game.get_text_font());
    game.render_text_center(game_over_text,0.5f,game.get_text_font());
    game.render_text_center(instruction,0.6f,game.get_text_font());
    if(name != "")
    {
        SDL_Point position = game.render_text_center(name,0.75f,game.get_text_font());
        if(blink_timer < 0.5f)
        {
            game.render_text(cursor,position.x,position.y,game.get_text_font());
        }
    }
    else
    {  
        if(blink_timer < 0.5f)
        {
            game.render_text_center(cursor,0.75f,game.get_text_font());
        }
    }
}
void Scene_end::render_phase_2()
{
    std::string leader_board = "leader_board";
    game.render_text_center(leader_board,0.1f,game.get_title_font());
    int index = 1;
    float y = 0.2f * game.get_window_height();
    for(auto item : game.get_leader_board())
    {
        std::string name = std::to_string(index) + ". " + item.second;
        std::string score = std::to_string(item.first);
        game.render_text(name,150,y,game.get_text_font());
        game.render_text(score,150,y,game.get_text_font(),false);
        y += 50;
        ++index;
    }
    std::string end = "Press L to restart";
    if(blink_timer < 0.5f)
    {
        game.render_text_center(end,1.0f,game.get_text_font());
    }
}
