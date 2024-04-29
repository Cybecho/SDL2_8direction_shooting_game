#include "GameClass.h"
#include "GameClassIntro.h"

GameClassIntro::GameClassIntro()
{
	// For Texture

	SDL_Surface* temp_surface = IMG_Load("../Resources/intro.png");
	texture_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);

	SDL_QueryTexture(texture_, NULL, NULL, &source_rectangle_.w, &source_rectangle_.h);
	destination_rectangle_.x = source_rectangle_.x = 0;
	destination_rectangle_.y = source_rectangle_.y = 0;
	destination_rectangle_.w = source_rectangle_.w;
	destination_rectangle_.h = source_rectangle_.h;

	cout << "***** Game Intro Phase Constructor Called *****" << endl;
}

GameClassIntro::~GameClassIntro()
{
	SDL_DestroyTexture(texture_);
	Mix_FreeMusic(intro_music);
	cout << "***** Game Intro Phase Destructor Called *****" << endl;
}

void GameClassIntro::InitIntro()
{
	//~ 오디오
	Mix_PlayMusic(intro_music, -1);
}

void GameClassIntro::Update()
{
}


void GameClassIntro::Render()
{
	SDL_SetRenderDrawColor(g_renderer, 255, 255, 255, 255);
	SDL_RenderClear(g_renderer); // clear the renderer to the draw color

	SDL_RenderCopy(g_renderer, texture_, &source_rectangle_, &destination_rectangle_);

	SDL_RenderPresent(g_renderer); // draw to the screen
}



void GameClassIntro::HandleEvents()
{
	SDL_Event event;
	if (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				g_current_game_phase = PHASE_PLAYING;
				Mix_HaltMusic(); // 인트로 배경음악 정지
			}
			break;

		default:
			break;
		}
	}
}

void GameClassIntro::ClearIntro() 
{
	//~ 오디오
	Mix_HaltMusic();
}