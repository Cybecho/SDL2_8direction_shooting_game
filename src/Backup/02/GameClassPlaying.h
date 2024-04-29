#pragma once

#include "Include.h"

class GameClassPlaying {
public:
    GameClassPlaying();
    ~GameClassPlaying();

    void InitGame();
    void HandleEvents();
    void Update();
    void Render();
    void ClearGame();

    bool IsRunning() const { return m_flag_running; }
    Uint32 GetFramePerSec() const { return m_frame_per_sec; }

private:
    void Set2Music();
    void Set2ReloadSound();
    void RenderScore();
    void RenderMagazine();
    void RenderReloadButton();

    bool m_flag_running;
    SDL_Renderer* m_renderer;
    Uint32 m_frame_per_sec;
    int m_maxFireCount;
};