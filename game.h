#ifndef _GAME__H
#define _GAME__H

#include <SDL.h>
#include <SDL_image.h>
#include <ctime>
#include <cstdlib>

#include "graphics.h"
#include "defs.h"
#include "background.h"
#include "ship_motion.h"
#include "bullet.h"
#include "time.h"
#include "menu.h"

void Game()
{
    srand(time(NULL));

    Graphics graphics;
    graphics.init();

    ScrollingBackground bgr(graphics, INGAME_BACKGROUND_IMG);
    BlueShip blueShip(graphics, BLUE_SHIP_FIXED_COORDINATE_Y, SDLK_LEFT, SDLK_RIGHT);
    RedShip redShip(graphics, RED_SHIP_FIXED_COORDINATE_Y, SDLK_a, SDLK_d);
    Bullet bullet(graphics, BULLET_IMG);

    GameState currentState = MENU;

    graphics.prepareScene();

    while (currentState != EXIT)
    {
        switch (currentState)
        {
            case MENU:
                handleGameStateMenu(graphics, currentState);
                break;
            case PLAY:
                handleGameStatePlay(graphics, bgr, blueShip, redShip, bullet, currentState);
                break;
            case GAMEMODE:
                handleGameStateGamemode(currentState);
                break;
            case TUTORIAL:
                handleGameStateTutorial(currentState);
                break;
            default:
                break;
        }
    }

    handleGameStateExit(graphics);
    graphics.quit();
}



#endif // _GAME__H
