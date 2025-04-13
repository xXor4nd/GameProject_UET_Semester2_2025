#ifndef _ASSETS__H
#define _ASSETS__H

#include "graphics.h"

struct Asset
{
    Graphics& graphics;
    SDL_Texture* background = NULL;
    SDL_Texture* blueShip = NULL;
    SDL_Texture* redShip = NULL;
    SDL_Texture* bullet = NULL;
    SDL_Texture* heart = NULL;
    SDL_Texture* healthBar = NULL;
    SDL_Texture* button = NULL;
    SDL_Texture* menuBackground = NULL;
    SDL_Texture* tutorial = NULL;

    TTF_Font* font30 = NULL;
    TTF_Font* font40 = NULL;
    TTF_Font* font72 = NULL;

    Asset(Graphics& g) : graphics(g) {}

    void loadAllAssets()
    {
        background = graphics.loadTexture("assets/background_ingame.png");
        blueShip = graphics.loadTexture("assets/blue_ship.png");
        redShip = graphics.loadTexture("assets/red_ship.png");
        bullet = graphics.loadTexture("assets/bullet.png");
        heart = graphics.loadTexture("assets/Heart.png");
        healthBar = graphics.loadTexture("assets/health_bar.png");
        button = graphics.loadTexture("assets/button.png");
        menuBackground = graphics.loadTexture("assets/menu_background.png");
        tutorial = graphics.loadTexture("assets/tutorial.png");

        font30 = TTF_OpenFont("assets/Purisa-BoldOblique.ttf", 30);
        font40 = TTF_OpenFont("assets/Purisa-BoldOblique.ttf", 40);
        font72 = TTF_OpenFont("assets/Purisa-BoldOblique.ttf", 72);
    }

    void clean()
    {
        SDL_DestroyTexture(background);
        SDL_DestroyTexture(blueShip);
        SDL_DestroyTexture(redShip);
        SDL_DestroyTexture(bullet);
        SDL_DestroyTexture(heart);
        SDL_DestroyTexture(healthBar);
        SDL_DestroyTexture(button);
        SDL_DestroyTexture(menuBackground);
        SDL_DestroyTexture(tutorial);
        TTF_CloseFont(font30);
        TTF_CloseFont(font40);
        TTF_CloseFont(font72);
    }
};

#endif // _ASSETS__H
