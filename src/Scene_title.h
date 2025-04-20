#pragma once
#include "Scene.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

class Scene_title : public Scene
{
public:
    Scene_title() = default;
    ~Scene_title() = default;
    void init() override;
    void render() override;
    void clean() override;
    void update(float) override;
    void handle_event(SDL_Event* event) override;
private:
    Mix_Music* bgm;
    float timer = 0.0f;
};