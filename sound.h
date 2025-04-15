#ifndef _SOUND__H
#define _SOUND__H

#include "graphics.h"
#include "Assets.h"
#include "GameState.h"

enum GameState;

struct Sound
{
    Graphics& graphics;
    Asset& assets;

    Mix_Music* ingameMusic = NULL;
    Mix_Music* menuMusic = NULL;

    Mix_Chunk* clickedSound = NULL;
    Mix_Chunk* collisionSound = NULL;
    Mix_Chunk* gameoverSound = NULL;
    Mix_Chunk* hoverSound = NULL;
    Mix_Chunk* loseSound = NULL;
    Mix_Chunk* winSound = NULL;
    Mix_Chunk* gamepointSound = NULL;

    Mix_Music* currentPlaying = NULL;
    bool isPaused = false;
    bool isMuted_menuMusic = false;
    bool bgmMuted = false;
    bool collisionMuted = false;
    bool pointMuted = false;

    Sound(Graphics& g, Asset& _assets) : graphics(g), assets(_assets)
    {
        ingameMusic = assets.ingameMusic;
        menuMusic = assets.menuMusic;

        clickedSound = assets.clickedSound;
        collisionSound = assets.collisionSound;
        gameoverSound = assets.gameoverSound;
        hoverSound = assets.hoverSound;
        loseSound = assets.loseSound;
        winSound = assets.winSound;
        gamepointSound = assets.gamepointSound;
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
        if (currentPlaying == newMusic && !isPaused) return;

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
            Mix_VolumeMusic(isMuted_menuMusic ? 0 : MIX_MAX_VOLUME);
        }
        else
        {
            Mix_VolumeMusic(MIX_MAX_VOLUME);
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
