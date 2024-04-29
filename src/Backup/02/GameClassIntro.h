#pragma once

#include "Include.h"

class GameClassIntro
{
public:
	GameClassIntro();
	~GameClassIntro();
	void InitIntro();
	void HandleEvents();
	void Update();
	void Render();
	void ClearIntro();

private:
	SDL_Texture* texture_; // the SDL_Texture 
	SDL_Rect source_rectangle_; // the rectangle for source image
	SDL_Rect destination_rectangle_; // for destination
};
