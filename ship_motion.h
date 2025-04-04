#ifndef _SHIP_MOTION__H
#define _SHIP_MOTION__H

#include <iostream>
#include "defs.h"
#include "graphics.h"
#include <vector>

using namespace std;

struct BlueShip
{
    Graphics& graphics;
    int x = 0, dx = 0;
    int y = BLUE_SHIP_RESTRICTED_LINE_Y;
    SDL_Texture* texture;
    const int fixedY;
    SDL_Keycode leftKey, rightKey;
    vector<SDL_Rect> mBlueCollider;

    void shiftCollider()
    {
        int tmp = 0;
        for(int i = 0; i < (int)mBlueCollider.size(); i++)
        {
            mBlueCollider[i].x = x + (SHIP_WIDTH - mBlueCollider[i].w) / 2;
            mBlueCollider[i].y = y + tmp;
            tmp += mBlueCollider[i].h;
        }
    }

    BlueShip(Graphics& g, const char* texturePath, const int yPos, SDL_Keycode left, SDL_Keycode right)
        : graphics(g), texture(graphics.loadTexture(texturePath)), fixedY(yPos), leftKey(left), rightKey(right)
        {
            mBlueCollider.resize(3);

            mBlueCollider[0] = {0, 0, 95, 1};
            mBlueCollider[1] = {0, 0, 71, 5};
            mBlueCollider[2] = {0, 0, 63, 4};

            shiftCollider();
        }

    ~BlueShip() { if(texture) SDL_DestroyTexture(texture);}

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

    void render()
    {
        graphics.DrawRestrictedLine();
        graphics.renderTexture(texture, x, fixedY);
    }

};


struct RedShip
{
    Graphics& graphics;
    int x = 0, dx = 0;
    int y = RED_SHIP_RESTRICTED_LINE_Y;
    SDL_Texture* texture;
    const int fixedY;
    SDL_Keycode leftKey, rightKey;
    vector<SDL_Rect> mRedCollider;

    void shiftCollider()
    {
        int tmp = 0;
        for(int i = 0; i < (int)mRedCollider.size(); i++)
        {
            mRedCollider[i].x = x + (SHIP_WIDTH - mRedCollider[i].w) / 2;
            mRedCollider[i].y = y + tmp;
            tmp += mRedCollider[i].h;
        }
    }

    RedShip(Graphics& g, const char* texturePath, const int yPos, SDL_Keycode left, SDL_Keycode right)
        : graphics(g), texture(graphics.loadTexture(texturePath)), fixedY(yPos), leftKey(left), rightKey(right)
        {
            mRedCollider.resize(3);

            mRedCollider[0].w = 63;
            mRedCollider[0].h = 4;

            mRedCollider[1].w = 71;
            mRedCollider[1].h = 5;

            mRedCollider[2].w = 95;
            mRedCollider[2].h = 1;

            shiftCollider();
        }

    ~RedShip() { if(texture) SDL_DestroyTexture(texture);}

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

    void render()
    {
        graphics.renderTexture(texture, x, fixedY);
    }

};

#endif // _SHIP_MOTION__H
