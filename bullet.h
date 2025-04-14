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
#include "Assets.h"

using namespace std;

struct Bullet
{
    Graphics& graphics;
    Asset& assets;
    SDL_Texture* texture = NULL;
    float x = (SCREEN_WIDTH - BULLET_WIDTH) / 2, dx = 1.5;
    float y = (SCREEN_HEIGHT - BULLET_HEIGHT) / 2, dy = 1.5;
    bool justCollided = false;
    vector<SDL_Rect> mColliders;

    Bullet(Graphics& g, Asset& _assets): graphics(g), assets(_assets)
    {
        texture = assets.bullet;

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

    void handleLogic(BlueShip& blueShip, RedShip& redShip)
    {
        if (handleTimeInterval(graphics, assets) == 0)
        {
            x += dx;
            shiftCollider();

            bool collidedX = false;
            if (x < 0 || x + BULLET_WIDTH > SCREEN_WIDTH)
            {
                x -= dx;
                dx = -dx;
                collidedX = true;
                shiftCollider();
            }
            else if (!justCollided && (checkCollision(mColliders, blueShip.mBlueCollider) || checkCollision(mColliders, redShip.mRedCollider)))
            {
                x -= dx;
                collidedX = true;

                dx = -dx;
                dx = dx > 0 ? dx + 0.2 : dx - 0.2;

                shiftCollider();
            }

            y += dy;
            shiftCollider();

            bool collidedY = false;
            if (y < 0 || y + BULLET_HEIGHT > SCREEN_HEIGHT)
            {
                y -= dy;
                dy = -dy;
                collidedY = true;
                shiftCollider();
            }
            else if (!justCollided && (checkCollision(mColliders, blueShip.mBlueCollider) || checkCollision(mColliders, redShip.mRedCollider)))
            {
                y -= dy;
                collidedY = true;

                dy = -dy;
                dy = dy > 0 ? dy + 0.2 : dy - 0.2;

                shiftCollider();
            }

            if ((collidedX ^ collidedY))
            {
                justCollided = true;
            }
            else if (collidedX && collidedY)
            {
                float angle;
                float dxNew, dyNew;
                float curSpeed = sqrt(dx * dx + dy * dy);

                do
                {
                    angle = (rand() % 130 - 65) * M_PI / 180.0;  // [-65, 64]
                    dxNew = curSpeed * cos(angle);
                    dyNew = curSpeed * sin(angle);
                } while (fabs(dxNew) < VERY_NEAR_ZERO_NUMBER || fabs(dyNew) < VERY_NEAR_ZERO_NUMBER);  // avoid horizontal angle / vertical angle

                dx = dxNew;
                dy = dyNew;
                justCollided = true;
            }
            else
            {
                justCollided = false;
            }

            const int DEADZONE_Y = 10;
            if (y + BULLET_HEIGHT < BLUE_SHIP_RESTRICTED_LINE_Y - DEADZONE_Y || y > RED_SHIP_RESTRICTED_LINE_Y + DEADZONE_Y)
            {
                if (y + BULLET_HEIGHT < BLUE_SHIP_RESTRICTED_LINE_Y - DEADZONE_Y)
                    blueShip.healthLoss += HEALTH_BAR_WIDTH / 4;
                else if (y > RED_SHIP_RESTRICTED_LINE_Y + DEADZONE_Y)
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
