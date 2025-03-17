#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "graphics.h"
#include "defs.h"

using namespace std;

int main(int argc, char *argv[])
{
    Graphics graphics;
    graphics.init();

    graphics.prepareScene();

    SDL_Texture* background = graphics.loadTexture("bikiniBottom.jpg");
    graphics.renderTexture(background, 0, 0);

    SDL_Event e;
    bool quit = false;
    while(!quit)
    {
        while(SDL_PollEvent(&e))
            if(e.type == SDL_QUIT) quit = true;

        graphics.presentScene();
    }

    graphics.quit();
    return 0;
}
