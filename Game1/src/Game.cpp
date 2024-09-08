#include "Game.h"
#include <vector>
#include <unordered_map>

std::vector<SDL_Surface*> pieces;
std::vector<SDL_Texture*> pieces_texture;
std::unordered_map<int , SDL_Texture* > mp;
std::vector<const char*> pieces_name = { "vendor/images/bB.png","vendor/images/bK.png","vendor/images/bN.png","vendor/images/bP.png","vendor/images/bQ.png","vendor/images/bR.png","vendor/images/wB.png","vendor/images/wK.png","vendor/images/wN.png","vendor/images/wP.png","vendor/images/wQ.png","vendor/images/wR.png" }; 
std::vector<int> piece_values = { 20 , 17 , 19 , 18 , 22 , 21 , 12 , 9 , 11 , 10 , 14 , 13};
SDL_Rect drect;
Game::Game()
{
}

Game::~Game()
{
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
		flags = SDL_WINDOW_FULLSCREEN;
	else
		flags = SDL_WINDOW_SHOWN;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Initialized" << std::endl;
		m_window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (m_window)
			std::cout << "Window created" << std::endl;
		m_renderer = SDL_CreateRenderer(m_window, -1, 0);
		if (m_renderer)
		{
			SDL_SetRenderDrawColor(m_renderer, 0, 255, 0, 255);
			std::cout << "Renderer created" << std::endl;
		}
		m_isRunning = true;
	}
	else
		m_isRunning = false;
}

void Game::handleEvents()
{

}

void Game::update()
{
}

void Game::render()
{
	SDL_RenderClear(m_renderer);
	bool white = true;
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			SDL_Rect square = { x * 50, y * 50, 50, 50 };

			if (white) {
				SDL_SetRenderDrawColor(m_renderer, 255, 206, 158, 255);  // Light square color
			}
			else {
				SDL_SetRenderDrawColor(m_renderer, 209, 139, 71, 255);   // Dark square color
			}

			SDL_RenderFillRect(m_renderer, &square);
			white = !white;
		}
		white = !white;
	}
	//SDL_RenderPresent(m_renderer);
}

void Game::clean()
{
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_Quit();
	std::cout << "Game cleaned " << std::endl;
}

SDL_Texture* loadTexture(const char* path , SDL_Renderer* renderer) {
	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL) {
		std::cerr << "Unable to load image " << path << "! SDL_image Error: " << IMG_GetError() << std::endl;
		return NULL;
	}

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
	if (texture == NULL) {
		std::cerr << "Unable to create texture from " << path << "! SDL Error: " << SDL_GetError() << std::endl;
	}

	SDL_FreeSurface(loadedSurface);
	return texture;
}

void Game::loadimages()
{
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		std::cerr << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << std::endl;
		return;
	}
	for (int i = 0;i <pieces_name.size();i++)
	{
		const char* path = pieces_name[i];
		SDL_Texture* player_tex = loadTexture(path, m_renderer);
		if (player_tex != NULL)
		{
			pieces_texture.push_back(player_tex);
		} 
		mp[piece_values[i]] = player_tex;
	}
	/*playertex = SDL_CreateTextureFromSurface(m_renderer, tmpsurface);
	SDL_FreeSurface(tmpsurface);*/
}

void Game::create_fen(std::vector<std::vector<int>> board)
{
	drect.w = 49;
	drect.h = 49;
	drect.x = 0;
	drect.y = 0;
	for (int i = 0;i < 8;i++)
	{
		for (int j = 0;j < 8;j++)
		{
			drect.x = 50*j;
			drect.y = 50*i;
			if (mp.find(board[i][j]) != mp.end())
			{
				SDL_RenderCopy(m_renderer, mp[board[i][j]], NULL, &drect);
			}
		}
	}
	/*SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
	SDL_Rect highlightRect = {100,100,50,50};
	SDL_RenderFillRect(m_renderer, &highlightRect);*/
	SDL_RenderPresent(m_renderer);

}

void Game::drawHighlight(int x, int y, int width, int height)
{
	SDL_SetRenderDrawColor(m_renderer, 255, 0, 0, 255);
	SDL_Rect highlightRect;
	x = x / 50;
	x = x * 50;
	y = y / 50;
	y = y * 50;
	highlightRect.x = x;
	highlightRect.y = y;
	highlightRect.w = width;
	highlightRect.h = height;
	SDL_RenderFillRect(m_renderer, &highlightRect);
	SDL_RenderPresent(m_renderer);
	SDL_Delay(20);
}

