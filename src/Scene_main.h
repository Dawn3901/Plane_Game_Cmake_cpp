#pragma once

#include "Scene.h"

class Scene_main : public Scene
{
public:
    void init() override;
    void clean() override;
    void render() override;
    void update() override;
    void handle_event(SDL_Event* event) override;
private:

};
