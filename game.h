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
#include "Assets.h"

void Game()
{
    Graphics graphics;
    graphics.init();

    Asset assets(graphics);
    assets.loadAllAssets();

    ScrollingBackground bgr(graphics, assets);
    BlueShip blueShip(graphics, assets, BLUE_SHIP_FIXED_COORDINATE_Y, SDLK_LEFT, SDLK_RIGHT);
    RedShip redShip(graphics, assets, RED_SHIP_FIXED_COORDINATE_Y, SDLK_a, SDLK_d);
    Bullet bullet(graphics, assets);

    GameState currentState = MENU;
    GameMode currentMode = MODE_1_PLAYER;

    graphics.prepareScene();

    while (currentState != EXIT)
    {
        switch (currentState)
        {
            case MENU:
                resetGame(bullet, blueShip, redShip, blueShip.healthLoss, redShip.healthLoss);
                handleGameStateMenu(graphics, assets, currentState);
                break;
            case PLAY:
                if (currentMode == MODE_1_PLAYER) handleGameStatePlay1P(graphics, bgr, blueShip, redShip, bullet, currentState);
                else handleGameStatePlay2P(graphics, bgr, blueShip, redShip, bullet, currentState);
                break;
            case GAMEMODE:
                handleGameStateGamemode(graphics, assets, currentState, currentMode);
                break;
            case TUTORIAL:
                handleGameStateTutorial(graphics, assets, currentState);
                break;
            case PAUSED:
                {
                    bool replayRequested = false;
                    handleGameStatePaused(graphics, assets, currentState, replayRequested);
                    if (replayRequested)
                    {
                        resetGame(bullet, blueShip, redShip, blueShip.healthLoss, redShip.healthLoss);
                    }
                    break;
                }
            case GAME_OVER:
                handleGameStateGameOver(graphics, assets, currentState, bullet, redShip, blueShip, currentMode);
                break;
            default:
                break;
        }
    }

    assets.clean();
    graphics.quit();
}



#endif // _GAME__H
