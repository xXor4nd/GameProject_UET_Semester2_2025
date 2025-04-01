#ifndef _SHIP_MOTION__H
#define _SHIP_MOTION__H

#include <iostream>
#include "defs.h"
#include "graphics.h"

using namespace std;

struct Ship
{
    Graphics& graphics;
    int x = 0, dx = 0;
    SDL_Texture* texture;
    const int fixedY;
    SDL_Keycode leftKey, rightKey;

    Ship(Graphics& g, const char* texturePath, const int yPos, SDL_Keycode left, SDL_Keycode right)
        : graphics(g), texture(graphics.loadTexture(texturePath)), fixedY(yPos), leftKey(left), rightKey(right) {}

    ~Ship() { SDL_DestroyTexture(texture);}

    void handleEvent(SDL_Event& e)
    {
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_LEFT: if (leftKey == SDLK_LEFT) dx -= SHIP_VELO; break;
                case SDLK_RIGHT: if (rightKey == SDLK_RIGHT) dx += SHIP_VELO; break;

                case SDLK_a: if (leftKey == SDLK_a) dx -= SHIP_VELO; break;
                case SDLK_d: if (rightKey == SDLK_d) dx += SHIP_VELO; break;
            }
        }
        else if (e.type == SDL_KEYUP && e.key.repeat == 0)
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_LEFT: if (leftKey == SDLK_LEFT) dx += SHIP_VELO; break;
                case SDLK_RIGHT: if (rightKey == SDLK_RIGHT) dx -= SHIP_VELO; break;

                case SDLK_a: if (leftKey == SDLK_a) dx += SHIP_VELO; break;
                case SDLK_d: if (rightKey == SDLK_d) dx -= SHIP_VELO; break;
            }
        }
    }

    void move()
    {
        x += dx;
        if (x < 0 || x + SHIP_WIDTH > SCREEN_WIDTH) x -= dx;
    }

    void render()
    {
        graphics.renderTexture(texture, x, fixedY);
    }
};




#endif // _SHIP_MOTION__H
