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
    SDL_Texture* texture = NULL;
    float x = (SCREEN_WIDTH - BULLET_WIDTH) / 2, dx = BULLET_INITIAL_SPEED;
    float y = (SCREEN_HEIGHT - BULLET_HEIGHT) / 2, dy = BULLET_INITIAL_SPEED;
    vector<SDL_Rect> mColliders;
    bool roundEnded = false;
    bool isDamaged = false;

    Bullet(Graphics& g, Asset& _assets, Sound& _sounds): graphics(g), assets(_assets), sounds(_sounds)
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
            bool collidedX = false;
            bool collidedY = false;

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
                x -= dx;
                dx = -dx;

                dx = dx > 0 ? dx + 0.2 : dx - 0.2;

                if (dx > 0) x += 1.5;
                else x -= 1.5;

                shiftCollider();
                collidedX = true;
                if (!sounds.collisionMuted) graphics.play(assets.collisionSound);
            }

            y += dy;
            shiftCollider();

            if (checkCollision(mColliders, blueShip.mBlueCollider) || checkCollision(mColliders, redShip.mRedCollider))
            {
                y -= dy;
                dy = -dy;

                dy = dy > 0 ? dy + 0.2 : dy - 0.2;

                if (dy > 0) y += 1.5;
                else y -= 1.5;

                shiftCollider();
                collidedY = true;
                if (!sounds.collisionMuted) graphics.play(assets.collisionSound);
            }


            if (collidedX && collidedY)
            {
                float curSpeed = sqrt(dx * dx + dy * dy);
                GenerateRandomAngle(curSpeed);
            }

            roundEnded = (y + BULLET_HEIGHT < BLUE_SHIP_RESTRICTED_LINE_Y - DEADZONE_Y || y > RED_SHIP_RESTRICTED_LINE_Y + DEADZONE_Y);

            if (roundEnded && !isDamaged)
            {
                if (!sounds.pointMuted)
                    graphics.play(assets.gamepointSound);

                if (y + BULLET_HEIGHT < BLUE_SHIP_RESTRICTED_LINE_Y - DEADZONE_Y)
                    blueShip.healthLoss += HEALTH_BAR_WIDTH / 4;
                else if (y > RED_SHIP_RESTRICTED_LINE_Y + DEADZONE_Y)
                    redShip.healthLoss += HEALTH_BAR_WIDTH / 4;

                isDamaged = true;
            }

        }
    }

    void resetBullet(BlueShip& blueShip, RedShip& redShip)
    {
        x = SCREEN_WIDTH / 2 - BULLET_WIDTH / 2;
        y = SCREEN_HEIGHT / 2 - BULLET_HEIGHT / 2;

        dx = BULLET_INITIAL_SPEED;
        dy = BULLET_INITIAL_SPEED;

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
        if (coin == 0)
            angle = (rand() % 41 - 65) * M_PI / 180.0; // [-65, -25]
        else
            angle = (rand() % 41 + 25) * M_PI / 180.0; // [25, 65]

        dxNew = speed * cos(angle);
        dyNew = speed * sin(angle);

        dx = dxNew;
        dy = dyNew;
    }


    void render()
    {
        graphics.renderTexture(texture, x, y);
    }
};

struct BulletManager
{
    Bullet bullet1;
    Bullet bullet2;
    Uint32 eventStartTime = 0;

    bool is2BulletsEvent = false;

    BulletManager(Graphics& graphics, Asset& assets, Sound& sounds)
        : bullet1(graphics, assets, sounds), bullet2(graphics, assets, sounds) {}

    void update(BlueShip& blueShip, RedShip& redShip)
    {
        if( !is2BulletsEvent && (HEALTH_BAR_WIDTH - blueShip.healthLoss <= HEALTH_BAR_WIDTH / 2) &&
           (HEALTH_BAR_WIDTH - redShip.healthLoss <= HEALTH_BAR_WIDTH / 2))
        {
            is2BulletsEvent = true;
            bullet1.dx = BULLET_INITIAL_SPEED + 0.5; bullet1.dy = BULLET_INITIAL_SPEED + 0.5;
            bullet2.dx = BULLET_INITIAL_SPEED + 0.5; bullet2.dx = BULLET_INITIAL_SPEED + 0.5;
            eventStartTime = SDL_GetTicks();
        }
    }

    void handleLogic(BlueShip& blueShip, RedShip& redShip)
    {
        bullet1.handleLogic(blueShip, redShip);
        if (is2BulletsEvent) bullet2.handleLogic(blueShip, redShip);
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
            bullet1.resetBullet(blueShip, redShip);
            if (is2BulletsEvent)
                bullet2.resetBullet(blueShip, redShip);

            startTime = SDL_GetTicks();
        }
    }
};

#endif // _BULLET__H
