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
    EXIT
};

struct Button
{
    Graphics& graphics;
    SDL_Rect rect;
    const char* label;
    SDL_Texture* buttonIMG = graphics.loadTexture(BUTTON_IMG);
    TTF_Font* font = graphics.loadFont(FONT, 30);
    bool isWithin = false;

    Button (Graphics& g) : graphics(g){}
    ~Button (){}

    void render()
    {
        SDL_Color color = isWithin ? SDL_Color{200, 200, 255, 255} : SDL_Color{255, 255, 255, 255};
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
        for (auto& tmp : buttons) tmp.render();
        graphics.presentScene();
    }
}

void handleGameStatePlay(Graphics& graphics, ScrollingBackground& bgr, BlueShip& blueShip, RedShip& redShip, Bullet& bullet, GameState& currentState)
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

void handleGameStateGamemode(GameState& currentState)
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

        }
    }
}

void handleGameStateTutorial(GameState& currentState)
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

        }
    }
}

void handleGameStateExit(Graphics& graphics)
{

}
#endif // _MENU__H
