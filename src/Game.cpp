#include "Game.h"
#include "Scene_main.h"
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>
Game::~Game()
{
    clean();
}
void Game::init()
{
    frame_time = 1000.0f / FPS;
    // 初始化 SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"SDL could not initailize! SDL error: %s\n", SDL_GetError());
        Is_running = false;
    }
    // 初始化 窗口
    window = SDL_CreateWindow("Plane Game",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,window_width,window_height,SDL_WINDOW_SHOWN);
    if(window == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_RENDER,"Window could not be created! SDL error: %s\n",SDL_GetError());
        Is_running = false;
    }
    // 初始化 渲染器
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);
    if(renderer == nullptr)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Renderer could not be created! SDL error: %s\n",SDL_GetError());
        Is_running = false;
    }
    // 初始化 图片
    if(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != (IMG_INIT_JPG | IMG_INIT_PNG))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"IMG could not initialize! iMG error: %s\n",IMG_GetError());
        Is_running = false;
    }
    // 初始化 音频
    if(Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) != (MIX_INIT_MP3 | MIX_INIT_OGG))
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"MIX could not initialize! MIX error: %s\n",Mix_GetError());
        Is_running = false;
    }
    if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,2048) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"MIX could not initialize! MIX error: %s\n",Mix_GetError());
        Is_running = false;
    }
    Mix_AllocateChannels(16);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
    Mix_Volume(-1,MIX_MAX_VOLUME /4);
    // 初始化 文字
    if(TTF_Init() == -1)
    {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR,"TTF could not initialize! TTF error: %s\n",SDL_GetError());
        Is_running = false;
    }
    // 初始化背景卷轴
    near_stars.texture = IMG_LoadTexture(renderer,"assets/image/Stars-A.png");
    SDL_QueryTexture(near_stars.texture,NULL,NULL,&near_stars.width,&near_stars.height); 
    near_stars.width /= 2;
    near_stars.height /= 2;
    far_stars.texture = IMG_LoadTexture(renderer,"assets/image/Stars-B.png");
    SDL_QueryTexture(far_stars.texture,NULL,NULL,&far_stars.width,&far_stars.height);
    far_stars.width /= 2;
    far_stars.height /= 2;
    // 新建并初始化场景
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
    if(near_stars.texture != nullptr)
    {
        SDL_DestroyTexture(near_stars.texture);
    }
    if(far_stars.texture != nullptr)
    {
        SDL_DestroyTexture(far_stars.texture);
    }
    IMG_Quit();
     
    Mix_CloseAudio();
    Mix_Quit();

    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
void Game::run()
{
    while(Is_running){
        auto frame_start = std::chrono::high_resolution_clock::now();
        SDL_Event event;
        handle_event(&event);
        update(delta_time);
        render();
        // 使用帧率控制
        auto frame_end = std::chrono::high_resolution_clock::now();
        float duration = std::chrono::duration_cast<std::chrono::milliseconds>(frame_end  - frame_start).count();
        if(duration < frame_time)
        {
            SDL_Delay(frame_time - duration);
            delta_time = frame_time / 1000.0f;
        }
        else{
            delta_time = duration / 1000.0f;
        }
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
void Game::update(float delta_time)
{
    update_background(delta_time);
    current_scene->update(delta_time);
}
void Game::render()
{
    SDL_RenderClear(renderer);
    render_background();
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
void Game::update_background(float delta_time)
{
    near_stars.offset += near_stars.speed * delta_time;
    if(near_stars.offset >= 0)
    {
        near_stars.offset -= near_stars.height;
    }

    far_stars.offset += far_stars.speed * delta_time;
    if(far_stars.offset >= 0)
    {
        far_stars.offset -= far_stars.height;
    }
}
void Game::render_background()
{
    for(int y = static_cast<int>(far_stars.offset);y < get_window_height(); y += far_stars.height)
    {
        for(int x = 0;x < get_window_width();x += far_stars.width)
        {
            SDL_Rect dst_rect = {x,y,far_stars.width,far_stars.height};
            SDL_RenderCopy(renderer,far_stars.texture,nullptr,&dst_rect);
        }
    }
    for(int y = static_cast<int>(near_stars.offset);y < get_window_height(); y += near_stars.height)
    {
        for(int x = 0;x < get_window_width();x += near_stars.width)
        {
            SDL_Rect dst_rect = {x,y,near_stars.width,near_stars.height};
            SDL_RenderCopy(renderer,near_stars.texture,nullptr,&dst_rect);
        }
    }
}