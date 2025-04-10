#include<SDL.h>
#include<SDL_image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include "Game.h"
#include "Scene_main.h"

int main(int,char**)
{
    Game* start = new Game();
    start->init();
    start->run();
    delete start;
    return 0;
}