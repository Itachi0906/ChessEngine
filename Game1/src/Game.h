#pragma once

#include "SDL.h"
#include <iostream>
#include "SDL_image.h"
#include <vector>

class Game
{
public:
	Game();
	~Game();
	void init( const char* title, int xpos, int ypos, int width, int height, bool fullscreen);
	void handleEvents();
	void update();
	void render();
	void clean();
	void loadimages();
	void create_fen(std::vector<std::vector<int>> board);
	void drawHighlight(int x, int y, int width, int height);
	bool running()
	{
		return m_isRunning;
	};
private:
	bool m_isRunning;
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
};

