#ifndef _BULLET__H
#define _BULLET__H

#include "graphics.h"
#include "defs.h"
#include <vector>
#include "ship_motion.h"
#include "collision.h"

using namespace std;

struct Bullet
{
    Graphics& graphics;
    SDL_Texture* texture;
    int x = 0, dx = 2;
    int y = 0, dy = 2;
    vector<SDL_Rect> mColliders;

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

    void move(BlueShip& blueShip, RedShip& redShip)
    {
        x += dx;
        shiftCollider();
        if(x < 0 || x + BULLET_WIDTH > SCREEN_WIDTH || checkCollision(mColliders, blueShip.mBlueCollider) || checkCollision(mColliders, redShip.mRedCollider))
        {
            dx = -dx;
            shiftCollider();
        }

        y += dy;
        shiftCollider();
        if(y < 0 || y + BULLET_HEIGHT > SCREEN_HEIGHT || checkCollision(mColliders, blueShip.mBlueCollider) || checkCollision(mColliders, redShip.mRedCollider))
        {
            dy = -dy;
            shiftCollider();
        }
    }

    void render()
    {
        graphics.renderTexture(texture, x, y);
    }
};

#endif // _BULLET__H
