#include "Gamestate.h"
#include "SDL.h"

Gamestate::Gamestate()
{
}

Gamestate::~Gamestate()
{
}

std::vector<std::string> split(std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	for (char c : s)
	{
		if (c == delimiter)
		{
			tokens.push_back(token);
			token = "";
		}
		else
		{
			token += c;
		}
	}
	tokens.push_back(token);
	return tokens;
}

int get_piece_value(char c)
{
	switch (c)
	{
	case 'K':return 1;
	case 'P':return 2;
	case 'N':return 3;
	case 'B':return 4;
	case 'R':return 5;
	case 'Q':return 6;
	default:
		return 0;
	}
}

std::vector<std::vector<int>> create_board(std::string& fen_board, bool whiteToMove,int &whiteKingLocation, int &blackKingLocation)
{
	std::vector<std::vector<int>> board(8, std::vector<int>(8, 0));
	int row = 0;
	int col = 0;
	for (char c : fen_board)
	{
		if (c == '/')
		{
			row++;
			col = 0;
		}
		else if (isdigit(c))
		{
			col += c - '0';
		}
		else
		{
			if (c == 'K')
				whiteKingLocation = row * 8 + col;
			else if(c == 'k')
				blackKingLocation = row * 8 + col;
			int x = islower(c) ? 16 : 8;
			board[row][col++] = get_piece_value(toupper(c)) | x;
		}
	}
	return board;
}

std::vector<bool> create_castling_rights(std::string& fen_castling_rights)
{
	std::vector<bool> castling_rights(4, false);
	for (char c : fen_castling_rights)
	{
		if (c == 'K')castling_rights[0] = true;
		if (c == 'Q')castling_rights[1] = true;
		if (c == 'k')castling_rights[2] = true;
		if (c == 'q')castling_rights[3] = true;
	}
	return castling_rights;
}

int get_square_index(std::string& square_name)
{
	int file = square_name[0] - 'a';
	int rank = square_name[1] - '0';
	return rank * 8 + file;
}

void Gamestate::init(std::string fen)
{
	//split into components
	std::vector<std::string> fen_parts = split(fen, ' ');
	//turn to move
	whiteToMove = fen_parts[1] == "w" ? true : false;
	//Create board
	board = create_board(fen_parts[0],whiteToMove,whiteKingLocation, blackKingLocation);
	//castling rights
	castling_rights = create_castling_rights(fen_parts[2]);
	CastleRightsLog.push_back(castling_rights);
	//enpassant possible or not
	enpassant_target_square = fen_parts[3] == "-" ? -1 : get_square_index(fen_parts[3]);
	enPassantPossibleLog.push_back(enpassant_target_square);
	//half move count for 50 move rule
	halfmoveCounter = std::stoi(fen_parts[4]);
	// total moves;
	moveCounter = std::stoi(fen_parts[5]);
}

void Gamestate::makemove(Move* move)
{
	board[move->startRow][move->startCol] = 0;
	board[move->endRow][move->endCol] = move->pieceMoved;
	movelog.push_back(move);
	if (move->pieceMoved == WhiteKing)
	{
		whiteKingLocation = move->endRow * 8 + move->endCol;
	}
	else if (move->pieceMoved == BlackKing)
	{
		blackKingLocation = move->endRow * 8 + move->endCol;
	}

	if ((move->pieceMoved == WhitePawn && move->endRow == 0) || (move->pieceMoved == BlackPawn && move->endRow == 7))
	{
		board[move->endRow][move->endCol] = move->pieceMoved + 4;
		move->changedPiece = move->endRow == 0 ? WhitePawn:BlackPawn;
	}


	if (move->pieceMoved == WhiteKing || move->pieceMoved == BlackKing)
	{
		if (move->endCol - move->startCol == 2)
		{
			board[move->endRow][move->endCol - 1] = board[move->endRow][move->endCol + 1];
			board[move->endRow][move->endCol + 1] = 0;
		}
		else if(move->startCol - move->endCol == 2)
		{
			board[move->endRow][move->endCol + 1] = board[move->endRow][move->endCol - 2];
			board[move->endRow][move->endCol - 2] = 0;
		}
	}

	if (move->endRow * 8 + move->endCol == enpassant_target_square && (move->pieceMoved == 10 || move->pieceMoved == 18))
	{
		board[move->startRow][move->endCol] = 0;
	}

	if ((move->pieceMoved == WhitePawn || move->pieceMoved == BlackPawn) && abs(move->startRow - move->endRow) == 2)
	{
		enpassant_target_square = ((move->startRow + move->endRow) / 2) * 8 + move->startCol;
	}
	else
	{
		enpassant_target_square = -1;
	}

	enPassantPossibleLog.push_back(enpassant_target_square);
	updateCastlingRights(move);
	CastleRightsLog.push_back(castling_rights);
}

void Gamestate::undomove(std::vector<Move*>& movelog,std::vector<std::vector<int>> &board)
{
	if (movelog.size() != 0)
	{
		Move* move = movelog[movelog.size() - 1];
		movelog.pop_back();
		board[move->startRow][move->startCol] = move->pieceMoved;
		board[move->endRow][move->endCol] = move->pieceCaptured;

		if ((move->changedPiece == WhitePawn && move->endRow == 0) || (move->changedPiece == BlackPawn && move->endRow == 7))
		{
			board[move->startRow][move->startCol] = move->changedPiece;
		}
		

		enPassantPossibleLog.pop_back();
		enpassant_target_square = enPassantPossibleLog[enPassantPossibleLog.size() - 1];
		if ((move->pieceMoved == WhitePawn || move->pieceMoved == BlackPawn) && abs(move->startRow - move->endRow) == 2)
		{
			enpassant_target_square = -1;
		}

		CastleRightsLog.pop_back();
		castling_rights = CastleRightsLog[CastleRightsLog.size() - 1];
		if (move->pieceMoved == WhiteKing || move->pieceMoved == BlackKing)
		{
			if (move->endCol - move->startCol == 2)
			{
				board[move->endRow][move->endCol + 1] = board[move->endRow][move->endCol - 1]; // shift the rook 
				board[move->endRow][move->endCol - 1] = 0;
			}
			else if(move->startCol - move->endCol == 2)
			{
				board[move->endRow][move->endCol - 2] = board[move->endRow][move->endCol + 1];
				board[move->endRow][move->endCol + 1] = 0;
			}
		}
	}
	else
	{
		whiteToMove = !whiteToMove;
	}
}

std::vector<std::vector<int>> Gamestate::GenerateAllMoves()
{
	std::vector<std::vector<int>> possible_moves;
	for (int i = 0;i < 8;i++)
	{
		for (int j = 0;j < 8;j++)
		{
			if (whiteToMove)
			{
				if (board[i][j] == WhiteRook || board[i][j] == WhiteQueen || board[i][j] == WhiteBishop)
				{
					generateSlidingMoves(possible_moves, i, j,board[i][j] - 8);
				}
				else if (board[i][j] == WhiteKnight)
				{
					generateKnightMoves(possible_moves, i, j);
				}
				else if (board[i][j] == WhitePawn)
				{
					generatePawnMoves(possible_moves, i, j);
				}
				else if (board[i][j] == WhiteKing)
				{
					generateKingMoves(possible_moves, i, j);
				}
			}
			else
			{
				if (board[i][j] == BlackRook || board[i][j] == BlackQueen || board[i][j] == BlackBishop)
				{
					generateSlidingMoves(possible_moves, i, j,board[i][j] - 16);
				}
				else if (board[i][j] == BlackKnight)
				{
					generateKnightMoves(possible_moves, i, j);
				}
				else if (board[i][j] == BlackPawn)
				{
					generatePawnMoves(possible_moves, i, j);
				}
				else if (board[i][j] == BlackKing)
				{
					generateKingMoves(possible_moves, i, j);
				}
			}
		}
	}
	return possible_moves;
}


void Gamestate::checkForPinsAndChecks()
{
	int allyColor, enemyColor, startRow, startCol;
	std::vector<int> possiblePin;
	inCheck = false;
	if (whiteToMove)
	{
		allyColor = White;
		enemyColor = Black;
		startRow = whiteKingLocation/8;
		startCol = whiteKingLocation%8;
	}
	else
	{
		allyColor = Black;
		enemyColor = White;
		startRow = blackKingLocation / 8;
		startCol = blackKingLocation % 8;
	}
	const int dir3[8][2] = {{0,1},{0,-1},{1,0},{-1,0},{-1,1},{-1,-1},{1,1},{1,-1}};
	for (int i = 0;i < 8;i++)
	{
		int dx = dir3[i][0];
		int dy = dir3[i][1];
		possiblePin = {};
		for (int j = 1;j < 8;j++)
		{
			int endRow = startRow + dx * j;
			int endCol = startCol + dy * j;
			if (0 <= endRow && endRow < 8 && 0 <= endCol && endCol < 8)
			{
				int endPiece = board[endRow][endCol];
				int y = endPiece ^ allyColor;
				if (y <= 6 && (endPiece != WhiteKing && endPiece != BlackKing))
				{
					if (possiblePin.size() == 0)
						possiblePin = { endRow , endCol , dx , dy };
					else
						break;
				}
				else if ((endPiece^enemyColor) <= 6)
				{
					int x = endPiece ^ enemyColor;
					if ((0 <= i && i <= 3 && (x) == 5) || (4 <= i && i <= 7 && (x) == 4) || (j == 1 && (x) == 2 && ((enemyColor == White && 6 <= i && i <= 7) || (enemyColor == Black && 4 <= i && i <= 5))) || ((x) == 6) || (j == 1 && (x) == 1))
					{
 						if (possiblePin.size() == 0)
						{
							inCheck = true;
							checks.push_back({ endRow, endCol , dx, dy });
							break;
						}
						else
						{
							pins.push_back(possiblePin);
							break;
						}
					}
					else
						break;
				}

			}
		}
	}
	const int dir2[8][2] = {{1,2} , {1, -2}, {-1,2},{-1,-2},{2,1},{2,-1},{-2,1},{-2,-1}};
	for (int i = 0;i<8;i++)
	{
		int endRow = startRow + dir2[i][0];
		int endCol = startCol + dir2[i][1];
		if (0 <= endRow && endRow < 8 && 0 <= endCol && endCol < 8)
		{
			int endPiece = board[endRow][endCol];
			if ((endPiece^enemyColor) == 3)
			{
				inCheck = true;
				checks.push_back({ endRow, endCol, dir2[i][0], dir2[i][1]});
			}
		}
	}
	return;
}

std::vector<std::vector<int>> Gamestate::GenerateAllValidMoves()
{
	checks.clear();
	checkForPinsAndChecks();
	std::vector<std::vector<int>> ValidMoves;
	int kingRow, kingCol;
	if (whiteToMove)
	{
		kingRow = whiteKingLocation / 8;
		kingCol = whiteKingLocation % 8;
	}
	else
	{
		kingRow = blackKingLocation / 8;
		kingCol = blackKingLocation % 8;
	}
	if (inCheck)
	{
		if (checks.size() == 1)
		{
			std::vector<std::vector<int>> ValidMoves1 = GenerateAllMoves();
			int checkRow = checks[0][0];
			int checkCol = checks[0][1];
			int pieceChecking = board[checkRow][checkCol];
			std::set<std::vector<int>> validSquares;
			if ((pieceChecking ^ 8) == 3 || (pieceChecking ^ 16) == 3)
			{
				validSquares = { { checkRow, checkCol } };
			}
			else
			{
				for (int i = 0;i < 8;i++)
				{
					std::vector<int> validSquare = { kingRow + checks[0][2] * i , kingCol + checks[0][3] * i };
					validSquares.insert(validSquare);
					if (validSquare[0] == checkRow && validSquare[1] == checkCol)
						break;
				}

			}
			for (int i = 0;i < ValidMoves1.size();i++)
			{
				if (ValidMoves1[i][0] * 8 + ValidMoves1[i][1] != kingRow * 8 + kingCol)
				{
					if (validSquares.find({ ValidMoves1[i][2] , ValidMoves1[i][3] }) != validSquares.end())
						ValidMoves.push_back(ValidMoves1[i]);
				}
				else
				{
					ValidMoves.push_back(ValidMoves1[i]);
				}
			}
		}
		else
		{
			generateKingMoves(ValidMoves, kingRow, kingCol);
		}
	}
	else
	{
		//std::cout << "here" << std::endl;
		ValidMoves = GenerateAllMoves();
	}
	return ValidMoves;
}

void Gamestate::generatePawnMoves(std::vector<std::vector<int>>& possible_moves, int r, int c)
{
	bool piecePinned = false;
	std::vector<int> pindirections;
	for (int i = pins.size() - 1;i >= 0;i--)
	{
		if (pins[i][0] == r && pins[i][1] == c)
		{
			piecePinned = true;
			pindirections = { pins[i][2], pins[i][3] };
			pins.erase(pins.begin() + i);
			break;
		}
	}
	int moveAmount, startRow, enemyColor,kingrow,kingcol;
	if (whiteToMove)
	{
		moveAmount = -1;
		startRow = 6;
		enemyColor = Black;
		kingrow = whiteKingLocation / 8;
		kingcol = whiteKingLocation % 8;
	}
	else
	{
		moveAmount = 1;
		startRow = 1;
		enemyColor = White;
		kingrow = blackKingLocation / 8;
		kingcol = blackKingLocation % 8;
	}
	Move* move = new Move();
	Position p, p2;

	if (board[r + moveAmount][c] == 0)
	{
		std::vector<int> temp = { moveAmount , 0 };
		if (!piecePinned || pindirections == temp)
		{
			p.instantiate(r,c), p2.instantiate(r + moveAmount , c);
			move->registerMove(p, p2, board);
			possible_moves.push_back({ r, c, r + moveAmount, c });
			if (r == startRow && board[r + 2 * moveAmount][c] == 0)
			{
				p2.instantiate(r + 2 * moveAmount , c);
				move->registerMove(p, p2, board);
				possible_moves.push_back({ r , c , r + 2 * moveAmount, c });
			}
		}
	}
	if (c - 1 >= 0)
	{
		std::vector<int> temp = { moveAmount, -1 };
		if (!piecePinned || pindirections == temp)
		{
			if ((board[r + moveAmount][c - 1]^enemyColor) <= 6)
			{
				p.instantiate(r,c);
				p2.instantiate(r + moveAmount, c - 1);
				move->registerMove(p, p2, board);
				possible_moves.push_back({ r, c , r + moveAmount , c - 1 });
			}
			int t = (r + moveAmount) * 8 +  c - 1 ;
			if (t == enpassant_target_square)
			{
				bool attackingPiece = false, blockingPiece = false;
				std::pair<int, int> insideRange, outsideRange;
				if (kingrow == r)
				{
					if (kingcol < c)
					{
						insideRange = { kingcol + 1 , c - 1 };
						outsideRange = { c + 1 , 8 };
					}
					else
					{
						insideRange = { c + 1,kingcol };
						outsideRange = { 0 , c - 1 };
					}
					for (int i = insideRange.first; i < insideRange.second;i++)
					{
						if (board[r][i] != 0)
						{
							blockingPiece = true;
						}
					}
					for (int i = outsideRange.first;i < outsideRange.second;i++)
					{
						int square = board[r][i];
						int x = square ^ enemyColor;
						if (x == 5 || x == 6)
						{
							attackingPiece = true;
						}
						else if (square != 0)
						{
							blockingPiece = true;
						}
					}
				}
				if (!attackingPiece || blockingPiece)
				{
					p2.instantiate(r +  moveAmount , c - 1);
					move->registerMove(p, p2, board);
					possible_moves.push_back({ r , c , r + moveAmount, c - 1});
				}
			}
		}
	}
	if (c + 1 < board.size())
	{
		std::vector<int> temp = { moveAmount, 1 };
		if (!piecePinned || pindirections == temp)
		{
			if ((board[r + moveAmount][c + 1]^enemyColor) <= 6)
			{
				p.instantiate(r,c);
				p2.instantiate(r + moveAmount, c + 1);
				move->registerMove(p, p2, board);
				possible_moves.push_back({ r, c , r + moveAmount , c + 1 });
			}
			int t = (r + moveAmount) * 8 + c  + 1;
			if (t == enpassant_target_square)
			{
				bool attackingPiece = false, blockingPiece = false;
				std::pair<int, int> insideRange, outsideRange;
				if (kingrow == r)
				{
					if (kingcol < c)
					{
						insideRange = { kingcol + 1 , c - 1 };
						outsideRange = { c + 2 , 8 };
					}
					else
					{
						insideRange = { c + 2,kingcol };
						outsideRange = { 0 , c };
					}
					for (int i = insideRange.first; i < insideRange.second;i++)
					{
						if (board[r][i] != 0)
						{
							blockingPiece = true;
						}
					}
					for (int i = outsideRange.first;i < outsideRange.second;i++)
					{
						int square = board[r][i];
						if ((square ^ enemyColor) == 5 || (square ^ enemyColor) == 6)
						{
							attackingPiece = true;
						}
						else if (square != 0)
						{
							blockingPiece = true;
						}
					}
				}
				if (!attackingPiece || blockingPiece)
				{
					p2.instantiate(r + moveAmount, c + 1);
					move->registerMove(p, p2, board);
					possible_moves.push_back({ r , c , r + moveAmount, c + 1 });
				}
			}
		}
	}
}

void Gamestate::generateSlidingMoves(std::vector<std::vector<int>>& possible_moves, int r, int c, int type)
{
	int allyColor, enemyColor;
	if (whiteToMove)
	{
		allyColor = 8;
		enemyColor = 16;
	}
	else
	{
		allyColor = 16;
		enemyColor = 8;
	}
	const int dir[8][2] = { {0,1},{0,-1},{1,0},{-1,0},{1,1},{1,-1},{-1,1},{-1,-1}};
	int startInd = 0 , endInd = 7;
	if (type == 4)
	{
		startInd = 4;
		endInd = 7;
	}
	else if (type == 5)
	{
		startInd = 0;
		endInd = 3;
	}

	bool piecePinned = false;
	std::vector<int> pindirections;
	for (int i = pins.size() - 1;i >= 0;i--)
	{
		if (pins[i][0] == r && pins[i][1] == c)
		{
			piecePinned = true;
			pindirections = { pins[i][2],pins[i][3]};
			if(type == 4)
				pins.erase(pins.begin() + i);
			else
				pins.erase(pins.begin() + i);
			break;
		}
	}

	for (int i = startInd;i <= endInd;i++)
	{
		for (int j = 1;j < 8;j++)
		{
			int endrow = r + dir[i][0] * j;
			int endcol = c + dir[i][1] * j;

			if (0 <= endrow && endrow < 8 && endcol >= 0 && endcol < 8)
			{
				std::vector<int> temo = { dir[i][0], dir[i][1] }, temp = { -dir[i][0], -dir[i][1] };

				if (!piecePinned || pindirections == temo || pindirections == temp)
				{
					Position p1, p2;
					p1.instantiate(r, c), p2.instantiate(endrow,endcol);
					int endpiece = board[endrow][endcol];
					if (endpiece == 0)
					{
						Move* move = new Move();
						move->registerMove(p1, p2, board);
						possible_moves.push_back({ r,c,endrow,endcol });
					}
					else if (abs(board[r][c] - endpiece) >= 3 && (endpiece^enemyColor) <= 6) 
					{
						Move* move = new Move();
						move->registerMove(p1, p2, board);
						possible_moves.push_back({ r,c,endrow,endcol });
						break;
					}
					else
						break;
				}
			}
		}
	}

}

void Gamestate::generateKnightMoves(std::vector<std::vector<int>>& possible_moves, int r, int c)
{
	bool piecePinned = false;
	std::vector<int> pindirections;
	for (int i = pins.size() - 1;i >= 0;i--)
	{
		if (pins[i][0] == r && pins[i][1] == c)
		{
			piecePinned = true;
			pindirections = { pins[i][2], pins[i][3] };
			pins.erase(pins.begin() + i);
			break;
		}
	}
	const int dir2[8][2] = {{1,2} , {1, -2}, {-1,2},{-1,-2},{2,1},{2,-1},{-2,1},{-2,-1}};
	for (auto p : dir2)
	{
		int endrow = r + p[0];
		int endcol = c + p[1];
		if (0 <= endrow && endrow < 8 && 0 <= endcol && endcol < 8)
		{
			if (!piecePinned)
			{
				Position p1, p2;
				p1.instantiate(r, c), p2.instantiate(endrow, endcol);
				int endpiece = board[endrow][endcol];
				if (endpiece == 0 || abs(board[r][c] - endpiece) > 5)
				{
					Move* move = new Move();
					move->registerMove(p1, p2, board);
					possible_moves.push_back({ r,c,endrow,endcol });
				}
				else
				{
					continue; // do nothing
				}
			}
		}
	}
}

void Gamestate::getCastleMoves(std::vector<std::vector<int>>& possible_moves, int r, int c)
{
	if (inCheck)
		return;
	if ((whiteToMove && castling_rights[0]) || (!whiteToMove && castling_rights[2]))
	{
		getKingsideCastleMoves(possible_moves, r, c);
	}
	if ((whiteToMove && castling_rights[1]) || (!whiteToMove && castling_rights[3]))
	{
		getQueensideCastleMoves(possible_moves, r, c);
	}
}

void Gamestate::getKingsideCastleMoves(std::vector<std::vector<int>>& possible_moves, int r, int c)
{
	if (board[r][c + 1] == 0  && board[r][c + 2] == 0 )
	{
		if (whiteToMove)
		{
			whiteKingLocation = r * 8 + c + 1;
			bool tcheck = inCheck;
			std::vector<std::vector<int>> tpin = pins, tchecks = checks;
			checkForPinsAndChecks();
			if (inCheck)
			{
				inCheck = tcheck;
				pins = tpin;
				checks = tchecks;
				return;
			}
			inCheck = tcheck;
			pins = tpin;
			checks = tchecks;
			whiteKingLocation = r * 8 + c + 2;
			tcheck = inCheck;
			tpin = pins, tchecks = checks;
			checkForPinsAndChecks();
			whiteKingLocation = 60;
			if (inCheck)
			{
				inCheck = tcheck;
				pins = tpin;
				checks = tchecks;
				return;
			}
			else
			{
				inCheck = tcheck;
				pins = tpin;
				checks = tchecks;
				Position p1, p2;
				p1.instantiate(r, c), p2.instantiate(r, c + 2);
				Move* move = new Move();
				move->registerMove(p1, p2, board);
				move->isCastleMove = true;
				possible_moves.push_back({ r,c,r,c + 2});
			}
		}
		else
		{
			blackKingLocation = r * 8 + c + 1;
			bool tcheck = inCheck;
			std::vector<std::vector<int>> tpin = pins, tchecks = checks;
			checkForPinsAndChecks();
			if (inCheck)
			{
				inCheck = tcheck;
				pins = tpin;
				checks = tchecks;
				return;
			}
			inCheck = tcheck;
			pins = tpin;
			checks = tchecks;
			blackKingLocation = r * 8 + c + 2;
			tcheck = inCheck;
			tpin = pins, tchecks = checks;
			checkForPinsAndChecks();
			blackKingLocation = 4;
			if (inCheck)
			{
				inCheck = tcheck;
				pins = tpin;
				checks = tchecks;
				return;
			}
			else
			{
				inCheck = tcheck;
				pins = tpin;
				checks = tchecks;
				Position p1, p2;
				p1.instantiate(r, c), p2.instantiate(r, c + 2);
				Move* move = new Move();
				move->registerMove(p1, p2, board);
				move->isCastleMove = true;
				possible_moves.push_back({r,c,r,c + 2});
			}
		}
	}
}

void Gamestate::getQueensideCastleMoves(std::vector<std::vector<int>>& possible_moves, int r, int c)
{
	if (board[r][c - 1] == 0 && board[r][c - 2] == 0 && board[r][c - 3] == 0)
	{
		if (whiteToMove)
		{
			whiteKingLocation = r * 8 + c - 1;
			bool tcheck = inCheck;
			std::vector<std::vector<int>> tpin = pins, tchecks = checks;
			checkForPinsAndChecks();
			if (inCheck)
			{
				inCheck = tcheck;
				pins = tpin;
				checks = tchecks;
				return;
			}
			inCheck = tcheck;
			pins = tpin;
			checks = tchecks;
			whiteKingLocation = r * 8 + c - 2;
			tcheck = inCheck;
			tpin = pins, tchecks = checks;
			checkForPinsAndChecks();
			whiteKingLocation = 60;
			if (inCheck)
			{
				inCheck = tcheck;
				pins = tpin;
				checks = tchecks;
				return;
			}
			else
			{
				inCheck = tcheck;
				pins = tpin;
				checks = tchecks;
				Position p1, p2;
				p1.instantiate(r, c), p2.instantiate(r, c - 2);
				Move* move = new Move();
				move->registerMove(p1, p2, board);
				move->isCastleMove = true;
				possible_moves.push_back({ r,c,r,c - 2 });
			}
		}
		else
		{
			blackKingLocation = r * 8 + c - 1;
			bool tcheck = inCheck;
			std::vector<std::vector<int>> tpin = pins, tchecks = checks;
			checkForPinsAndChecks();
			if (inCheck)
			{
				inCheck = tcheck;
				pins = tpin;
				checks = tchecks;
				return;
			}
			inCheck = tcheck;
			pins = tpin;
			checks = tchecks;
			blackKingLocation = r * 8 + c - 2;
			tcheck = inCheck;
			tpin = pins, tchecks = checks;
			checkForPinsAndChecks();
			blackKingLocation = 4;
			if (inCheck)
			{
				inCheck = tcheck;
				pins = tpin;
				checks = tchecks;
				return;
			}
			else
			{
				inCheck = tcheck;
				pins = tpin;
				checks = tchecks;
				Position p1, p2;
				p1.instantiate(r, c), p2.instantiate(r, c - 2);
				Move* move = new Move();
				move->registerMove(p1, p2, board);
				move->isCastleMove = true;
				possible_moves.push_back({ r,c,r,c - 2 });
			}
		}

	}

}

void Gamestate::updateCastlingRights(Move* move)
{
	if (move->pieceMoved == WhiteKing)
	{
		castling_rights[0] = false;
		castling_rights[1] = false;
	}
	else if (move->pieceMoved == BlackKing)
	{
		castling_rights[2] = false;
		castling_rights[3] = false;
	}
	else if (move->pieceMoved == WhiteRook)
	{
		if (move->startCol == 7)
			castling_rights[0] = false;
		else if(move->startCol == 0)
			castling_rights[1] = false;
	}
	else if (move->pieceMoved == BlackRook)
	{
		if (move->startCol == 7)
			castling_rights[2] = false;
		else if (move->startCol == 0)
			castling_rights[3] = false;
	}

	if (move->pieceCaptured == WhiteRook)
	{
		if (move->endCol == 7)
			castling_rights[0] = false;
		else if (move->endCol == 0)
			castling_rights[1] = false;
	}
	else if (move->pieceCaptured == BlackRook)
	{
		if (move->endCol == 7)
			castling_rights[2] = false;
		else if (move->endCol == 0)
			castling_rights[3] = false;
	}
}

void Gamestate::generateKingMoves(std::vector<std::vector<int>>& possible_moves,int r,int c)
{
	const int dir2[8][2] = {{1,0} , {1, 1}, {0,1},{-1,1},{-1,0},{-1,-1},{0,-1},{1,-1}};
	int allyColor = whiteToMove == true ? White : Black;
	for (auto p : dir2)
	{
		int endrow = r + p[0];
		int endcol = c + p[1];
		if (0 <= endrow && endrow < 8 && 0 <= endcol && endcol < 8)
		{
			Position p1, p2;
			p1.instantiate(r, c), p2.instantiate(endrow, endcol);
			int endpiece = board[endrow][endcol];
			if (endpiece == 0 || abs(board[r][c] - endpiece) > 5)
			{
				if (allyColor == 8)
				{
					whiteKingLocation = endrow * 8 + endcol;
				}
				else
				{
					blackKingLocation = endrow * 8 + endcol;
				}
				bool tcheck = inCheck;
				std::vector<std::vector<int>> tpin = pins, tchecks = checks;
				checkForPinsAndChecks();
				if (!inCheck)
				{
					Move* move = new Move();
					move->registerMove(p1, p2, board);
					possible_moves.push_back({ r,c,endrow,endcol });
				}
				inCheck = tcheck;
				pins = tpin;
				checks = tchecks;
				if (allyColor == 8)
				{
					whiteKingLocation = r * 8 + c;
				}
				else
				{
					blackKingLocation = r * 8 + c;
				}
			}
			else
			{
				continue; // do nothing
			}
		}
	}
	getCastleMoves(possible_moves, r, c);
}



