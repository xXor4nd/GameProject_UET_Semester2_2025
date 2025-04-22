#ifndef _GAME__H
#define _GAME__H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <ctime>
#include <cstdlib>

#include "graphics.h"
#include "defs.h"
#include "animation.h"
#include "ship_motion.h"
#include "bullet.h"
#include "time.h"
#include "menu.h"
#include "Assets.h"
#include "sound.h"
#include "GameState.h"

void gameLoop(Graphics& graphics, Asset& assets, Sound& sounds, ScrollingBackground& bgr, Sparkle& sparkle, BlueShip& blueShip, RedShip& redShip, BulletManager& bulletManager)
{
    GameState currentState = MENU;
    GameMode currentMode = MODE_1_PLAYER;

    while (currentState != EXIT)
    {
        sounds.playMusicforState(currentState);

        switch (currentState)
        {
            case MENU:
                resetGame(bulletManager, blueShip, redShip, blueShip.healthLoss, redShip.healthLoss);
                handleGameStateMenu(graphics, assets, sounds, currentState, sparkle);
                break;
            case PLAY:
                if (currentMode == MODE_1_PLAYER)
                    handleGameStatePlay1P(graphics, assets, bgr, blueShip, redShip, bulletManager, currentState);
                else
                    handleGameStatePlay2P(graphics, assets, bgr, blueShip, redShip, bulletManager, currentState);
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
                    handleGameStatePaused(graphics, assets, sounds, currentState, replayRequested);
                    if (replayRequested)
                    {
                        resetGame(bulletManager, blueShip, redShip, blueShip.healthLoss, redShip.healthLoss);
                    }
                    break;
                }
            case GAME_OVER:
                handleGameStateGameOver(graphics, assets, currentState, bulletManager, redShip, blueShip, currentMode);
                break;
            default:
                break;
        }
    }
}

#endif // _GAME__H
