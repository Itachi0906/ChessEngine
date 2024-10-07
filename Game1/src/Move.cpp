#include "Move.h"

Move::Move()
{
}

Move::~Move()
{
}

void Move::registerMove(Position st_sq, Position end_sq, std::vector<std::vector<int>>& board)
{
	startCol = st_sq.getFile();
	startRow = st_sq.getRank();
	endCol = end_sq.getFile();
	endRow = end_sq.getRank();
	pieceMoved = board[startRow][startCol];
	pieceCaptured = board[endRow][endCol];
	isCaptured = pieceCaptured != 0 ? true : false;
}

