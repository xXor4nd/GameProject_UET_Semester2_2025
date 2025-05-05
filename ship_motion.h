#ifndef _SHIP_MOTION__H
#define _SHIP_MOTION__H

#include <iostream>
#include <vector>

#include "defs.h"
#include "graphics.h"
#include "Assets.h"

using namespace std;

struct BlueShip
{
    Graphics& graphics;
    Asset& assets;

    int x = 0, dx = 0;
    int y = BLUE_SHIP_RESTRICTED_LINE_Y;
    int healthLoss = 0;
    bool isGameOver = false;

    SDL_Texture* shipTexture = NULL;
    SDL_Texture* health_bar_texture = NULL;
    SDL_Texture* blue_heart = NULL;

    const int fixedY;
    SDL_Keycode leftKey, rightKey;
    vector<SDL_Rect> mBlueCollider;

    BlueShip(Graphics& g, Asset& _assets, const int yPos, SDL_Keycode left, SDL_Keycode right)
        : graphics(g), assets(_assets), fixedY(yPos), leftKey(left), rightKey(right)
        {
            shipTexture = assets.blueShip;
            health_bar_texture = assets.healthBar;
            blue_heart = assets.heart;

            mBlueCollider.resize(3);

            mBlueCollider[0] = {0, 0, 65, 5};
            mBlueCollider[1] = {0, 0, 55, 5};
            mBlueCollider[2] = {0, 0, 45, 5};

            shiftCollider();
        }

    void handleEvent(SDL_Event& e)
    {
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_LEFT: if (leftKey == SDLK_LEFT) dx -= SHIP_VELO; break;
                case SDLK_RIGHT: if (rightKey == SDLK_RIGHT) dx += SHIP_VELO; break;
            }
        }
        else if (e.type == SDL_KEYUP && e.key.repeat == 0)
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_LEFT: if (leftKey == SDLK_LEFT) dx += SHIP_VELO; break;
                case SDLK_RIGHT: if (rightKey == SDLK_RIGHT) dx -= SHIP_VELO; break;
            }
        }
    }

    void shiftCollider()
    {
        int tmp = 0;
        for (int i = 0; i < (int)mBlueCollider.size(); i++)
        {
            mBlueCollider[i].x = x + (SHIP_WIDTH - mBlueCollider[i].w) / 2;
            mBlueCollider[i].y = y + tmp;
            tmp += mBlueCollider[i].h;
        }
    }

    void move()
    {
        x += dx;
        shiftCollider();
        if (x < 0 || x + SHIP_WIDTH > SCREEN_WIDTH)
        {
            x -= dx;
            shiftCollider();
        }
    }

    void handleHealth()
    {
        if (healthLoss >= HEALTH_BAR_WIDTH)
        {
            isGameOver = true;
        }

        int remainingHealth = max(0, HEALTH_BAR_WIDTH - healthLoss);
        SDL_Rect health_bar_rect = { BLUE_SHIP_HEALTH_BAR_FIXED_COORDINATE_X, BLUE_SHIP_HEALTH_BAR_FIXED_COORDINATE_Y, remainingHealth, HEALTH_BAR_HEIGHT };

        SDL_SetRenderDrawColor(graphics.renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(graphics.renderer, &health_bar_rect);
    }

    void render()
    {
        graphics.DrawRestrictedLine();
        graphics.renderTexture(shipTexture, x, fixedY);
        handleHealth();
        graphics.renderTexture(blue_heart, BLUE_SHIP_HEART_FIXED_COORDINATE_X, BLUE_SHIP_HEART_FIXED_COORDINATE_Y);
        graphics.renderTexture(health_bar_texture, BLUE_SHIP_HEALTH_BAR_FIXED_COORDINATE_X, BLUE_SHIP_HEALTH_BAR_FIXED_COORDINATE_Y);

        // Debug
//        for (const auto& rect : mBlueCollider)
//        {
//            SDL_SetRenderDrawColor(graphics.renderer, 255, 0, 0, 255);
//            SDL_RenderDrawRect(graphics.renderer, &rect);
//        }
    }

};


struct RedShip
{
    Graphics& graphics;
    Asset& assets;

    int x = 0, dx = 0;
    int y = SCREEN_HEIGHT - SHIP_HEIGHT - 35;
    int healthLoss = 0;
    bool isGameOver = false;

    SDL_Texture* shipTexture = NULL;
    SDL_Texture* health_bar_texture = NULL;
    SDL_Texture* red_heart = NULL;

    const int fixedY;
    SDL_Keycode leftKey, rightKey;
    vector<SDL_Rect> mRedCollider;

    RedShip(Graphics& g, Asset& _assets, const int yPos, SDL_Keycode left, SDL_Keycode right)
        : graphics(g), assets(_assets), fixedY(yPos), leftKey(left), rightKey(right)
        {
            shipTexture = assets.redShip;
            health_bar_texture = assets.healthBar;
            red_heart = assets.heart;

            mRedCollider.resize(3);

            mRedCollider[0] = {0, 0, 45, 5};
            mRedCollider[1] = {0, 0, 55, 5};
            mRedCollider[2] = {0, 0, 65, 5};

            shiftCollider();
        }

    void handleEvent(SDL_Event& e)
    {
        if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_a: if (leftKey == SDLK_a) dx -= SHIP_VELO; break;
                case SDLK_d: if (rightKey == SDLK_d) dx += SHIP_VELO; break;
            }
        }
        else if (e.type == SDL_KEYUP && e.key.repeat == 0)
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_a: if (leftKey == SDLK_a) dx += SHIP_VELO; break;
                case SDLK_d: if (rightKey == SDLK_d) dx -= SHIP_VELO; break;
            }
        }
    }

    void shiftCollider()
    {
        int tmp = 0;
        for (int i = 0; i < (int)mRedCollider.size(); i++)
        {
            mRedCollider[i].x = x + (SHIP_WIDTH - mRedCollider[i].w) / 2;
            mRedCollider[i].y = y + tmp;
            tmp += mRedCollider[i].h;
        }
    }

    void move()
    {
        x += dx;
        shiftCollider();
        if (x < 0 || x + SHIP_WIDTH > SCREEN_WIDTH)
        {
            x -= dx;
            shiftCollider();
        }
    }

    void handleHealth()
    {
        if (healthLoss >= HEALTH_BAR_WIDTH)
        {
            isGameOver = true;
        }

        int remainingHealth = max(0, HEALTH_BAR_WIDTH - healthLoss);
        SDL_Rect health_bar_rect = { RED_SHIP_HEALTH_BAR_FIXED_COORDINATE_X, RED_SHIP_HEALTH_BAR_FIXED_COORDINATE_Y, remainingHealth, HEALTH_BAR_HEIGHT };

        SDL_SetRenderDrawColor(graphics.renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(graphics.renderer, &health_bar_rect);
    }

    void render()
    {
        graphics.renderTexture(shipTexture, x, fixedY);
        handleHealth();
        graphics.renderTexture(red_heart, RED_SHIP_HEART_FIXED_COORDINATE_X, RED_SHIP_HEART_FIXED_COORDINATE_Y);
        graphics.renderTexture(health_bar_texture, RED_SHIP_HEALTH_BAR_FIXED_COORDINATE_X, RED_SHIP_HEALTH_BAR_FIXED_COORDINATE_Y);

        // Debug
//        for (const auto& rect : mRedCollider)
//        {
//            SDL_SetRenderDrawColor(graphics.renderer, 255, 0, 0, 255);
//            SDL_RenderDrawRect(graphics.renderer, &rect);
//        }
    }

};

#endif // _SHIP_MOTION__H
