#pragma once

#include "Include.h"


class GameClassEnding
{
public:
	GameClassEnding();
	~GameClassEnding();
	void InitEnding();
	void HandleEvents();
	void Update();
	void Render();
	void ClearEnding();

private:
	SDL_Texture* texture_; // the SDL_Texture 
	SDL_Rect source_rectangle_; // the rectangle for source image
	SDL_Rect destination_rectangle_; // for destination
};