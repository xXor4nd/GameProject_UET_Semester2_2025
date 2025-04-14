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
    SDL_Texture* checkedSquare = NULL;
    SDL_Texture* square = NULL;
    SDL_Texture* volume_off = NULL;
    SDL_Texture* volume_on = NULL;

    TTF_Font* font30 = NULL;
    TTF_Font* font40 = NULL;
    TTF_Font* font72 = NULL;

    Mix_Music* ingameMusic = NULL;
    Mix_Music* menuMusic = NULL;

    Mix_Chunk* clickedSound = NULL;
    Mix_Chunk* collisionSound = NULL;
    Mix_Chunk* gameoverSound = NULL;
    Mix_Chunk* hoverSound = NULL;
    Mix_Chunk* loseSound = NULL;
    Mix_Chunk* winSound = NULL;

    Asset(Graphics& g) : graphics(g) {}

    void loadAllAssets()
    {
        background = graphics.loadTexture("assets/img/background_ingame.png");
        blueShip = graphics.loadTexture("assets/img/blue_ship.png");
        redShip = graphics.loadTexture("assets/img/red_ship.png");
        bullet = graphics.loadTexture("assets/img/bullet.png");
        heart = graphics.loadTexture("assets/img/Heart.png");
        healthBar = graphics.loadTexture("assets/img/health_bar.png");
        button = graphics.loadTexture("assets/img/button.png");
        menuBackground = graphics.loadTexture("assets/img/menu_background.png");
        tutorial = graphics.loadTexture("assets/img/tutorial.png");

        font30 = TTF_OpenFont("assets/font/Purisa-BoldOblique.ttf", 30);
        font40 = TTF_OpenFont("assets/font/Purisa-BoldOblique.ttf", 40);
        font72 = TTF_OpenFont("assets/font/Purisa-BoldOblique.ttf", 72);

        ingameMusic = graphics.loadMusic("assets/sound/ingame_music.wav");
        menuMusic = graphics.loadMusic("assets/sound/menu_music.wav");

        clickedSound = graphics.loadSound("assets/sound/clicked_sound.wav");
        collisionSound = graphics.loadSound("assets/sound/collision_sound.wav");
        gameoverSound = graphics.loadSound("assets/sound/gameover_sound.wav");
        hoverSound = graphics.loadSound("assets/sound/hover_sound.wav");
        loseSound = graphics.loadSound("assets/sound/lose_sound.wav");
        winSound = graphics.loadSound("assets/sound/win_sound.wav");
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
        Mix_FreeMusic(ingameMusic);
        Mix_FreeMusic(menuMusic);
        Mix_FreeChunk(clickedSound);
        Mix_FreeChunk(collisionSound);
        Mix_FreeChunk(gameoverSound);
        Mix_FreeChunk(hoverSound);
        Mix_FreeChunk(loseSound);
        Mix_FreeChunk(winSound);
    }
};

#endif // _ASSETS__H
