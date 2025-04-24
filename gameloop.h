#ifndef _GAMELOOP__H
#define _GAMELOOP__H

#include "game.h"
#include "logic.h"

void gameLoop(Game& game)
{
    while (game.currentState != EXIT)
    {
        game.sounds.playMusicforState(game.currentState);

        switch (game.currentState)
        {
            case MENU:
                resetGame(game);
                handleGameStateMenu(game);
                break;
            case PLAY:
                if (game.currentMode == MODE_1_PLAYER)
                    handleGameStatePlay1P(game);
                else
                    handleGameStatePlay2P(game);
                break;
            case GAMEMODE:
                handleGameStateGamemode(game);
                break;
            case TUTORIAL:
                handleGameStateTutorial(game);
                break;
            case PAUSED:
                handleGameStatePaused(game);
                break;
            case GAME_OVER:
                handleGameStateGameOver(game);
                break;
            default:
                break;
        }
    }
}

#endif // _GAMELOOP__H
