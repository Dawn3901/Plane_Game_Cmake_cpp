#include "Scene.h"
#include "Game.h"
#include <SDL_mixer.h>

class Game;
class Scene_end : public Scene
{
public:
    ~Scene_end() = default;
    void init() override;
    void render() override;
    void clean() override;
    void update(float) override;
    void handle_event(SDL_Event* event) override;
private:
    void render_phase_1();
    void render_phase_2();
private:
    bool is_typing = true;
    std::string name = "";
    float blink_timer = 1.0f;
    Mix_Music* bgm;
};