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
#include "sound.h"

using namespace std;

struct Bullet
{
    Graphics& graphics;
    Asset& assets;
    Sound& sounds;

    float x = (SCREEN_WIDTH - BULLET_WIDTH) / 2, dx = BULLET_INITIAL_SPEED;
    float y = (SCREEN_HEIGHT - BULLET_HEIGHT) / 2, dy = BULLET_INITIAL_SPEED;
    bool roundEnded = false;
    bool isDamaged = false;

    SDL_Texture* bulletTexture = NULL;
    vector<SDL_Rect> mColliders;

    Bullet(Graphics& g, Asset& _assets, Sound& _sounds): graphics(g), assets(_assets), sounds(_sounds)
    {
        bulletTexture = assets.bullet;

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
        for (int i = 0; i < (int)mColliders.size(); i++)
        {
            mColliders[i].x = x + (BULLET_WIDTH - mColliders[i].w) / 2;
            mColliders[i].y = y + tmp;
            tmp += mColliders[i].h;
        }
    }

    void pushOutOfCollider(const vector<SDL_Rect>& shipColliders)
    {
        while (checkCollision(mColliders, shipColliders))
        {
            x += (dx > 0 ? -1 : 1);
            y += (dy > 0 ? -1 : 1);
            shiftCollider();
        }
    }

    void handleLogic(BlueShip& blueShip, RedShip& redShip, int timeLeft)
    {
        if (timeLeft == 0)
        {
            bool collidedX = false;
            bool collidedY = false;
            bool isPlayCollisionSound = false;

            x += dx;
            shiftCollider();

            if (x < 0 || x + BULLET_WIDTH > SCREEN_WIDTH)
            {
                x -= dx;
                dx = -dx;
                collidedX = true;
                shiftCollider();
            }

            else if (checkCollision(mColliders, blueShip.mBlueCollider) || checkCollision(mColliders, redShip.mRedCollider))
            {
                collidedX = true;
                isPlayCollisionSound = true;

                x -= dx;
                shiftCollider();
                pushOutOfCollider(blueShip.mBlueCollider);
                pushOutOfCollider(redShip.mRedCollider);

                dx = -dx;
                do
                {
                    dx = dx > 0 ? dx + 0.2 : dx - 0.2;
                } while (dx == 0);
            }

            y += dy;
            shiftCollider();

            if (checkCollision(mColliders, blueShip.mBlueCollider) || checkCollision(mColliders, redShip.mRedCollider))
            {
                collidedY = true;
                isPlayCollisionSound = true;

                y -= dy;
                shiftCollider();
                pushOutOfCollider(blueShip.mBlueCollider);
                pushOutOfCollider(redShip.mRedCollider);

                dy = -dy;

                do
                {
                    dy = dy > 0 ? dy + 0.2 : dy - 0.2;
                } while (dy == 0);
            }


            if (collidedX && collidedY)
            {
                float curSpeed = sqrt(dx * dx + dy * dy);
                GenerateRandomAngle(curSpeed);
            }

            if (isPlayCollisionSound && !sounds.collisionMuted)
                graphics.play(assets.collisionSound);

//           if (collidedX || collidedY)
//           {
//               cout << "Collision detected!" << endl;
//               cout << "New dx = " << dx << ", dy = " << dy << endl;
//           }

//            if (collidedX && collidedY)
//            {
//                cout << "Double Collision detected!" << endl;
//                cout << "New dx = " << dx << ", dy = " << dy << endl;
//            }

            roundEnded = (y + BULLET_HEIGHT < BLUE_SHIP_RESTRICTED_LINE_Y - DEADZONE_Y || y > RED_SHIP_RESTRICTED_LINE_Y + DEADZONE_Y);
        }

        if (roundEnded && !isDamaged)
        {
            if (!sounds.pointMuted)
                graphics.play(assets.gamepointSound);

            if (y + BULLET_HEIGHT < BLUE_SHIP_RESTRICTED_LINE_Y - DEADZONE_Y)
                blueShip.healthLoss += HEALTH_BAR_WIDTH / 6;
            else if (y > RED_SHIP_RESTRICTED_LINE_Y + DEADZONE_Y)
                redShip.healthLoss += HEALTH_BAR_WIDTH / 6;

            isDamaged = true;
        }
    }

    void resetBullet(BlueShip& blueShip, RedShip& redShip)
    {
        x = SCREEN_WIDTH / 2 - BULLET_WIDTH / 2;
        y = SCREEN_HEIGHT / 2 - BULLET_HEIGHT / 2;

        roundEnded = false;
        isDamaged = false;

        GenerateRandomAngle(BULLET_INITIAL_SPEED + 0.5);
        shiftCollider();
    }

    void GenerateRandomAngle(float speed)
    {
        float angle;
        float dxNew, dyNew;

        int coin = rand() % 2;
        if (coin & 1)
            angle = (rand() % 41 - 65) * M_PI / 180.0; // [-65, -25]
        else
            angle = (rand() % 41 + 25) * M_PI / 180.0; // [25, 65]

//        cout << "New angle: " << angle << endl;

        dxNew = speed * cos(angle);
        dyNew = speed * sin(angle);

        dx = dxNew;
        dy = dyNew;
    }


    void render()
    {
        graphics.renderTexture(bulletTexture, x, y);
    }
};

struct BulletManager
{
    Bullet bullet1;
    Bullet bullet2;

    bool is2BulletsEvent = false;
    Uint32 eventStartTime = 0;
    Uint32 startTime = 0;

    BulletManager(Graphics& graphics, Asset& assets, Sound& sounds)
        : bullet1(graphics, assets, sounds), bullet2(graphics, assets, sounds) {}

    void handleLogic(BlueShip& blueShip, RedShip& redShip)
    {
        int timeLeft = handleTimeInterval(bullet1.graphics, bullet1.assets, startTime);
        bullet1.handleLogic(blueShip, redShip, timeLeft);
        if (is2BulletsEvent) bullet2.handleLogic(blueShip, redShip, timeLeft);
    }

    void resetRound(BlueShip& blueShip, RedShip& redShip)
    {
        is2BulletsEvent = (rand() % 100 < 30);

        if (!is2BulletsEvent)
        {
            bullet1.resetBullet(blueShip, redShip);
        }
        else
        {
            bullet1.resetBullet(blueShip, redShip);
            bullet2.resetBullet(blueShip, redShip);
            setBulletPosition();
            eventStartTime = SDL_GetTicks();
        }

        startTime = SDL_GetTicks();
    }

    void setBulletPosition()
    {
        float centerX = SCREEN_WIDTH / 2.0 - BULLET_WIDTH / 2;
        float centerY = SCREEN_HEIGHT / 2.0 - BULLET_HEIGHT / 2;
        float h = INITIAL_BULLET_DISTANCE_FROM_CENTER;

        bullet1.x = centerX - h;
        bullet2.x = centerX + h;
        bullet1.y = bullet2.y = centerY;

        bullet1.shiftCollider();
        bullet2.shiftCollider();
    }

    void render(BlueShip& blueShip, RedShip& redShip)
    {
        if (is2BulletsEvent && SDL_GetTicks() - eventStartTime <= 3000)
        {
            SDL_Color yellow = {255, 255, 0, 255};
            SDL_Texture* textTexture = bullet1.graphics.renderText("EVENT: 2 BULLETS", bullet1.assets.font20, yellow);

            if (textTexture)
            {
                int textW, textH;
                SDL_QueryTexture(textTexture, NULL, NULL, &textW, &textH);
                int textX = SCREEN_WIDTH / 2 - textW / 2;
                int textY = SCREEN_HEIGHT / 2 - 100;

                bullet1.graphics.renderTexture(textTexture, textX, textY);
                SDL_DestroyTexture(textTexture);
            }
        }

        if (!bullet1.roundEnded)
            bullet1.render();

        if (is2BulletsEvent && !bullet2.roundEnded)
            bullet2.render();

        if (bullet1.roundEnded && (!is2BulletsEvent || bullet2.roundEnded))
        {
            resetRound(blueShip, redShip);
        }
    }
};

#endif // _BULLET__H
