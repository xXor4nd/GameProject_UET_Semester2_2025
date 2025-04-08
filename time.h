#ifndef _TIME__H
#define _TIME__H

#include <SDL_ttf.h>
#include <iostream>
#include <sstream>
#include <string>
#include "graphics.h"
#include "defs.h"

using namespace std;

int handleTimeInterval(Graphics& graphics)
{
    TTF_Font* font = graphics.loadFont("assets/Purisa-BoldOblique.ttf", 72);
    SDL_Color color = {255, 255, 255, 100};

    int currentTime = SDL_GetTicks();
    int timeLeft = countdownTime - (currentTime - startTime);

    if (timeLeft > 0)
    {
        int secondLeft = (timeLeft / 1000) + 1;

        stringstream timeText;
        timeText << secondLeft;

        SDL_Texture* timeTexture = graphics.renderText(timeText.str().c_str(), font, color);
        int textW = 0, textH = 0;
        SDL_QueryTexture(timeTexture, NULL, NULL, &textW, &textH);
        graphics.renderTexture(timeTexture, (SCREEN_WIDTH - textW) / 2, (SCREEN_HEIGHT - textH) / 2);

        SDL_DestroyTexture(timeTexture);
    }
    else timeLeft = 0;
    return timeLeft;
}

#endif // _TIME__H
