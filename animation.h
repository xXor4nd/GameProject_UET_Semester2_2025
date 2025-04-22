#ifndef _ANIMATION__H
#define _ANIMATION__H

#include <iostream>
#include <vector>

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

struct Sparkle
{
    Graphics& graphics;
    Asset& assets;

    SDL_Texture* sparkleTexture;
    vector<SDL_Rect> clips;
    int currentFrame = 0;

    Sparkle(Graphics& g, Asset _assets, int frames, const int _clips [][4]): graphics(g), assets(_assets)
    {
        sparkleTexture = assets.sparkle;
        SDL_Rect clip;
        for (int i = 0; i < frames; i++)
        {
            clip.x = _clips[i][0];
            clip.y = _clips[i][1];
            clip.w = _clips[i][2];
            clip.h = _clips[i][3];
            clips.emplace_back(clip);
        }
    }

    void renderSprite(int x, int y)
    {
        SDL_Rect clip = clips[currentFrame];
        SDL_Rect renderQuad = {x, y, clip.w, clip.h};
        SDL_RenderCopy(graphics.renderer, sparkleTexture, &clip, &renderQuad);
    }

    void tick()
    {
        currentFrame = (currentFrame + 1) % (int)clips.size();
    }
};

#endif // _ANIMATION__H
