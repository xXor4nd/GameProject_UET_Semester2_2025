#ifndef _BACKGROUND__H
#define _BACKGROUND__H

#include <iostream>
#include "graphics.h"
#include "defs.h"
#include "Assets.h"

using namespace std;

struct ScrollingBackground
{
    Graphics& graphics;
    Asset& assets;
    SDL_Texture* IngameBackground = NULL;
    int scrollingOffset = 0;

    ScrollingBackground (Graphics& g, Asset& _assets): graphics(g), assets(_assets)
    {
        IngameBackground = assets.background;
    }

    ~ScrollingBackground() {}

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
