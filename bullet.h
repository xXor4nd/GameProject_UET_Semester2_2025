#ifndef _BULLET__H
#define _BULLET__H

#include <vector>
#include <iostream>
#include <cmath>

#include "graphics.h"
#include "defs.h"
#include "ship_motion.h"
#include "collision.h"
#include "time.h"

using namespace std;

struct Bullet
{
    Graphics& graphics;
    SDL_Texture* texture;
    float x = (SCREEN_WIDTH - BULLET_WIDTH) / 2, dx = 1.5;
    float y = (SCREEN_HEIGHT - BULLET_HEIGHT) / 2, dy = 1.5;
    bool justCollided = false;
    vector<SDL_Rect> mColliders;

    Bullet(Graphics& g, const char* texturePath ): graphics(g), texture(graphics.loadTexture(texturePath))
    {
        mColliders.resize(11);

        mColliders[0] = {0, 0, 5, 1};
        mColliders[1] = {0, 0, 9, 1};
        mColliders[2] = {0, 0, 13, 1};
        mColliders[3] = {0, 0, 15, 2};
        mColliders[4] = {0, 0, 17, 2};
        mColliders[5] = {0, 0, 19, 5};
        mColliders[6] = {0, 0, 17, 2};
        mColliders[7] = {0, 0, 15, 2};
        mColliders[8] = {0, 0, 13, 1};
        mColliders[9] = {0, 0, 9, 1};
        mColliders[10] = {0, 0, 5, 1};

        shiftCollider();
    }

    ~Bullet() { if(texture) SDL_DestroyTexture(texture); }

    void shiftCollider()
    {
        int tmp = 0;
        for(int i = 0; i < (int)mColliders.size(); i++)
        {
            mColliders[i].x = x + (BULLET_WIDTH - mColliders[i].w) / 2;
            mColliders[i].y = y + tmp;
            tmp += mColliders[i].h;
        }
    }

    void move(BlueShip& blueShip, RedShip& redShip)
    {
        if(handleTimeInterval(graphics) == 0)
        {
            x += dx;
            shiftCollider();

            if(x < 0 || x + BULLET_WIDTH > SCREEN_WIDTH)
            {
                x -= dx;
                dx = -dx;
                shiftCollider();
            }
            else if(!justCollided && (checkCollision(mColliders, blueShip.mBlueCollider) || checkCollision(mColliders, redShip.mRedCollider)))
            {
                x -= dx;
                dx = -dx;
                dx = dx > 0 ? dx + 0.2 : dx - 0.2;
                justCollided = true;
                shiftCollider();
            }
            else
            {
                justCollided = false;
            }


            y += dy;
            shiftCollider();
            if(y < 0 || y + BULLET_HEIGHT > SCREEN_HEIGHT)
            {
                y -= dy;
                dy = -dy;
                shiftCollider();
            }
            else if(!justCollided && (checkCollision(mColliders, blueShip.mBlueCollider) || checkCollision(mColliders, redShip.mRedCollider)))
            {
                y -= dy;
                dy = -dy;
                dy = dy > 0 ? dy + 0.2 : dy - 0.2;
                justCollided = true;
                shiftCollider();
            }
            else
            {
                justCollided = false;
            }

            if(y + BULLET_HEIGHT < BLUE_SHIP_RESTRICTED_LINE_Y || y > RED_SHIP_RESTRICTED_LINE_Y)
            {
                if (y + BULLET_HEIGHT < BLUE_SHIP_RESTRICTED_LINE_Y)
                    blueShip.healthLoss += HEALTH_BAR_WIDTH / 4;
                else if (y > RED_SHIP_RESTRICTED_LINE_Y)
                    redShip.healthLoss += HEALTH_BAR_WIDTH / 4;

                x = SCREEN_WIDTH / 2 - BULLET_WIDTH / 2;
                y = SCREEN_HEIGHT / 2 - BULLET_HEIGHT / 2;
                dx = dy = 1.5;
                startTime = SDL_GetTicks();
                shiftCollider();
            }
        }
    }

    void render()
    {
        graphics.renderTexture(texture, x, y);
    }
};

#endif // _BULLET__H
