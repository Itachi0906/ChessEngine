#include "Game.h"
#include "Gamestate.h"
#include "AI.h"

Game *game = nullptr;
Gamestate* gs = nullptr;
AI* ai;

bool compare(Position p1, Position p2)
{
	if (p1.getRank() != p2.getRank() || p1.getFile() == p2.getFile())
		return false;
	return true;
}

int main(int argc , char *argv[])
{
	//initialize the game and gamestate
	game = new Game();
	gs = new Gamestate();
	ai = new AI();
	game->init("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 400, false); // Ye sdl ka use h window create ho jaati isse 
	game->loadimages();
	gs->init("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	bool running = game->running();
	std::vector<std::pair<int,int>> clicks;
	std::pair<int,int> sq_selected;
	sq_selected = { -1, -1 };
	bool movemade = false;
	std::vector<std::vector<int>> possible_moves = gs->GenerateAllValidMoves();
	bool playerOne = true;
	bool playerTwo = false;
	bool AIThinking = false;
	bool moveUndone = false;
	while (running)
	{
		
		bool humanturn = (gs->whiteToMove & playerOne) | (!gs->whiteToMove & playerTwo);
		SDL_Event event;
		while (SDL_PollEvent(&event) != 0)
		{
			if (event.type == SDL_QUIT)
			{
				running = false;
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN)
			{
				int x, y;
				SDL_GetMouseState(&x, &y);
				int col = x / 50, row = y / 50;
				std::cout << y << " " << x << std::endl;
				std::pair<int,int> p; 
				p = { row, col };
				if (sq_selected == p)
				{
					sq_selected = { -1, -1 };
					clicks = {};
				}
				else
				{
					sq_selected = { row, col };
					game->drawHighlight(x, y, 50, 50);
					clicks.push_back(sq_selected);
				}
				if (clicks.size() == 2)
				{
					Move* move = new Move();
					Position startClick, endClick;
					startClick.instantiate(clicks[0].first, clicks[0].second);
					endClick.instantiate(clicks[1].first, clicks[1].second);
					move->registerMove(startClick, endClick, gs->board);
					std::pair<int, int> p = { move->startCol, move->startRow }, p2 = {move->endCol,move->endRow};
					std::vector<int> MMMove = { move->startRow,move->startCol,move->endRow,move->endCol };
					std::vector<int>::iterator it;
					if (std::find(possible_moves.begin(), possible_moves.end(), MMMove) != possible_moves.end())
					{
						gs->makemove(move);
						possible_moves.clear();
						movemade = true;
					}
					if (!movemade)
					{
						sq_selected = { -1, -1 };
						clicks = {};
					}
				}
			}
			else if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.sym == SDLK_z)
				{
					gs->undomove(gs->movelog, gs->board);
					movemade = true;
					possible_moves.clear();
					if (AIThinking)
						AIThinking = false;
					moveUndone = true;
				}
			}
		}

		if (!humanturn && !moveUndone)
		{
			if (!AIThinking)
			{
				AIThinking = true;
				Move* AImove = ai->FindBestMove(gs, possible_moves);
				gs->makemove(AImove);
				possible_moves.clear();
				AIThinking = false;
				movemade = true;
			}
		}


		if (movemade)
		{
			gs->whiteToMove = !gs->whiteToMove;
			possible_moves = gs->GenerateAllValidMoves();
			if (possible_moves.size() == 0)
			{
				std::cout << "CheckMate" <<' '<< (gs->whiteToMove == 0?"white Wins" : "Black Wins") << '\n';
				break;
			}
			clicks = {};
			sq_selected = { -1, -1 };
			movemade = false;
			moveUndone = false;
		}


		game->update();
		game->render();
		game->create_fen(gs->board);
	}
	game->clean();
	return 0;
}