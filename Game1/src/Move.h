#pragma once
#include <iostream>
#include <vector>

class Move
{
public:
	Move();
	~Move();
	
	void register_move(std::pair<int, int>& st_sq, std::pair<int, int>& end_sq, std::vector<std::vector<int>>& board);


	int start_row;
	int start_col;
	int end_row;
	int end_col;
	int piece_moved;
	int piece_captured;
	bool isCaptured;
	bool isCastleMove = false;
	int changedPiece = -1;
};

