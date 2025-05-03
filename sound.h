#ifndef _SOUND__H
#define _SOUND__H

#include "graphics.h"
#include "Assets.h"
#include "GameState.h"

struct Sound
{
    Graphics& graphics;
    Asset& assets;

    Mix_Music* ingameMusic = NULL;
    Mix_Music* menuMusic = NULL;

    Mix_Music* currentPlaying = NULL;

    bool isPaused = false;
    bool isMuted_menuMusic = false;

    bool bgmMuted = false;
    bool collisionMuted = false;
    bool pointMuted = false;
    const int LOW_VOLUME = MIX_MAX_VOLUME / 2;

    Sound(Graphics& g, Asset& _assets) : graphics(g), assets(_assets)
    {
        ingameMusic = assets.ingameMusic;
        menuMusic = assets.menuMusic;
    }

    void stopMusic()
    {
        Mix_HaltMusic();
        currentPlaying = NULL;
        isPaused = false;
    }

    void pauseMusic()
    {
        if (Mix_PlayingMusic() && !isPaused)
        {
            Mix_PauseMusic();
            isPaused = true;
        }
    }

    void resumeMusic()
    {
        if (isPaused)
        {
            Mix_ResumeMusic();
            isPaused = false;
        }
    }

    void switchMusicTo(Mix_Music* newMusic)
    {
        if (currentPlaying == newMusic) return;

        stopMusic();
        if (newMusic)
        {
            graphics.play(newMusic);
            currentPlaying = newMusic;
        }
    }

    void toggleMuteMenuMusic()
    {
        isMuted_menuMusic = !isMuted_menuMusic;
        if (isMuted_menuMusic) Mix_VolumeMusic(0);
        else Mix_VolumeMusic(MIX_MAX_VOLUME);
        updateVolume();
    }

    void updateVolume()
    {
        if (currentPlaying == ingameMusic)
        {
            Mix_VolumeMusic(bgmMuted ? 0 : MIX_MAX_VOLUME);
        }
        else if (currentPlaying == menuMusic)
        {
            Mix_VolumeMusic(isMuted_menuMusic ? 0 : LOW_VOLUME);
        }
        else
        {
            Mix_VolumeMusic(LOW_VOLUME);
        }
    }

    void playMusicforState(GameState currentState)
    {
        switch (currentState)
        {
            case EXIT:
                stopMusic();
                break;
            case PAUSED:
                pauseMusic();
                break;
            case PLAY:
                if (isPaused) resumeMusic();
                else switchMusicTo(ingameMusic);
                break;
            default:
                switchMusicTo(menuMusic);
                break;
        }
        updateVolume();
    }
};

#endif // _SOUND__H
