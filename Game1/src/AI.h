#pragma once
#include "Gamestate.h"
#include <unordered_map>


class AI
{
public:
	Move* findRandomMove(Gamestate* gs, std::vector<std::vector<int>>& validMoves);
	Move* FindBestMove(Gamestate* gs, std::vector<std::vector<int>> &validMoves);
	double MinMaxAlphaBeta(Gamestate* gs, std::vector<std::vector<int>>& validMoves, int depth, double alpha, double beta, int turnMultiplier);

	int Depth = 3;
	int checkmate = 100000;
	int stalemate = 0;
	Move* nextMove ;
};

