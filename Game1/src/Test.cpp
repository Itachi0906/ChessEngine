//#include "Move.h"
//#include "Gamestate.h"
//
//Gamestate* gs = NULL;
//Move* m = NULL;
//
//int main(int argc, char* argv[])
//{
//	gs = new Gamestate();
//	//gs->init("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8");
//	gs->init("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1");
//	std::vector<std::vector<int>> moves = gs->GenerateAllValidMoves();
//	std::cout << moves.size() << std::endl;
//	int cnt = 0;
//	for (auto move : moves)
//	{
//		Move* m1 = new Move();
//		std::pair<int, int> p = { move[0], move[1] }, p1 = { move[2], move[3] };
//		m1->register_move(p,p1,gs->board);
//		gs->makemove(m1);
//
//		gs->whiteToMove = !gs->whiteToMove;
//		std::vector<std::vector<int>> m2 = gs->GenerateAllValidMoves();
//		cnt += m2.size();
//		gs->whiteToMove = !gs->whiteToMove;
//		gs->undomove(gs->movelog, gs->board);
//		//std::cout << (char)('a' + move[1]) << 8 - move[0] << " " << (char)('a' + move[3]) << 8 - move[2] << std::endl;
//	}
//	std::cout << cnt << std::endl;
//
//}