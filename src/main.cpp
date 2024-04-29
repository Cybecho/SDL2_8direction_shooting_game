#include "GameClass.h"
#include "GameClassPlaying.h"
#include "GameClassIntro.h"
#include "GameClassEnding.h"


SDL_Window* g_window;
SDL_Renderer* g_renderer;
Uint32 g_last_time_ms;

std::vector<GameClassIntro> g_intro;
std::vector<GameClassEnding> g_ending;
std::vector<GameClassPlaying> g_playing;

int g_current_game_phase;
int g_count_game_phase;
bool g_mute;

//~ 오디오
Mix_Music* intro_music = NULL;
Mix_Music* ending_music = NULL;

int main(int argc, char* argv[]) {
    // Initializing SDL library
    SDL_Init(SDL_INIT_EVERYTHING);

    TTF_Init();

    // 오디오 초기화
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        std::cout << "Mix_OpenAudio " << Mix_GetError() << std::endl;
        exit(1);
    }

    //~ 음악 로드
    intro_music = Mix_LoadMUS("../Resources/Zenith_Zone.mp3");
    ending_music = Mix_LoadMUS("../Resources/Kalimba.mp3");


    g_window = SDL_CreateWindow("201921408 SBU", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    g_renderer = SDL_CreateRenderer(g_window, -1, 0);

    g_current_game_phase = PHASE_INTRO;
    g_count_game_phase = 0;
    g_mute = false;

    GameClassIntro intro;
    GameClassPlaying game;
    GameClassEnding ending;

    game.InitGame();
    ending.InitEnding();
    intro.InitIntro();

    g_last_time_ms = SDL_GetTicks();

    while (game.IsRunning()) {
        Uint32 cur_time_ms = SDL_GetTicks();

        if (cur_time_ms - g_last_time_ms < (1000 / game.GetFramePerSec()))
            continue;

        if (g_current_game_phase == PHASE_INTRO)
        {
            intro.HandleEvents();
            intro.Update();
            intro.Render();
        }
        else if (g_current_game_phase == PHASE_PLAYING)
        {
            game.HandleEvents();
            game.Update();
            game.Render();
        }
        else if (g_current_game_phase == PHASE_ENDING)
        {
            ending.HandleEvents();
            ending.Update();
            ending.Render();
        }
        else
        {
            std::cout << "Invalid game phase" << std::endl;
            break;
        }

        g_last_time_ms = cur_time_ms;
    }

    for (auto& intro : g_intro) {
        intro.~GameClassIntro();
    }
    g_intro.clear();
    for (auto& ending : g_ending) {
		ending.~GameClassEnding();
	}
    g_ending.clear();
    for (auto& game : g_playing) {
		game.~GameClassPlaying();
	}
    g_playing.clear();

    Mix_FreeMusic(intro_music);
    Mix_FreeMusic(ending_music);

    SDL_DestroyRenderer(g_renderer);
    SDL_DestroyWindow(g_window);

    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();

    return 0;
}