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
#include "Assets.h"
#include "sound.h"
#include "GameState.h"

struct Game
{
    Graphics& graphics;
    Asset& assets;
    Sound& sounds;

    ScrollingBackground bgr;
    Sparkle sparkle;
    BlueShip blueShip;
    RedShip redShip;
    BulletManager bulletManager;

    GameState currentState = MENU;
    GameMode currentMode = MODE_1_PLAYER;

    Game(Graphics& g, Asset& _assets, Sound& _sound):
        graphics(g), assets(_assets), sounds(_sound),
        bgr(graphics, assets),
        sparkle(graphics, assets, SPARKLE_FRAMES, SPARKLE_CLIPS),
        blueShip(graphics, assets, BLUE_SHIP_FIXED_COORDINATE_Y, SDLK_LEFT, SDLK_RIGHT),
        redShip(graphics, assets, RED_SHIP_FIXED_COORDINATE_Y, SDLK_a, SDLK_d),
        bulletManager(graphics, assets, sounds) {}
};

#endif // _GAME__H
