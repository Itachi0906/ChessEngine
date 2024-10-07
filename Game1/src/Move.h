#pragma once
#include <iostream>
#include <vector>
#include "Position.h"

class Move
{
public:
	Move();
	~Move();
	void registerMove(Position st_sq, Position end_sq, std::vector<std::vector<int>>& board);
	int startRow;
	int startCol;
	int endRow;
	int endCol;
	int pieceMoved;
	int pieceCaptured;
	bool isCaptured;
	bool isCastleMove = false;
	int changedPiece = -1;
};

