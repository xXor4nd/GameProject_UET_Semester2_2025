#ifndef _GAMESTATE__H
#define _GAMESTATE__H

enum GameState
{
    MENU,
    PLAY,
    GAMEMODE,
    TUTORIAL,
    EXIT,
    PAUSED,
    GAME_OVER
};

enum GameMode
{
    MODE_1_PLAYER,
    MODE_2_PLAYER
};

#endif // _GAMESTATE__H
