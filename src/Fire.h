#pragma once

#include "Include.h"

class Fire {
public:
    Fire(int x, int y, int directionX, int directionY, SDL_Renderer* renderer);
    ~Fire();
    void update();
    void render(SDL_Renderer* renderer) const;
    static void destroyTexture();
    bool isOutOfScreen();
    static void loadTexture(SDL_Renderer* renderer);
    static void Set2Sound();
    static void Clear2Sound();

private:
    int m_x;
    int m_y;
    int m_directionX;
    int m_directionY;
    int m_speed;
    int m_frame;
    int m_frameCount;
    int m_frameDelay;
    int m_frameTimer;

    //~ 메모리 절약을 위해 재사용되는 텍스쳐는 정적 변수로 선언
    static SDL_Texture* m_texture;
    SDL_Rect m_sourceRects[3];

    //~ 메모리 절약을 위해 재사용되는 사운드는 정적 변수로 선언
    static Mix_Chunk* m_sound;
};