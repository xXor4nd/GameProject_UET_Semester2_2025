#ifndef _LOGIC__H
#define _LOGIC__H

#include "graphics.h"
#include "animation.h"
#include "ship_motion.h"
#include "bullet.h"
#include "Assets.h"
#include "sound.h"
#include "GameState.h"
#include "defs.h"
#include "game.h"
#include "fps.h"

struct Button
{
    Game& game;

    SDL_Rect rect;
    const char* label;
    SDL_Texture* buttonIMG = NULL;
    TTF_Font* font;

    bool isWithin = false;
    bool isSelected = false;

    Button (Game& _game) : game(_game)
    {
        buttonIMG = game.assets.button;
        font = game.assets.font30;
    }

    void render()
    {
        SDL_Color color;
        if (isSelected || isWithin) color = {0, 255, 100};
        else color = {255, 255, 255};

        SDL_SetRenderDrawColor(game.graphics.renderer, 50, 50, 80, 255);
        SDL_RenderFillRect(game.graphics.renderer, &rect);
        game.graphics.renderTexture(buttonIMG, rect.x, rect.y);
        SDL_Texture* buttonText = game.graphics.renderText(label, font, color);

        int textW, textH;
        SDL_QueryTexture(buttonText, NULL, NULL, &textW, &textH);
        int textX = rect.x + (rect.w - textW) / 2;
        int textY = rect.y + (rect.h - textH) / 2;
        game.graphics.renderTexture(buttonText, textX, textY);

        SDL_DestroyTexture(buttonText);
    }

    bool isInside(int x, int y)
    {
        return (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h);
    }

};

void resetGame(Game& game)
{
    game.bulletManager.resetRound(game.blueShip, game.redShip);

    game.blueShip.x = 0;
    game.redShip.x = 0;

    game.blueShip.dx = 0;
    game.redShip.dx = 0;

    game.blueShip.healthLoss = 0;
    game.redShip.healthLoss = 0;

    game.blueShip.isGameOver = false;
    game.redShip.isGameOver = false;

    game.bulletManager.startTime = SDL_GetTicks();
}

void handleGameStateMenu(Game& game)
{
    SDL_Texture* menuBackground = game.assets.menuBackground;
    Uint32 prevTick = SDL_GetTicks();
    SDL_Rect soundButton = { 10, SCREEN_HEIGHT - 34, 24, 24 };
    Uint32 menuStart = SDL_GetTicks();

    Button buttons[4] = { Button(game), Button(game), Button(game), Button(game) };
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
                game.currentState = EXIT;
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
                        game.graphics.play(game.assets.hoverSound);
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
                            case 0:
                                {
                                    Uint32 passedTime = SDL_GetTicks() - menuStart;
                                    game.bulletManager.startTime += passedTime;
                                    game.bulletManager.eventStartTime += passedTime;
                                    game.currentState = PLAY;
                                    game.graphics.play(game.assets.clickedSound);
                                    break;
                                }
                            case 1: game.currentState = GAMEMODE; game.graphics.play(game.assets.clickedSound); break;
                            case 2: game.currentState = TUTORIAL; game.graphics.play(game.assets.clickedSound); break;
                            case 3: game.currentState = EXIT; game.graphics.play(game.assets.clickedSound); break;
                        }
                        quit = true;
                    }
                }

                int mx = e.button.x, my = e.button.y;
                if (mx >= soundButton.x && mx <= soundButton.x + soundButton.w && my >= soundButton.y && my <= soundButton.y + soundButton.h)
                {
                    game.sounds.toggleMuteMenuMusic();
                }
            }
        }

        game.graphics.renderTexture(menuBackground, 0, 0);
        for (int i = 0; i < 4; i++)
        {
            buttons[i].render();
        }

        Uint32 now = SDL_GetTicks();
        if (now - prevTick > 100)
        {
            game.sparkle.tick();
            prevTick = now;
        }
        game.sparkle.renderSprite(10, 10);
        game.sparkle.renderSprite(SCREEN_WIDTH - 10 - 32, 10);

        if (game.sounds.isMuted_menuMusic) game.graphics.renderTexture(game.assets.volume_off, soundButton.x, soundButton.y);
        else game.graphics.renderTexture(game.assets.volume_on, soundButton.x, soundButton.y);

        game.graphics.presentScene();
    }
}

void handleGameStatePlay2P(Game& game)
{
    SDL_Event e;
    bool quit = false;
    FPS FPSlimit(150);

    while (!quit)
    {
        FPSlimit.startFrame();
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                game.currentState = EXIT;
                quit = true;
            }

            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                game.currentState = PAUSED;
                quit = true;
            }

            game.blueShip.handleEvent(e);
            game.redShip.handleEvent(e);

//            int x, y;
//            SDL_GetMouseState(&x, &y);
//            cerr << x << ", " << y << endl;

        }

        if (game.blueShip.isGameOver)
        {
            game.graphics.play(game.assets.gameoverSound);
            SDL_Delay(1500);
            game.currentState = GAME_OVER;
            quit = true;
        }
        else if (game.redShip.isGameOver)
        {
            game.graphics.play(game.assets.gameoverSound);
            SDL_Delay(1500);
            game.currentState = GAME_OVER;
            quit = true;
        }

        //game.graphics.prepareScene();

        game.bgr.renderBackground(INGAME_BACKGROUND_SCROLLING_SPEED);

        game.blueShip.move();
        game.blueShip.render();

        game.redShip.move();
        game.redShip.render();

        game.bulletManager.handleLogic(game.blueShip, game.redShip);
        game.bulletManager.render(game.blueShip, game.redShip);

        game.graphics.presentScene();
        FPSlimit.endFrame();
    }
}

void handleGameStatePlay1P(Game& game)
{
    SDL_Event e;
    bool quit = false;
    FPS FPSlimit(150);

    while (!quit)
    {
        FPSlimit.startFrame();
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                game.currentState = EXIT;
                quit = true;
            }

            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                game.currentState = PAUSED;
                quit = true;
            }

            game.redShip.handleEvent(e);
        }

        int blueCenterX = game.blueShip.x + SHIP_WIDTH / 2;
        int blueCenterY = game.blueShip.y + SHIP_HEIGHT / 2;

        float minDist = 1e9;
        float targetX = -1;

        if (!game.bulletManager.is2BulletsEvent)
        {
            Bullet& b = game.bulletManager.bullet1;
            if (!b.roundEnded && b.dy < 0)
            {
                float bulletCenterX = b.x + BULLET_WIDTH / 2;
                targetX = bulletCenterX;
            }
        }
        else
        {
            Bullet* bullets[] = { &game.bulletManager.bullet1, &game.bulletManager.bullet2 };
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
        }

        if (targetX != -1)
        {
            if (targetX < blueCenterX - DELAY_DISTANCE_X)
                game.blueShip.dx = -SHIP_BOT_VELO;
            else if (targetX > blueCenterX + DELAY_DISTANCE_X)
                game.blueShip.dx = SHIP_BOT_VELO;
            else
                game.blueShip.dx = 0;
        }
        else
        {
            game.blueShip.dx = 0;
        }

        if (game.blueShip.isGameOver)
        {
            game.graphics.play(game.assets.gameoverSound);
            SDL_Delay(1500);
            game.currentState = GAME_OVER;
            quit = true;
        }
        else if (game.redShip.isGameOver)
        {
            game.graphics.play(game.assets.gameoverSound);
            SDL_Delay(1500);
            game.currentState = GAME_OVER;
            quit = true;
        }

        game.bgr.renderBackground(INGAME_BACKGROUND_SCROLLING_SPEED);

        game.blueShip.move();
        game.blueShip.render();

        game.redShip.move();
        game.redShip.render();

        game.bulletManager.handleLogic(game.blueShip, game.redShip);
        game.bulletManager.render(game.blueShip, game.redShip);

        game.graphics.presentScene();
        FPSlimit.endFrame();
    }
}


void handleGameStateGamemode(Game& game)
{
    SDL_Texture* bgr = game.assets.background;

    Button buttons[3] = { Button(game), Button(game), Button(game) };
    const char* labels[3] = {"1 PLAYER", "2 PLAYERS", "BACK"};

    for (int i = 0; i < 3; i++)
    {
        buttons[i].label = labels[i];
        buttons[i].rect = {BUTTON_COORDINATE_X, 289 + i * BUTTON_HEIGHT, BUTTON_WIDTH, BUTTON_HEIGHT};
    }

    SDL_Event e;
    bool quit = false;
    int selectedIndex = (game.currentMode == MODE_1_PLAYER ? 0 : 1);

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                game.currentState = EXIT;
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
                        game.graphics.play(game.assets.hoverSound);
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
                            game.graphics.play(game.assets.clickedSound);
                            game.currentState = MENU;
                            quit = true;
                        }
                        else
                        {
                            selectedIndex = i;
                            game.graphics.play(game.assets.clickedSound);
                            game.currentMode = (i == 0) ? MODE_1_PLAYER : MODE_2_PLAYER;
                            quit = true;
                        }
                    }
                }
            }
        }

        game.graphics.renderTexture(bgr, 0, 0);
        for (int i = 0; i < 3; i++)
        {
            buttons[i].isSelected = (i == selectedIndex);
            buttons[i].render();
        }
        game.graphics.presentScene();
    }
}

void handleGameStatePaused(Game& game)
{
    SDL_Texture* bgr = game.assets.background;
    Uint32 pauseStart = SDL_GetTicks();

    Button buttons[3] = { Button(game), Button(game), Button(game) };
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
                game.currentState = EXIT;
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
                        game.graphics.play(game.assets.hoverSound);
                }
            }

            else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT)
            {
                int mx = e.button.x, my = e.button.y;
                if (mx >= bgmBox.x && mx <= bgmBox.x + bgmBox.w && my >= bgmBox.y && my <= bgmBox.y + bgmBox.h)
                {
                    game.sounds.bgmMuted = !game.sounds.bgmMuted;
                    game.sounds.updateVolume();
                }
                else if (mx >= collisionBox.x && mx <= collisionBox.x + collisionBox.w && my >= collisionBox.y && my <= collisionBox.y + collisionBox.h)
                {
                    game.sounds.collisionMuted = !game.sounds.collisionMuted;
                }
                else if (mx >= pointBox.x && mx <= pointBox.x + pointBox.w && my >= pointBox.y && my <= pointBox.y + pointBox.h)
                {
                    game.sounds.pointMuted = !game.sounds.pointMuted;
                }
                else
                {
                    for (int i = 0; i < 3; i++)
                    {
                        if (buttons[i].isWithin)
                        {
                            game.graphics.play(game.assets.clickedSound);
                            switch (i)
                            {
                                case 0:
                                {
                                    Uint32 passedTime = SDL_GetTicks() - pauseStart;
                                    game.bulletManager.startTime += passedTime;
                                    game.currentState = PLAY;
                                    break;
                                }
                                case 1:
                                    game.currentState = PLAY;
                                    game.sounds.stopMusic();
                                    resetGame(game);
                                    break;
                                case 2:
                                    game.currentState = MENU;
                                    break;
                            }
                            quit = true;
                        }
                    }
                }
            }
        }

        game.graphics.renderTexture(bgr, 0, 0);
        for (int i = 0; i < 3; i++)
        {
            buttons[i].render();
        }

        TTF_Font* font = game.assets.font20;
        SDL_Color white = {255, 255, 255};
        SDL_Texture* text1 = game.graphics.renderText("Background Music", font, white);
        SDL_Texture* text2 = game.graphics.renderText("Collision Sound", font, white);
        SDL_Texture* text3 = game.graphics.renderText("GamePoint Sound", font, white);

        game.graphics.renderTexture(text1, 100, 140);
        game.graphics.renderTexture(text2, 100, 190);
        game.graphics.renderTexture(text3, 100, 240);

        game.graphics.renderTexture(game.sounds.bgmMuted ? game.assets.uncheck_square : game.assets.checked_square, bgmBox.x, bgmBox.y);
        game.graphics.renderTexture(game.sounds.collisionMuted ? game.assets.uncheck_square : game.assets.checked_square, collisionBox.x, collisionBox.y);
        game.graphics.renderTexture(game.sounds.pointMuted ? game.assets.uncheck_square : game.assets.checked_square, pointBox.x, pointBox.y);

        TTF_Font* titleFont = game.assets.font40;
        SDL_Texture* titleText = game.graphics.renderText("GAME PAUSED", titleFont, white);
        int textW, textH;
        SDL_QueryTexture(titleText, NULL, NULL, &textW, &textH);
        game.graphics.renderTexture(titleText, SCREEN_WIDTH/2 - textW/2, 60);

        game.graphics.presentScene();

        SDL_DestroyTexture(titleText);
        SDL_DestroyTexture(text1);
        SDL_DestroyTexture(text2);
        SDL_DestroyTexture(text3);
    }
}


void handleGameStateTutorial(Game& game)
{
    SDL_Texture* bgr = game.assets.background;
    SDL_Texture* tutorialIMG = game.assets.tutorial;

    SDL_Event e;
    bool quit = false;
    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                game.currentState = EXIT;
                quit = true;
            }

            else if ( e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_RETURN) )
            {
                game.currentState = MENU;
                quit = true;
            }
        }

        game.graphics.renderTexture(bgr, 0, 0);
        game.graphics.renderTexture(tutorialIMG, 0, 235);
        game.graphics.presentScene();
    }
}

void handleGameStateGameOver(Game& game)
{
    SDL_Texture* bgr = game.assets.background;

    Button buttons[2] = { Button(game), Button(game) };
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
                game.currentState = EXIT;
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
                        game.graphics.play(game.assets.hoverSound);
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
                                game.graphics.play(game.assets.clickedSound);
                                game.currentState = PLAY;
                                resetGame(game);
                                break;
                            case 1:
                                game.graphics.play(game.assets.clickedSound);
                                game.currentState = MENU;
                                break;
                        }
                        quit = true;
                    }
                }
            }
        }

        game.graphics.renderTexture(bgr, 0, 0);
        for (int i = 0; i < 2; i++)
        {
            buttons[i].render();
        }

        TTF_Font* titleFont = game.assets.font40;
        SDL_Color color = {255, 255, 255};
        SDL_Texture* titleText = game.graphics.renderText("GAME OVER!", titleFont, color);

        int textW, textH;
        SDL_QueryTexture(titleText, NULL, NULL, &textW, &textH);
        game.graphics.renderTexture(titleText, SCREEN_WIDTH/2 - textW/2, 200);

        const char* GameOverText;
        if (game.currentMode == MODE_2_PLAYER)
        {
            if (game.redShip.isGameOver)
            {
                GameOverText = "BLUE WON!";
                color = {0, 0, 255};
            }
            else if (game.blueShip.isGameOver)
            {
                GameOverText = "RED WON!";
                color = {255, 0, 0};
            }
            // else GameOverText = "???";
        }

        else
        {
            if (game.redShip.isGameOver)
            {
                GameOverText = "YOU LOSE :(";
                color = {255, 172, 28};
            }
            else if (game.blueShip.isGameOver)
            {
                GameOverText = "YOU WIN!";
                color = {255, 172, 28};
            }
            // else GameOverText = "???";
        }


        if (!isPlayed)
        {
            if (game.currentMode == MODE_2_PLAYER)
            {
                if (game.redShip.isGameOver)
                {
                    game.graphics.play(game.assets.winSound);
                }
                else if (game.blueShip.isGameOver)
                {
                    game.graphics.play(game.assets.winSound);
                }
            }
            else
            {
                if (game.redShip.isGameOver)
                {
                    game.graphics.play(game.assets.loseSound);
                }
                else if (game.blueShip.isGameOver)
                {
                    game.graphics.play(game.assets.winSound);
                }
            }
            isPlayed = true;
        }

        SDL_Texture* winningText = game.graphics.renderText(GameOverText, titleFont, color);
        int winW, winH;
        SDL_QueryTexture(winningText, NULL, NULL, &winW, &winH);
        game.graphics.renderTexture(winningText, SCREEN_WIDTH/2 - winW/2, 500);

        game.graphics.presentScene();

        SDL_DestroyTexture(titleText);
        SDL_DestroyTexture(winningText);
    }
}

#endif // _LOGIC__H
