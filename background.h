#ifndef _BACKGROUND__H
#define _BACKGROUND__H

#include <iostream>
#include "graphics.h"
#include "defs.h"

using namespace std;

struct ScrollingBackground
{
    Graphics& graphics;
    SDL_Texture* IngameBackground;
    int scrollingOffset = 0;

    ScrollingBackground (Graphics& g, const char* texturePath): graphics(g), IngameBackground(graphics.loadTexture(texturePath)) {}

    ~ScrollingBackground() { if(IngameBackground) SDL_DestroyTexture(IngameBackground); }

    void renderBackground (int INGAME_BACKGROUND_SCROLLING_SPEED)
    {
        scrollingOffset += INGAME_BACKGROUND_SCROLLING_SPEED;
        if(scrollingOffset > INGAME_BACKGROUND_HEIGHT)
        {
            scrollingOffset = 0;
        }

        graphics.renderTexture(IngameBackground, 0, scrollingOffset);
        graphics.renderTexture(IngameBackground, 0, scrollingOffset - INGAME_BACKGROUND_HEIGHT);
    }
};

#endif // _BACKGROUND__H
