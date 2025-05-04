#ifndef _FPS__H
#define _FPS__H

#include <SDL.h>

struct FPS
{
    int fpsLimit;
    Uint32 frameDelay;
    Uint32 frameStart;
    Uint32 frameTime;
    Uint32 fpsTimer;
    int frameCount;

    FPS(int fpsLimit): fpsLimit(fpsLimit), frameDelay(1000 / fpsLimit),
                        frameStart(0), frameTime(0), fpsTimer(SDL_GetTicks()), frameCount(0) {}

    void startFrame()
    {
        frameStart = SDL_GetTicks();
    }

    void endFrame()
    {
        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < frameDelay)
        {
            SDL_Delay(frameDelay - frameTime);
        }

        // Debug: Count FPS
//        frameCount++;
//        if (SDL_GetTicks() - fpsTimer >= 1000)
//        {
//            std::cout << "FPS: " << frameCount << std::endl;
//            frameCount = 0;
//            fpsTimer = SDL_GetTicks();
//        }
    }
};

#endif // _FPS__H
