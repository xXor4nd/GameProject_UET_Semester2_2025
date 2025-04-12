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
    Graphics graphics;
    graphics.init();

    ScrollingBackground bgr(graphics, INGAME_BACKGROUND_IMG);
    BlueShip blueShip(graphics, BLUE_SHIP_FIXED_COORDINATE_Y, SDLK_LEFT, SDLK_RIGHT);
    RedShip redShip(graphics, RED_SHIP_FIXED_COORDINATE_Y, SDLK_a, SDLK_d);
    Bullet bullet(graphics, BULLET_IMG);

    GameState currentState = MENU;
    GameMode currentMode = MODE_1_PLAYER;

    graphics.prepareScene();

    while (currentState != EXIT)
    {
        switch (currentState)
        {
            case MENU:
                resetGame(bullet, blueShip, redShip, blueShip.healthLoss, redShip.healthLoss);
                handleGameStateMenu(graphics, currentState);
                break;
            case PLAY:
                if (currentMode == MODE_1_PLAYER) handleGameStatePlay1P(graphics, bgr, blueShip, redShip, bullet, currentState);
                else handleGameStatePlay2P(graphics, bgr, blueShip, redShip, bullet, currentState);
                break;
            case GAMEMODE:
                handleGameStateGamemode(graphics, currentState, currentMode);
                break;
            case TUTORIAL:
                handleGameStateTutorial(graphics, currentState);
                break;
            case PAUSED:
                {
                    bool replayRequested = false;
                    handleGameStatePaused(graphics, currentState, replayRequested);
                    if (replayRequested)
                    {
                        resetGame(bullet, blueShip, redShip, blueShip.healthLoss, redShip.healthLoss);
                    }
                    break;
                }
            default:
                break;
        }
    }
    graphics.quit();
}



#endif // _GAME__H
