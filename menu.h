#ifndef _MENU__H
#define _MENU__H

#include "graphics.h"
#include "background.h"
#include "ship_motion.h"
#include "bullet.h"

enum GameState
{
    MENU,
    PLAY,
    GAMEMODE,
    TUTORIAL,
    EXIT,
    PAUSED
};

enum GameMode
{
    MODE_1_PLAYER,
    MODE_2_PLAYER
};

struct Button
{
    Graphics& graphics;
    SDL_Rect rect;
    const char* label;
    SDL_Texture* buttonIMG;
    TTF_Font* font;
    bool isWithin = false;
    bool isSelected = false;

    Button (Graphics& g) : graphics(g)
    {
        buttonIMG = graphics.loadTexture(BUTTON_IMG);
        font = graphics.loadFont(FONT, 30);
    }
    ~Button ()
    {
        if (buttonIMG) SDL_DestroyTexture(buttonIMG);
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
        SDL_QueryTexture(buttonText, nullptr, nullptr, &textW, &textH);
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

void handleGameStateMenu(Graphics& graphics, GameState& currentState)
{
    SDL_Texture* menuBackground = graphics.loadTexture(MENU_BACKGROUND_IMG);

    Button buttons[4] = { Button(graphics), Button(graphics), Button(graphics), Button(graphics) };
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

    SDL_DestroyTexture(menuBackground);
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


void handleGameStateGamemode(Graphics& graphics, GameState& currentState, GameMode& currentMode)
{
    SDL_Texture* bgr = graphics.loadTexture(INGAME_BACKGROUND_IMG);

    Button buttons[3] = { Button(graphics), Button(graphics), Button(graphics) };
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
    SDL_DestroyTexture(bgr);
}

void handleGameStatePaused(Graphics& graphics, GameState& currentState, bool& replayRequested)
{
    SDL_Texture* bgr = graphics.loadTexture(INGAME_BACKGROUND_IMG);

    Button buttons[3] = { Button(graphics), Button(graphics), Button(graphics) };
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

        TTF_Font* titleFont = graphics.loadFont(FONT, 40);
        SDL_Color color = {255, 255, 255};
        SDL_Texture* titleText = graphics.renderText("GAME PAUSED", titleFont, color);

        int textW, textH;
        SDL_QueryTexture(titleText, NULL, NULL, &textW, &textH);
        graphics.renderTexture(titleText, SCREEN_WIDTH/2 - textW/2, 180);
        SDL_DestroyTexture(titleText);

        graphics.presentScene();
    }

    SDL_DestroyTexture(bgr);
}


void handleGameStateTutorial(Graphics& graphics, GameState& currentState)
{
    SDL_Texture* bgr = graphics.loadTexture(INGAME_BACKGROUND_IMG);
    SDL_Texture* tutorialIMG = graphics.loadTexture(TUTORIAL_IMG);

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
    SDL_DestroyTexture(bgr);
    SDL_DestroyTexture(tutorialIMG);
}

void resetGame(Bullet& bullet, BlueShip& blueShip, RedShip& redShip, int& blueHealthLoss, int& redHealthLoss)
{
    bullet.x = (SCREEN_WIDTH - BULLET_WIDTH) / 2;
    bullet.y = (SCREEN_HEIGHT - BULLET_HEIGHT) / 2;
    bullet.dx = 1.5;
    bullet.dy = 1.5;

    blueShip.x = 0;
    redShip.x = 0;

    blueHealthLoss = 0;
    redHealthLoss = 0;

    startTime = SDL_GetTicks();
}

#endif // _MENU__H
