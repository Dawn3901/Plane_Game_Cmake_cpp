#include "Scene_title.h"
#include "Scene_main.h"
#include "Game.h"
#include <string>
void Scene_title::init()
{
    // 加载音乐
    bgm = Mix_LoadMUS("assets/music/06_Battle_in_Space_Intro.ogg");
    if(bgm == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Failed to load music! Mixer Error: %s\n",Mix_GetError());
    }
    Mix_PlayMusic(bgm,-1);
}

void Scene_title::clean()
{
    if(bgm != nullptr)
    {
        Mix_HaltMusic();
        Mix_FreeMusic(bgm);
    }
}

void Scene_title::handle_event(SDL_Event* event)
{
    if(event->type == SDL_KEYDOWN)
    {
        if(event->key.keysym.scancode == SDL_SCANCODE_J)
        {
            Scene_main* scene_main = new Scene_main();
            game.change_scene(scene_main);
        }
    }
}

void Scene_title::render()
{
    std::string title = "SDL Plane Game";
    game.render_text_center(title,0.4f,game.get_title_font());
    if(timer < 0.4f)
    {
        std::string text = "Press J to start";
        game.render_text_center(text,0.8f,game.get_text_font());
    }
}

void Scene_title::update(float delta_time)
{
    timer += delta_time;
    if(timer >= 0.8f)
    {
        timer -= 0.8f;
    }
}