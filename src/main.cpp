#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include "Game.h"
#include "Scene_main.h"

int main(int,char**)
{
    //单例模式新建游戏
    Game& start = Game::get_instance();
    start.init();
    start.run();
    return 0;
}