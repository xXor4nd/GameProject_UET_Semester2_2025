#ifndef _BACKGROUND__H
#define _BACKGROUND__H

#include <iostream>
#include "graphics.h"
#include "defs.h"

using namespace std;

struct ScrollingBackground
{
    Graphics& graphics;
    SDL_Texture* IngameBackground = NULL;
    int scrollingOffset = 0;
    int width = 0, height = 0;

    ScrollingBackground (Graphics& g): graphics(g){}

    ~ScrollingBackground()
    {
        if (IngameBackground)
        {
            SDL_DestroyTexture(IngameBackground);
        }
    }

    void setTexture (SDL_Texture* _IngameBackground)
    {
        IngameBackground = _IngameBackground;
        SDL_QueryTexture(IngameBackground, NULL, NULL, &width, &height);
    }

    void scroll (int velo)
    {
        scrollingOffset += velo;
        if(scrollingOffset > height)
        {
            scrollingOffset = 0;
        }
    }

    void renderBackground (int INGAME_BACKGROUND_SCROLLING_SPEED)
    {
        scrollingOffset += INGAME_BACKGROUND_SCROLLING_SPEED;
        if(scrollingOffset > height)
        {
            scrollingOffset = 0;
        }

        graphics.renderTexture(IngameBackground, 0, scrollingOffset);
        graphics.renderTexture(IngameBackground, 0, scrollingOffset - height);
    }
};

#endif // _BACKGROUND__H
