#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

#include "graphics.h"
#include "defs.h"
#include "background.h"
#include "ship_motion.h"

using namespace std;

int main(int argc, char *argv[])
{
    Graphics graphics;
    graphics.init();

    ScrollingBackground bgr(graphics);
    bgr.setTexture(graphics.loadTexture(INGAME_BACKGROUND_IMG));

    BlueShip blueship (graphics);

    graphics.prepareScene();

    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) quit = true;

            blueship.handleEvent(e);
        }

        bgr.renderBackground(INGAME_BACKGROUND_SCROLLING_SPEED);

        blueship.move();
        blueship.render();

        graphics.presentScene();
    }

    graphics.quit();
    return 0;
}
