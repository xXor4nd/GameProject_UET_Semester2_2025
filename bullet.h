#ifndef _BULLET__H
#define _BULLET__H

#include "graphics.h"
#include "defs.h"

using namespace std;

struct Bullet
{
    Graphics& graphics;
    SDL_Texture* texture;
    int x = 0, dx = 2;
    int y = 0, dy = 2;

    Bullet(Graphics& g, const char* texturePath ): graphics(g), texture(graphics.loadTexture(texturePath)) {}

    void move()
    {
        x += dx; y+= dy;
        if(x < 0 || x + BULLET_WIDTH > SCREEN_WIDTH) dx = -dx;
        if(y < 0 || y + BULLET_HEIGHT > SCREEN_HEIGHT) dy = -dy;
    }

    void render()
    {
        graphics.renderTexture(texture, x, y);
    }
};

#endif // _BULLET__H
