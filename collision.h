#ifndef _COLLISION__H
#define _COLLISION__H

#include <iostream>
#include <vector>
#include <SDL.h>

using namespace std;

bool checkCollision(const vector<SDL_Rect>& a, const vector<SDL_Rect>& b)
{
    int LeftA, RightA, TopA, BottomA;
    int LeftB, RightB, TopB, BottomB;

    for (int i = 0; i < (int)a.size(); i++)
    {
        LeftA = a[i].x; RightA = a[i].x + a[i].w; TopA = a[i].y; BottomA = a[i].y + a[i].h;
        for(int j = 0; j < (int)b.size(); j++)
        {
            LeftB = b[j].x; RightB = b[j].x + b[j].w; TopB = b[j].y; BottomB = b[j].y + b[j].h;
            if( !(TopA >= BottomB || BottomA <= TopB || LeftA >= RightB || LeftB >= RightA) ) return true;
        }
    }

    return false;
}

#endif // _COLLISION__H
