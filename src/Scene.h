#pragma once
#include<SDL.h>

class Scene
{
public:
    Scene() = default;
    virtual ~Scene() = default;
    virtual void init() = 0;
    virtual void clean() = 0;
    virtual void render() = 0;
    virtual void update(float) = 0;
    virtual void handle_event(SDL_Event* event) = 0;
private:
    
};