#ifndef _MENU__H
#define _MENU__H

#include "graphics.h"
#include "background.h"
#include "ship_motion.h"
#include "bullet.h"
#include "Assets.h"
#include "sound.h"
#include "GameState.h"
#include "defs.h"

struct Button
{
    Graphics& graphics;
    Asset& assets;

    SDL_Rect rect;
    const char* label;
    SDL_Texture* buttonIMG = NULL;
    TTF_Font* font;

    bool isWithin = false;
    bool isSelected = false;

    Button (Graphics& g, Asset& _assets) : graphics(g), assets(_assets)
    {
        buttonIMG = assets.button;
        font = assets.font30;
    }

    void render()
    {
        SDL_Color color;
        if (isSelected || isWithin) color = {0, 255, 100};
        else color = {255, 255, 255};

        SDL_SetRenderDrawColor(graphics.renderer, 50, 50, 80, 255);
        SDL_RenderFillRect(graphics.renderer, &rect);
        graphics.renderTexture(buttonIMG, rect.x, rect.y);
        SDL_Texture* buttonText = graphics.renderText(label, font, color);

        int textW, textH;
        SDL_QueryTexture(buttonText, NULL, NULL, &textW, &textH);
        int textX = rect.x + (rect.w - textW) / 2;
        int textY = rect.y + (rect.h - textH) / 2;
        graphics.renderTexture(buttonText, textX, textY);

        SDL_DestroyTexture(buttonText);
    }

    bool isInside(int x, int y)
    {
        return (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h);
    }

};

void resetGame(BulletManager& bulletManager, BlueShip& blueShip, RedShip& redShip, int& blueHealthLoss, int& redHealthLoss)
{
    bulletManager.resetRound(blueShip, redShip);

    blueShip.x = 0;
    redShip.x = 0;

    blueShip.dx = 0;
    redShip.dx = 0;

    blueHealthLoss = 0;
    redHealthLoss = 0;

    blueShip.isGameOver = false;
    redShip.isGameOver = false;

    bulletManager.startTime = SDL_GetTicks();
}

void handleGameStateMenu(Graphics& graphics, Asset& assets, Sound& sounds, GameState& currentState)
{
    SDL_Texture* menuBackground = assets.menuBackground;
    SDL_Rect soundButton = { 10, SCREEN_HEIGHT - 34, 24, 24 };

    Button buttons[4] = { Button(graphics, assets), Button(graphics, assets), Button(graphics, assets), Button(graphics, assets) };
    const char* labels[4] = {"PLAY", "GAMEMODE", "TUTORIAL", "EXIT"};

    for (int i = 0; i < 4; i++)
    {
        buttons[i].label = labels[i];
        buttons[i].rect = {BUTTON_COORDINATE_X, BUTTON_COORDINATE_Y + i * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT};
    }

    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                currentState = EXIT;
                quit = true;
            }


            else if (e.type == SDL_MOUSEMOTION)
            {
                int mx = e.motion.x, my = e.motion.y;
                for (auto& btn : buttons)
                {
                    bool prevState = btn.isWithin;
                    btn.isWithin = btn.isInside(mx, my);

                    if (!prevState && btn.isWithin)
                    {
                        graphics.play(assets.hoverSound);
                    }
                }
            }

            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                for (int i = 0; i < 4; ++i)
                {
                    if (buttons[i].isWithin)
                    {
                        switch (i)
                        {
                            case 0: currentState = PLAY; graphics.play(assets.clickedSound); break;
                            case 1: currentState = GAMEMODE; graphics.play(assets.clickedSound); break;
                            case 2: currentState = TUTORIAL; graphics.play(assets.clickedSound); break;
                            case 3: currentState = EXIT; graphics.play(assets.clickedSound); break;
                        }
                        quit = true;
                    }
                }

                int mx = e.button.x, my = e.button.y;
                if (mx >= soundButton.x && mx <= soundButton.x + soundButton.w && my >= soundButton.y && my <= soundButton.y + soundButton.h)
                {
                    sounds.toggleMuteMenuMusic();
                }
            }
        }

        graphics.renderTexture(menuBackground, 0, 0);
        for (int i = 0; i < 4; i++)
        {
            buttons[i].render();
        }

        if (sounds.isMuted_menuMusic) graphics.renderTexture(assets.volume_off, soundButton.x, soundButton.y);
        else graphics.renderTexture(assets.volume_on, soundButton.x, soundButton.y);

        graphics.presentScene();
    }
}

void handleGameStatePlay2P(Graphics& graphics, Asset& assets, ScrollingBackground& bgr, BlueShip& blueShip, RedShip& redShip, BulletManager& bulletManager, GameState& currentState)
{
    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                currentState = EXIT;
                quit = true;
            }

            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                currentState = PAUSED;
                quit = true;
            }

            blueShip.handleEvent(e);
            redShip.handleEvent(e);

//            int x, y;
//            SDL_GetMouseState(&x, &y);
//            cerr << x << ", " << y << endl;

        }

        if (blueShip.isGameOver)
        {
            graphics.play(assets.gameoverSound);
            SDL_Delay(1500);
            currentState = GAME_OVER;
            quit = true;
        }
        else if (redShip.isGameOver)
        {
            graphics.play(assets.gameoverSound);
            SDL_Delay(1500);
            currentState = GAME_OVER;
            quit = true;
        }

        bgr.renderBackground(INGAME_BACKGROUND_SCROLLING_SPEED);

        blueShip.move();
        blueShip.render();

        redShip.move();
        redShip.render();

        bulletManager.handleLogic(blueShip, redShip);
        bulletManager.render(blueShip, redShip);

        graphics.presentScene();
    }
}

void handleGameStatePlay1P(Graphics& graphics, Asset& assets, ScrollingBackground& bgr, BlueShip& blueShip, RedShip& redShip, BulletManager& bulletManager, GameState& currentState)
{
    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                currentState = EXIT;
                quit = true;
            }

            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                currentState = PAUSED;
                quit = true;
            }

            redShip.handleEvent(e);
        }

        int blueCenterX = blueShip.x + SHIP_WIDTH / 2;
        int blueCenterY = blueShip.y + SHIP_HEIGHT / 2;

        float minDist = 1e9;
        float targetX = -1;

        Bullet* bullets[] = { &bulletManager.bullet1, &bulletManager.bullet2 };
        for (auto b : bullets)
        {
            if (!b->roundEnded && b->dy < 0)
            {
                float bulletCenterX = b->x + BULLET_WIDTH / 2;
                float bulletCenterY = b->y + BULLET_HEIGHT / 2;

                float dist = sqrt((bulletCenterX - blueCenterX) * (bulletCenterX - blueCenterX) + (bulletCenterY - blueCenterY) *  (bulletCenterY - blueCenterY) );

                if (dist < minDist)
                {
                    minDist = dist;
                    targetX = bulletCenterX;
                }
            }
        }
        if (targetX != -1)
        {
            if (targetX < blueCenterX - DEADZONE_X)
                blueShip.dx = -SHIP_VELO;
            else if (targetX > blueCenterX + DEADZONE_X)
                blueShip.dx = SHIP_VELO;
            else
                blueShip.dx = 0;
        }
        else
        {
            blueShip.dx = 0;
        }

        if (blueShip.isGameOver)
        {
            graphics.play(assets.gameoverSound);
            SDL_Delay(1500);
            currentState = GAME_OVER;
            quit = true;
        }
        else if (redShip.isGameOver)
        {
            graphics.play(assets.gameoverSound);
            SDL_Delay(1500);
            currentState = GAME_OVER;
            quit = true;
        }

        bgr.renderBackground(INGAME_BACKGROUND_SCROLLING_SPEED);

        blueShip.move();
        blueShip.render();

        redShip.move();
        redShip.render();

        bulletManager.handleLogic(blueShip, redShip);
        bulletManager.render(blueShip, redShip);

        graphics.presentScene();
    }
}


void handleGameStateGamemode(Graphics& graphics, Asset& assets, GameState& currentState, GameMode& currentMode)
{
    SDL_Texture* bgr = assets.background;

    Button buttons[3] = { Button(graphics, assets), Button(graphics, assets), Button(graphics, assets) };
    const char* labels[3] = {"1 PLAYER", "2 PLAYERS", "BACK"};

    for (int i = 0; i < 3; i++)
    {
        buttons[i].label = labels[i];
        buttons[i].rect = {BUTTON_COORDINATE_X, 289 + i * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT};
    }

    SDL_Event e;
    bool quit = false;
    int selectedIndex = (currentMode == MODE_1_PLAYER ? 0 : 1);

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                currentState = EXIT;
                quit = true;
            }

            else if (e.type == SDL_MOUSEMOTION)
            {
                int mx = e.motion.x, my = e.motion.y;
                for (auto& btn : buttons)
                {
                    bool prevState = btn.isWithin;
                    btn.isWithin = btn.isInside(mx, my);

                    if (!prevState && btn.isWithin)
                    {
                        graphics.play(assets.hoverSound);
                    }
                }
            }

            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                for (int i = 0; i < 3; i++)
                {
                    if (buttons[i].isWithin)
                    {
                        if (i == 2)
                        {
                            graphics.play(assets.clickedSound);
                            currentState = MENU;
                            quit = true;
                        }
                        else
                        {
                            selectedIndex = i;
                            graphics.play(assets.clickedSound);
                            currentMode = (i == 0) ? MODE_1_PLAYER : MODE_2_PLAYER;
                            quit = true;
                        }
                    }
                }
            }
        }

        graphics.renderTexture(bgr, 0, 0);
        for (int i = 0; i < 3; i++)
        {
            buttons[i].isSelected = (i == selectedIndex);
            buttons[i].render();
        }
        graphics.presentScene();
    }
}

void handleGameStatePaused(Graphics& graphics, Asset& assets, Sound& sounds, GameState& currentState, bool& replayRequested)
{
    SDL_Texture* bgr = assets.background;

    Button buttons[3] = { Button(graphics, assets), Button(graphics, assets), Button(graphics, assets) };
    const char* labels[3] = {"RESUME", "REPLAY", "MENU"};

    for (int i = 0; i < 3; i++)
    {
        buttons[i].label = labels[i];
        buttons[i].rect = {BUTTON_COORDINATE_X, 289 + i * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT};
    }

    SDL_Rect bgmBox = {65, 140, 24, 24};
    SDL_Rect collisionBox = {65, 190, 24, 24};
    SDL_Rect pointBox = {65, 240, 24, 24};

    SDL_Event e;
    bool quit = false;

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                currentState = EXIT;
                quit = true;
            }

            else if (e.type == SDL_MOUSEMOTION)
            {
                int mx = e.motion.x, my = e.motion.y;
                for (auto& btn : buttons)
                {
                    bool prevState = btn.isWithin;
                    btn.isWithin = btn.isInside(mx, my);
                    if (!prevState && btn.isWithin)
                        graphics.play(assets.hoverSound);
                }
            }

            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                int mx = e.button.x, my = e.button.y;
                if (mx >= bgmBox.x && mx <= bgmBox.x + bgmBox.w && my >= bgmBox.y && my <= bgmBox.y + bgmBox.h)
                {
                    sounds.bgmMuted = !sounds.bgmMuted;
                    sounds.updateVolume();
                }
                else if (mx >= collisionBox.x && mx <= collisionBox.x + collisionBox.w && my >= collisionBox.y && my <= collisionBox.y + collisionBox.h)
                {
                    sounds.collisionMuted = !sounds.collisionMuted;
                }
                else if (mx >= pointBox.x && mx <= pointBox.x + pointBox.w && my >= pointBox.y && my <= pointBox.y + pointBox.h)
                {
                    sounds.pointMuted = !sounds.pointMuted;
                }
                else
                {
                    for (int i = 0; i < 3; i++)
                    {
                        if (buttons[i].isWithin)
                        {
                            graphics.play(assets.clickedSound);
                            switch (i)
                            {
                                case 0:
                                    currentState = PLAY;
                                    replayRequested = false;
                                    break;
                                case 1:
                                    currentState = PLAY;
                                    replayRequested = true;
                                    sounds.stopMusic();
                                    break;
                                case 2:
                                    currentState = MENU;
                                    replayRequested = false;
                                    break;
                            }
                            quit = true;
                        }
                    }
                }
            }
        }

        graphics.renderTexture(bgr, 0, 0);
        for (int i = 0; i < 3; i++)
        {
            buttons[i].render();
        }

        TTF_Font* font = assets.font20;
        SDL_Color white = {255, 255, 255};
        SDL_Texture* text1 = graphics.renderText("Background Music", font, white);
        SDL_Texture* text2 = graphics.renderText("Collision Sound", font, white);
        SDL_Texture* text3 = graphics.renderText("GamePoint Sound", font, white);

        graphics.renderTexture(text1, 100, 140);
        graphics.renderTexture(text2, 100, 190);
        graphics.renderTexture(text3, 100, 240);

        graphics.renderTexture(sounds.bgmMuted ? assets.uncheck_square : assets.checked_square, bgmBox.x, bgmBox.y);
        graphics.renderTexture(sounds.collisionMuted ? assets.uncheck_square : assets.checked_square, collisionBox.x, collisionBox.y);
        graphics.renderTexture(sounds.pointMuted ? assets.uncheck_square : assets.checked_square, pointBox.x, pointBox.y);

        TTF_Font* titleFont = assets.font40;
        SDL_Texture* titleText = graphics.renderText("GAME PAUSED", titleFont, white);
        int textW, textH;
        SDL_QueryTexture(titleText, NULL, NULL, &textW, &textH);
        graphics.renderTexture(titleText, SCREEN_WIDTH/2 - textW/2, 60);

        graphics.presentScene();

        SDL_DestroyTexture(titleText);
        SDL_DestroyTexture(text1);
        SDL_DestroyTexture(text2);
        SDL_DestroyTexture(text3);
    }
}


void handleGameStateTutorial(Graphics& graphics, Asset& assets, GameState& currentState)
{
    SDL_Texture* bgr = assets.background;
    SDL_Texture* tutorialIMG = assets.tutorial;

    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                currentState = EXIT;
                quit = true;
            }

            else if ( e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_RETURN) )
            {
                currentState = MENU;
                quit = true;
            }
        }

        graphics.renderTexture(bgr, 0, 0);
        graphics.renderTexture(tutorialIMG, 0, 235);
        graphics.presentScene();
    }
}

void handleGameStateGameOver(Graphics& graphics, Asset& assets, GameState& currentState, BulletManager& bulletManager, RedShip& redShip, BlueShip& blueShip, GameMode& currentMode)
{
    SDL_Texture* bgr = assets.background;

    Button buttons[2] = { Button(graphics, assets), Button(graphics, assets) };
    const char* labels[2] = {"REPLAY", "MENU"};

    for (int i = 0; i < 2; i++)
    {
        buttons[i].label = labels[i];
        buttons[i].rect = {BUTTON_COORDINATE_X, 318 + i * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT};
    }

    SDL_Event e;
    bool quit = false;
    bool isPlayed = false;


    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                currentState = EXIT;
                quit = true;
            }

            else if (e.type == SDL_MOUSEMOTION)
            {
                int mx = e.motion.x, my = e.motion.y;
                for (auto& btn : buttons)
                {
                    bool prevState = btn.isWithin;
                    btn.isWithin = btn.isInside(mx, my);

                    if (!prevState && btn.isWithin)
                    {
                        graphics.play(assets.hoverSound);
                    }
                }
            }

            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                for (int i = 0; i < 2; i++)
                {
                    if (buttons[i].isWithin)
                    {
                        switch (i)
                        {
                            case 0:
                                graphics.play(assets.clickedSound);
                                currentState = PLAY;
                                resetGame(bulletManager, blueShip, redShip, blueShip.healthLoss, redShip.healthLoss);
                                break;
                            case 1:
                                graphics.play(assets.clickedSound);
                                currentState = MENU;
                                break;
                        }
                        quit = true;
                    }
                }
            }
        }

        graphics.renderTexture(bgr, 0, 0);
        for (int i = 0; i < 2; i++)
        {
            buttons[i].render();
        }

        TTF_Font* titleFont = assets.font40;
        SDL_Color color = {255, 255, 255};
        SDL_Texture* titleText = graphics.renderText("GAME OVER!", titleFont, color);

        int textW, textH;
        SDL_QueryTexture(titleText, NULL, NULL, &textW, &textH);
        graphics.renderTexture(titleText, SCREEN_WIDTH/2 - textW/2, 200);

        const char* GameOverText;
        if (currentMode == MODE_2_PLAYER)
        {
            if (redShip.isGameOver)
            {
                GameOverText = "BLUE WON!";
                color = {0, 0, 255};
            }
            else if (blueShip.isGameOver)
            {
                GameOverText = "RED WON!";
                color = {255, 0, 0};
            }
            // else GameOverText = "???";
        }

        else
        {
            if (redShip.isGameOver)
            {
                GameOverText = "YOU LOSE :(";
                color = {255, 172, 28};
            }
            else if (blueShip.isGameOver)
            {
                GameOverText = "YOU WIN!";
                color = {255, 172, 28};
            }
            // else GameOverText = "???";
        }


        if (!isPlayed)
        {
            if (currentMode == MODE_2_PLAYER)
            {
                if (redShip.isGameOver)
                {
                    graphics.play(assets.winSound);
                }
                else if (blueShip.isGameOver)
                {
                    graphics.play(assets.winSound);
                }
            }
            else
            {
                if (redShip.isGameOver)
                {
                    graphics.play(assets.loseSound);
                }
                else if (blueShip.isGameOver)
                {
                    graphics.play(assets.winSound);
                }
            }
            isPlayed = true;
        }

        SDL_Texture* winningText = graphics.renderText(GameOverText, titleFont, color);
        int winW, winH;
        SDL_QueryTexture(winningText, NULL, NULL, &winW, &winH);
        graphics.renderTexture(winningText, SCREEN_WIDTH/2 - winW/2, 500);

        graphics.presentScene();

        SDL_DestroyTexture(titleText);
        SDL_DestroyTexture(winningText);
    }
}

#endif // _MENU__H
