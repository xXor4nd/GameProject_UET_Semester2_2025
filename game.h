#ifndef _GAME__H
#define _GAME__H

#include <SDL.h>
#include <SDL_image.h>

#include "graphics.h"
#include "defs.h"
#include "background.h"
#include "ship_motion.h"
#include "bullet.h"
#include "time.h"

void GameLoop()
{
    Graphics graphics;
    graphics.init();

    ScrollingBackground bgr(graphics, INGAME_BACKGROUND_IMG);

    BlueShip blueShip(graphics, BLUE_SHIP_IMG, BLUE_SHIP_FIXED_COORDINATE_Y, SDLK_LEFT, SDLK_RIGHT);
    RedShip redShip(graphics, RED_SHIP_IMG, RED_SHIP_FIXED_COORDINATE_Y, SDLK_a, SDLK_d);
    Bullet bullet(graphics, BULLET_IMG);

    graphics.prepareScene();

    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) quit = true;

            blueShip.handleEvent(e);
            redShip.handleEvent(e);

//            int x, y;
//            SDL_GetMouseState(&x, &y);
//            cerr << x << ", " << y << endl;

        }

        bgr.renderBackground(INGAME_BACKGROUND_SCROLLING_SPEED);

        blueShip.move();
        blueShip.render();

        redShip.move();
        redShip.render();

        bullet.move(blueShip, redShip);
        bullet.render();

        graphics.presentScene();
    }

    graphics.quit();
}



#endif // _GAME__H
