#include "GameClass.h"
#include "GameClassEnding.h"


GameClassEnding::GameClassEnding()
{
	// For Texture

	SDL_Surface* temp_surface = IMG_Load("../Resources/ending.png");
	texture_ = SDL_CreateTextureFromSurface(g_renderer, temp_surface);
	SDL_FreeSurface(temp_surface);

	SDL_QueryTexture(texture_, NULL, NULL, &source_rectangle_.w, &source_rectangle_.h);
	destination_rectangle_.x = 0;
	destination_rectangle_.y = 0;
	source_rectangle_.x = 0;
	source_rectangle_.y = 0;
	destination_rectangle_.w = source_rectangle_.w;
	destination_rectangle_.h = source_rectangle_.h;

}

GameClassEnding::~GameClassEnding()
{
	SDL_DestroyTexture(texture_);
}

void GameClassEnding::InitEnding() 
{
}

void GameClassEnding::Update()
{
}


void GameClassEnding::Render()
{
	SDL_SetRenderDrawColor(g_renderer, 255, 255, 0, 255);
	SDL_RenderClear(g_renderer); // clear the renderer to the draw color

	SDL_RenderCopy(g_renderer, texture_, &source_rectangle_, &destination_rectangle_);

	SDL_RenderPresent(g_renderer); // draw to the screen
}



void GameClassEnding::HandleEvents()
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

			// If the mouse left button is pressed. 
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				g_current_game_phase = PHASE_INTRO;
			}
			break;

		default:
			break;
		}
	}
}

void GameClassEnding::ClearEnding() 
{
}