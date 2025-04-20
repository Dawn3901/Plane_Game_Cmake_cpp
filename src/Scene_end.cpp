#include "Scene_end.h"
#include <string>
#include <iostream>
void Scene_end::init()
{
    if(SDL_IsTextInputActive())
    {
        SDL_StartTextInput();
        if(SDL_IsTextInputActive())
        {
            SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Failed to open the text_input. SDL Error: %s\n",SDL_GetError());
        }

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
            }
            if(event->key.keysym.scancode == SDL_SCANCODE_BACKSPACE)
            {
                if(name != "") name.pop_back();
            }
        }
    }
    else
    {

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
        SDL_Point position = game.render_text_center(name,0.7f,game.get_text_font());
        //std::cout<<"x = "<<position.x<<";y = "<<position.y<<std::endl;
        if(blink_timer < 0.5f)
        {
            game.render_text(cursor,position.x,position.y,game.get_text_font());
        }
    }
    else
    {  
        if(blink_timer < 0.5f)
        {
            game.render_text_center(cursor,0.7f,game.get_text_font());
        }
    }
}
void Scene_end::render_phase_2()
{

}
