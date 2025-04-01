#include <iostream>
#include "defs.h"
#include "graphics.h"

using namespace std;

struct BlueShip
{
    Graphics& graphics;
    int x = 0, y = 0;
    int dx = 0, dy = 0;
    SDL_Texture* gBlueShip = graphics.loadTexture(BLUE_SHIP_IMG);

    BlueShip (Graphics& g): graphics(g){}

    void handleEvent(SDL_Event& e)
    {
        if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
        {
            switch( e.key.keysym.sym )
            {
                case SDLK_UP: dy -= SHIP_VELO; break;
                case SDLK_DOWN: dy += SHIP_VELO; break;
                case SDLK_LEFT: dx -= SHIP_VELO; break;
                case SDLK_RIGHT: dx += SHIP_VELO; break;
            }
        }

        else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
        {
            switch( e.key.keysym.sym )
            {
                case SDLK_UP: dy += SHIP_VELO; break;
                case SDLK_DOWN: dy -= SHIP_VELO; break;
                case SDLK_LEFT: dx += SHIP_VELO; break;
                case SDLK_RIGHT: dx -= SHIP_VELO; break;
            }
        }
    }

    void move()
    {
        x += dx;
        if (x < 0 || x + SHIP_WIDTH > SCREEN_WIDTH) x -= dx;

        y += dy;
        if (y < 0 || y + SHIP_HEIGHT > SCREEN_HEIGHT) y -= dy;
    }


    void render()
    {
        graphics.renderTexture(gBlueShip, x, y);
    }
};
