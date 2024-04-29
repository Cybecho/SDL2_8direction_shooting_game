#include "Fire.h"
#include "Include.h"

SDL_Texture* Fire::m_texture = nullptr;
Mix_Chunk* Fire::m_sound = nullptr;

Fire::Fire(int x, int y, int directionX, int directionY, SDL_Renderer* renderer)
    : m_x(x), m_y(y), m_directionX(directionX), m_directionY(directionY), m_speed(FIRE_SPEED),
    m_frame(0), m_frameCount(3), m_frameDelay(5), m_frameTimer(0) {
    if (!m_texture) {
        loadTexture(renderer);
    }

    if (!m_sound) {
        Set2Sound();
    }

    Mix_PlayChannel(-1, m_sound, 0);

    for (int i = 0; i < m_frameCount; ++i) {
        m_sourceRects[i] = { i * 200, 0, 200, 200 };
    }

    cout << "Fire Create" << " x : " << this->m_x << " y : " << this->m_y << endl;
}

Fire::~Fire() {
	cout << "Fire Delete" << endl;
}

void Fire::update() {
    m_x += m_directionX * m_speed;
    m_y += m_directionY * m_speed;

    ++m_frameTimer;
    if (m_frameTimer >= m_frameDelay) {
        m_frameTimer = 0;
        ++m_frame;
        if (m_frame >= m_frameCount) {
            m_frame = 0;
        }
    }
}

void Fire::Set2Sound() {
    if (!m_sound) {
        m_sound = Mix_LoadWAV("../Resources/Bullett_Classic.mp3");
        if (!m_sound) {
            printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        }
        else {
            // 사운드 볼륨을 50%로 설정 (0 ~ 128 범위)
            Mix_VolumeChunk(m_sound, 32);
        }
    }
}

void Fire::Clear2Sound() {
    if (m_sound) {
        Mix_FreeChunk(m_sound);
        m_sound = nullptr;
    }
}

void Fire::render(SDL_Renderer* renderer) const {
    SDL_Rect dstRect = { m_x-100, m_y-100, 200, 200 };
    SDL_RenderCopy(renderer, m_texture, &m_sourceRects[m_frame], &dstRect);
}

bool Fire::isOutOfScreen() {
    return m_x < -200 || m_x > WINDOW_WIDTH || m_y < -200 || m_y > WINDOW_HEIGHT;
}

void Fire::loadTexture(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("../Resources/fire.png");
    m_texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

void Fire::destroyTexture() {
    SDL_DestroyTexture(m_texture);
    m_texture = nullptr;
}