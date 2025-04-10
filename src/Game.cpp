#include "Game.h"
#include "Scene_main.h"
#include <SDL_image.h>
Game::~Game()
{
    clean();
}
void Game::init()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"SDL could not initailize! SDL error: %s\n", SDL_GetError());
        Is_running = false;
    }
    window = SDL_CreateWindow("Plane Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,window_width,window_height,SDL_WINDOW_SHOWN);
    if(window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER,"Window could not be created! SDL error: %s\n",SDL_GetError());
        Is_running = false;
    }
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Renderer could not be created! SDL error: %s\n",SDL_GetError());
        Is_running = false;
    }
    if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"IMG could not initialize! iMG error: %s\n",IMG_GetError());
        Is_running = false;
    }
    current_scene = new Scene_main();
    current_scene->init();
}
void Game::clean()
{
    if(current_scene != nullptr)
    {
        current_scene->clean();
        delete current_scene;
    }
    IMG_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void Game::run()
{
    while(Is_running){
        SDL_Event event;
        handle_event(&event);
        update();
        render();
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
void Game::handle_event(SDL_Event* event)
{
    while(SDL_PollEvent(event))
    {
        if(event->type == SDL_QUIT)
        {
            Is_running = false;
        }
        current_scene->handle_event(event);
    }
}
void Game::update()
{
    current_scene->update();
}
void Game::render()
{
    SDL_RenderClear(renderer);
    current_scene->render();
    SDL_RenderPresent(renderer);
}
int Game::get_window_width()
{
    return window_width;
}
int Game::get_window_height()
{
    return window_height;
}
