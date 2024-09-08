#include "Move.h"

Move::Move()
{
}

Move::~Move()
{
}

void Move::register_move(std::pair<int, int>& st_sq, std::pair<int, int>& end_sq, std::vector<std::vector<int>>& board)
{
	start_col = st_sq.second;
	start_row = st_sq.first;
	end_col = end_sq.second;
	end_row = end_sq.first;
	piece_moved = board[start_row][start_col];
	piece_captured = board[end_row][end_col];
	isCaptured = piece_captured != 0 ? true : false;
}

