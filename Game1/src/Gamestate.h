#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <set>
#include "Move.h"
#include "Position.h"

class Gamestate
{
public:
	Gamestate();
	~Gamestate();

	void init(std::string fen);
	void makemove(Move* move);
	void undomove(std::vector<Move*>& movelog, std::vector<std::vector<int>>& board);
	void checkForPinsAndChecks();
	std::vector<std::vector<int>>  GenerateAllMoves();
	std::vector<std::vector<int>>  GenerateAllValidMoves();

	bool whiteToMove;
	int halfmoveCounter;
	int moveCounter;
	int whiteKingLocation;
	int blackKingLocation;
	std::vector<std::vector<int>> board;
	std::vector<bool> castling_rights;
	int enpassant_target_square = -1;
	std::vector<Move*> movelog;
	bool inCheck = false;
	std::vector<std::vector<int>> checks;
	std::vector<std::vector<int>> pins;
	std::vector<int> enPassantPossibleLog;
	std::vector<std::vector<bool>> CastleRightsLog;

private:
	void generatePawnMoves(std::vector<std::vector<int>>& possible_moves, int r, int c);
	void generateSlidingMoves(std::vector<std::vector<int>>& possible_moves, int r, int c, int type);
	void generateKingMoves(std::vector<std::vector<int>>& possible_moves, int r, int c);
	void generateKnightMoves(std::vector<std::vector<int>>& possible_moves, int r, int c);
	void getCastleMoves(std::vector<std::vector<int>>& possible_moves, int r, int c);
	void getKingsideCastleMoves(std::vector<std::vector<int>>& possible_moves, int r, int c);
	void getQueensideCastleMoves(std::vector<std::vector<int>>& possible_moves, int r, int c);
	void updateCastlingRights(Move* move);
	enum pieceMapping {
		White = 8,
		WhiteKing,
		WhitePawn,
		WhiteKnight,
		WhiteBishop,
		WhiteRook,
		WhiteQueen,
		Black = 16,
		BlackKing,
		BlackPawn,
		BlackKnight,
		BlackBishop,
		BlackRook,
		BlackQueen
	};
};

