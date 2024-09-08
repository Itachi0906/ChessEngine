#include "AI.h"

std::unordered_map<int, std::vector<std::vector<double>>> piece_position_table = {
        {10, {
            {0, 0, 0, 0, 0, 0, 0, 0},
            {5, 5, 5, 5, 5, 5, 5, 5},
            {1, 1, 2, 3, 3, 2, 1, 1},
            {0.5, 0.5, 1, 2.5, 2.5, 1, 0.5, 0.5},
            {0, 0, 0, 2, 2, 0, 0, 0},
            {0.5, -0.5, -1, 0, 0, -1, -0.5, 0.5},
            {0.5, 1, 1, -2, -2, 1, 1, 0.5},
            {0, 0, 0, 0, 0, 0, 0, 0}
        }},
        {11, {
            {-5, -4, -3, -3, -3, -3, -4, -5},
            {-4, -2, 0, 0, 0, 0, -2, -4},
            {-3, 0, 1, 1.5, 1.5, 1, 0, -3},
            {-3, 0.5, 1, 2, 2, 1, 0.5, -3},
            {-3, 0, 1.5, 2.5, 2.5, 1.5, 0, -3},
            {-3, 0.5, 1, 2, 2, 1, 0.5, -3},
            {-4, -2, 0, 0.5, 0.5, 0, -2, -4},
            {-5, -4, -3, -3, -3, -3, -4, -5}
        }},
        {12, {
            {-2, -1, -1, -1, -1, -1, -1, -2},
            {-1, 0, 0, 0, 0, 0, 0, -1},
            {-1, 0, 0.5, 1, 1, 0.5, 0, -1},
            {-1, 0.5, 0.5, 1, 1, 0.5, 0.5, -1},
            {-1, 0, 1, 1, 1, 1, 0, -1},
            {-1, 1, 1, 1, 1, 1, 1, -1},
            {-1, 0.5, 0, 0, 0, 0, 0.5, -1},
            {-2, -1, -1, -1, -1, -1, -1, -2}
        }},
        {13, {
            {0, 0, 0, 0, 0, 0, 0, 0},
            {0.5, 1, 1, 1, 1, 1, 1, 0.5},
            {-0.5, 0, 0, 0, 0, 0, 0, -0.5},
            {-0.5, 0, 0, 0, 0, 0, 0, -0.5},
            {-0.5, 0, 0, 0, 0, 0, 0, -0.5},
            {-0.5, 0, 0, 0, 0, 0, 0, -0.5},
            {-0.5, 0, 0, 0, 0, 0, 0, -0.5},
            {0, 0, 0, 0, 0, 0, 0, 0}
        }},
        {14, {
            {-2, -1, -1, -0.5, -0.5, -1, -1, -2},
            {-1, 0, 0, 0, 0, 0, 0, -1},
            {-1, 0, 0.5, 0.5, 0.5, 0.5, 0, -1},
            {-0.5, 0, 0.5, 0.5, 0.5, 0.5, 0, -0.5},
            {0, 0, 0.5, 0.5, 0.5, 0.5, 0, -0.5},
            {-1, 0.5, 0.5, 0.5, 0.5, 0.5, 0.5, -1},
            {-1, 0, 0.5, 0, 0, 0, 0, -1},
            {-2, -1, -1, -0.5, -0.5, -1, -1, -2}
        }},
        {9, {
            {-3, -4, -4, -5, -5, -4, -4, -3},
            {-3, -4, -4, -5, -5, -4, -4, -3},
            {-3, -4, -4, -5, -5, -4, -4, -3},
            {-3, -4, -4, -5, -5, -4, -4, -3},
            {-2, -3, -3, -4, -4, -3, -3, -2},
            {-1, -2, -2, -2, -2, -2, -2, -1},
            {2, 2, 0, 0, 0, 0, 2, 2},
            {2, 3, 1, 0, 0, 1, 3, 2}
        }},
        {18, {
            {0, 0, 0, 0, 0, 0, 0, 0},
            {-5, -5, -5, -5, -5, -5, -5, -5},
            {-1, -1, -2, -3, -3, -2, -1, -1},
            {-0.5, -0.5, -1, -2.5, -2.5, -1, -0.5, -0.5},
            {0, 0, 0, -2, -2, 0, 0, 0},
            {-0.5, 0.5, 1, 0, 0, 1, 0.5, -0.5},
            {-0.5, -1, -1, 2, 2, -1, -1, -0.5},
            {0, 0, 0, 0, 0, 0, 0, 0}
        }},
        {19, {
            {5, 4, 3, 3, 3, 3, 4, 5},
            {4, 2, 0, 0, 0, 0, 2, 4},
            {3, 0, -1, -1.5, -1.5, -1, 0, 3},
            {3, -0.5, -1, -2, -2, -1, -0.5, 3},
            {3, 0, -1.5, -2.5, -2.5, -1.5, 0, 3},
            {3, -0.5, -1, -2, -2, -1, -0.5, 3},
            {4, 2, 0, -0.5, -0.5, 0, 2, 4},
            {5, 4, 3, 3, 3, 3, 4, 5}
        }},
        {20, {
            {2, 1, 1, 1, 1, 1, 1, 2},
            {1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, -0.5, -1, -1, -0.5, 0, 1},
            {1, -0.5, -0.5, -1, -1, -0.5, -0.5, 1},
            {1, 0, -1, -1, -1, -1, 0, 1},
            {1, -1, -1, -1, -1, -1, -1, 1},
            {1, -0.5, 0, 0, 0, 0, -0.5, 1},
            {2, 1, 1, 1, 1, 1, 1, 2}
        }},
        {21, {
            {0, 0, 0, 0, 0, 0, 0, 0},
            {-0.5, -1, -1, -1, -1, -1, -1, -0.5},
            {0.5, 0, 0, 0, 0, 0, 0, 0.5},
            {0.5, 0, 0, 0, 0, 0, 0, 0.5},
            {0.5, 0, 0, 0, 0, 0, 0, 0.5},
            {0.5, 0, 0, 0, 0, 0, 0, 0.5},
            {0.5, 1, 1, 1, 1, 1, 1, 0.5},
            {0, 0, 0, 0, 0, 0, 0, 0}
        }},
        {22, {
            {2, 1, 1, 0.5, 0.5, 1, 1, 2},
            {1, 0, 0, -0.5, -0.5, 0, 0, 1},
            {1, 0, -0.5, -0.5, -0.5, -0.5, 0, 1},
            {0.5, 0, -0.5, -0.5, -0.5, -0.5, 0, 0.5},
            {0, 0, -0.5, -0.5, -0.5, -0.5, 0, 0},
            {1, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 1},
            {1, 0, -0.5, 0, 0, 0, 0, 1},
            {2, 1, 1, 0.5, 0.5, 1, 1, 2}
        }},
        {17, {
            {3, 4, 4, 5, 5, 4, 4, 3},
            {3, 4, 4, 5, 5, 4, 4, 3},
            {3, 4, 4, 5, 5, 4, 4, 3},
            {3, 4, 4, 5, 5, 4, 4, 3},
            {2, 3, 3, 4, 4, 3, 3, 2},
            {1, 2, 2, 2, 2, 2, 2, 1},
            {-2, -2, 0, 0, 0, 0, -2, -2},
            {-2, -3, -1, 0, 0, -1, -3, -2}
        }}
    };


Move* AI::findRandomMove(Gamestate* gs, std::vector<std::vector<int>>& validMoves)
{
	nextMove = new Move();
	auto vec = validMoves[rand() % validMoves.size()];
	std::pair<int,int> p = { vec[0],vec[1] }, p2 = { vec[2],vec[3] };
	nextMove->register_move(p, p2, gs->board);
	return nextMove;
}

Move* AI::FindBestMove(Gamestate* gs, std::vector<std::vector<int>>& validMoves)
{
	nextMove = new Move();
	int turnM = gs->whiteToMove == true ? 1 : -1;
	MinMaxAlphaBeta(gs, validMoves, Depth, -checkmate, checkmate, turnM);
    //std::cout << nextMove->start_row << " " << nextMove->start_col << " " << nextMove->end_row << " " << nextMove->end_col << std::endl;
	return nextMove;
}

double evaluate(std::vector<std::vector<int>>& board)
{
	std::unordered_map<int, double> piece_scores = { {1,0},{2,100},{3,300},{4,330},{5,500},{6,900} };
	double score = 0;
	for (int i = 0;i < 8;i++)
	{
		for (int j = 0;j < 8;j++)
		{
			if (board[i][j] > 16)
			{
                score -= piece_scores[board[i][j] - 16];//piece_position_table[board[i][j] - 16][i][j];
                /*if (piece_position_table.find(board[i][j] - 16) != piece_position_table.end())
                {
                    score -= piece_position_table[board[i][j] - 16][i][j];
                }*/
			}
			else
			{
                score += piece_scores[board[i][j] - 8];//piece_position_table[board[i][j] - 8][i][j];
                /*if (piece_position_table.find(board[i][j] - 8) != piece_position_table.end())
                {
                    score += piece_position_table[board[i][j] - 8][i][j];
                }*/
			}
		}
	}
	return score;
}

double AI::MinMaxAlphaBeta(Gamestate* gs, std::vector<std::vector<int>>& validMoves, int depth, double alpha, double beta, int turnMultiplier)
{
	if (depth == 0)
	{
		return turnMultiplier*evaluate(gs->board);
	}

	double maxScore = -checkmate;
	if (validMoves.size() == 0)
	{
		if (gs->inCheck)
			return -checkmate;
		else
			return 0;
	}

	for (auto move : validMoves)
	{
		Move* m = new Move();
		std::pair<int, int> p = { move[0],move[1] }, p1 = { move[2],move[3] };
		m->register_move(p, p1,gs->board);
        std::cout << m->start_row << " " << m->start_col << " " << m->end_row << " " << m->end_col << std::endl;
		gs->makemove(m);
		gs->whiteToMove = !gs->whiteToMove;
		std::vector<std::vector<int>> nextMoves = gs->GenerateAllValidMoves();
        //std::cout << nextMoves.size() << std::endl;
		double score = -MinMaxAlphaBeta(gs, nextMoves, depth - 1, -beta, -alpha, -turnMultiplier);
		if (score > maxScore)
		{
			maxScore = score;
			if (depth == Depth)
			{
				nextMove = m;
			}
		}
		gs->whiteToMove = !gs->whiteToMove;
		gs->undomove(gs->movelog, gs->board);
		if (maxScore > alpha)
		{
			alpha = maxScore;
		}
		if (alpha >= beta)
		{
			break;
		}
	}
	return alpha;

}
