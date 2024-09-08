#include "Game.h"
#include "Gamestate.h"
#include "AI.h"

Game *game = nullptr;
Gamestate* gs = nullptr;
AI* ai;

int main(int argc , char *argv[])
{
	//initialize the game and gamestate
	game = new Game();
	gs = new Gamestate();
	ai = new AI();
	game->init("Chess", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 400, false); // Ye sdl ka use h window create ho jaati isse 
	game->loadimages();
	gs->init("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
	//gs->init("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
	//gs->init("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
	//initialize the variables

	bool running = game->running();
	std::vector<std::pair<int,int>> clicks;
	std::pair<int, int> sq_selected = {-1,-1};
	bool movemade = false;
	std::vector<std::vector<int>> possible_moves = gs->GenerateAllValidMoves();
	bool playerOne = true;
	bool playerTwo = false;
	bool AIThinking = false;
	bool moveUndone = false;
	//std::cout << possible_moves.size() << std::endl;
	//std::cout << gs->blackKingLocation << std::endl;
	/*for (auto vec : gs->pins)
	{
		std::cout << vec[0] << " " << vec[1] << " " << vec[2] << " " << vec[3] << std::endl;
	}*/
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
				auto p = std::make_pair(row, col);
				//std::cout << sq_selected.first<<" "<<sq_selected.second<< std::endl;
				//std::cout << p.first << " " << p.second << std::endl;
				if (sq_selected == p)
				{
					//////////////std::cout << "duplicate" << std::endl;
					sq_selected = {-1,-1};
					clicks = {};
				}
				else
				{
					/////////////////std::cout << "sq selected" << std::endl;
					sq_selected = {row , col};
					game->drawHighlight(x, y, 50, 50);
					clicks.push_back(sq_selected);
				}
				if (clicks.size() == 2)
				{
					Move* move = new Move();
					move->register_move(clicks[0], clicks[1], gs->board);
					std::pair<int, int> p = { move->start_col, move->start_row }, p2 = {move->end_col,move->end_row};
					//std::cout << move->start_row << " " << move->start_col << " " << move->end_row << " " << move->end_col << std::endl;
					std::vector<int> MMMove = { move->start_row,move->start_col,move->end_row,move->end_col };
					std::vector<int>::iterator it;
					///////////std::cout << "clicks 2" << std::endl;
					//std::cout << gs->whiteToMove << std::endl;
					if (std::find(possible_moves.begin(), possible_moves.end(), MMMove) != possible_moves.end())
					{
						//std::cout << "here" << std::endl;
						gs->makemove(move);
						possible_moves.clear();
						movemade = true;
					}
					if (!movemade)
					{
						sq_selected = {-1,-1};
						clicks = {};
					}
					//std::cout << gs->whiteToMove << std::endl;
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
					//std::cout << gs->whiteToMove << std::endl;
				}
			}
		}

		if (!humanturn && !moveUndone)
		{
			if (!AIThinking)
			{
				AIThinking = true;
				//Move* AImove = ai->findRandomMove(gs,possible_moves);
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
			//std::cout << gs->movelog.size()<<std::endl;
			possible_moves = gs->GenerateAllValidMoves();
			//std::cout << gs->inCheck << std::endl;
			/*for (auto move:possible_moves)
			{
				std::cout << move[0] << " " << move[1] << " " << move[2] << " " << move[3] << std::endl;
			}*/
			clicks = {};
			sq_selected = {-1,-1};
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