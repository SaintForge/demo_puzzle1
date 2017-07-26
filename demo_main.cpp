#include "Game_Engine.h"

int SDL_main(int argc, char *argv[])
{
    Game_Engine *game = new Game_Engine();
    game->run();
    
    return 0;
}
