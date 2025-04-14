#ifndef _MENU__H
#define _MENU__H

#include "graphics.h"
#include "background.h"
#include "ship_motion.h"
#include "bullet.h"
#include "Assets.h"
#include "sound.h"
#include "GameState.h"

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

void resetGame(Bullet& bullet, BlueShip& blueShip, RedShip& redShip, int& blueHealthLoss, int& redHealthLoss)
{
    bullet.x = (SCREEN_WIDTH - BULLET_WIDTH) / 2;
    bullet.y = (SCREEN_HEIGHT - BULLET_HEIGHT) / 2;
    bullet.dx = 1.5;
    bullet.dy = 1.5;

    blueShip.x = 0;
    redShip.x = 0;

    blueShip.dx = 0;
    redShip.dx = 0;

    blueHealthLoss = 0;
    redHealthLoss = 0;

    blueShip.isGameOver = false;
    redShip.isGameOver = false;

    startTime = SDL_GetTicks();
}

void handleGameStateMenu(Graphics& graphics, Asset& assets, GameState& currentState)
{
    SDL_Texture* menuBackground = assets.menuBackground;

    Button buttons[4] = { Button(graphics, assets), Button(graphics, assets), Button(graphics, assets), Button(graphics, assets) };
    const char* labels[4] = {"PLAY", "GAMEMODE", "TUTORIAL", "EXIT"};

    for (int i = 0; i < 4; i++)
    {
        buttons[i].label = labels[i];
        buttons[i].rect = {BUTTON_COORDINATE_X, BUTTON_COORDINATE_Y + i * 57, BUTTON_WIDTH, BUTTON_HEIGHT};
    }

    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT) quit = true;

            else if (e.type == SDL_MOUSEMOTION)
            {
                int mx = e.motion.x, my = e.motion.y;
                for (auto& tmp : buttons)
                    tmp.isWithin = tmp.isInside(mx, my);
            }

            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                for (int i = 0; i < 4; ++i)
                {
                    if (buttons[i].isWithin)
                    {
                        switch (i)
                        {
                            case 0: currentState = PLAY; break;
                            case 1: currentState = GAMEMODE; break;
                            case 2: currentState = TUTORIAL; break;
                            case 3: currentState = EXIT; break;
                        }
                        quit = true;
                    }
                }
            }
        }

        graphics.renderTexture(menuBackground, 0, 0);
        for (int i = 0; i < 4; i++)
        {
            buttons[i].render();
        }
        graphics.presentScene();
    }
}

void handleGameStatePlay2P(Graphics& graphics, ScrollingBackground& bgr, BlueShip& blueShip, RedShip& redShip, Bullet& bullet, GameState& currentState)
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
            SDL_Delay(1500);
            currentState = GAME_OVER;
            quit = true;
        }
        else if (redShip.isGameOver)
        {
            SDL_Delay(1500);
            currentState = GAME_OVER;
            quit = true;
        }

        bgr.renderBackground(INGAME_BACKGROUND_SCROLLING_SPEED);

        blueShip.move();
        blueShip.render();

        redShip.move();
        redShip.render();

        bullet.handleLogic(blueShip, redShip);
        bullet.render();

        graphics.presentScene();
    }
}

void handleGameStatePlay1P(Graphics& graphics, ScrollingBackground& bgr, BlueShip& blueShip, RedShip& redShip, Bullet& bullet, GameState& currentState)
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
        int bulletCenterX = bullet.x + BULLET_WIDTH / 2;

        const int DEADZONE_X = 47;
        if (bullet.dy < 0)
        {
            if (bulletCenterX < blueCenterX - DEADZONE_X)
                blueShip.dx = -SHIP_VELO;
            else if (bulletCenterX > blueCenterX + DEADZONE_X)
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
            SDL_Delay(1500);
            currentState = GAME_OVER;
            quit = true;
        }
        else if (redShip.isGameOver)
        {
            SDL_Delay(1500);
            currentState = GAME_OVER;
            quit = true;
        }

        bgr.renderBackground(INGAME_BACKGROUND_SCROLLING_SPEED);

        blueShip.move();
        blueShip.render();

        redShip.move();
        redShip.render();

        bullet.handleLogic(blueShip, redShip);
        bullet.render();

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
        buttons[i].rect = {BUTTON_COORDINATE_X, 289 + i * 57, BUTTON_WIDTH, BUTTON_HEIGHT};
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
                for (auto& tmp : buttons)
                    tmp.isWithin = tmp.isInside(mx, my);
            }

            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                for (int i = 0; i < 3; i++)
                {
                    if (buttons[i].isWithin)
                    {
                        if (i == 2)
                        {
                            currentState = MENU;
                            quit = true;
                        }
                        else
                        {
                            selectedIndex = i;
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
        buttons[i].rect = {BUTTON_COORDINATE_X, 289 + i * 57, BUTTON_WIDTH, BUTTON_HEIGHT};
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
                    btn.isWithin = btn.isInside(mx, my);
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                for (int i = 0; i < 3; i++)
                {
                    if (buttons[i].isWithin)
                    {
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

        graphics.renderTexture(bgr, 0, 0);
        for (int i = 0; i < 3; i++)
        {
            buttons[i].render();
        }

        TTF_Font* titleFont = assets.font40;
        SDL_Color color = {255, 255, 255};
        SDL_Texture* titleText = graphics.renderText("GAME PAUSED", titleFont, color);

        int textW, textH;
        SDL_QueryTexture(titleText, NULL, NULL, &textW, &textH);
        graphics.renderTexture(titleText, SCREEN_WIDTH/2 - textW/2, 180);

        graphics.presentScene();
        SDL_DestroyTexture(titleText);
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

void handleGameStateGameOver(Graphics& graphics, Asset& assets, GameState& currentState, Bullet& bullet, RedShip& redShip, BlueShip& blueShip, GameMode& currentMode)
{
    SDL_Texture* bgr = assets.background;

    Button buttons[2] = { Button(graphics, assets), Button(graphics, assets) };
    const char* labels[2] = {"REPLAY", "MENU"};

    for (int i = 0; i < 2; i++)
    {
        buttons[i].label = labels[i];
        buttons[i].rect = {BUTTON_COORDINATE_X, 318 + i * 57, BUTTON_WIDTH, BUTTON_HEIGHT};
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
                    btn.isWithin = btn.isInside(mx, my);
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
                                currentState = PLAY;
                                resetGame(bullet, blueShip, redShip, blueShip.healthLoss, redShip.healthLoss);
                                break;
                            case 1:
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
