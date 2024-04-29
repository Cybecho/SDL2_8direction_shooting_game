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

}

GameClassIntro::~GameClassIntro()
{
	SDL_DestroyTexture(texture_);
}

void GameClassIntro::InitIntro()
{
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
			}
			break;

		default:
			break;
		}
	}
}

void GameClassIntro::ClearIntro() 
{
}