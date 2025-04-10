#include "Game.h"

void Game::init()
{

}
void Game::clean()
{

}
void Game::run()
{
    while(Is_running){
        SDL_Event event;
        current_scene->handle_event(&event);
        current_scene->update();
        current_scene->render();
    }
}
void Game::change_scene(Scene* newScene)
{
    if(current_scene != nullptr)
    {
        current_scene->clean();
    }
    current_scene = newScene;
    current_scene->init();
}