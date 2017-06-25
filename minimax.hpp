#ifndef _MINIMAX_HPP
#define _MINIMAX_HPP

#include <vector>
#include "chessboard.hpp"

class MinimaxTree {
public:
	MinimaxTree();
	MinimaxTree(Chessboard cb);
	~MinimaxTree();

	// move 
	void makeMove(Move move);

	// estimates the best move for a given player
	Move getBestMove(bool isWhite, int depth);

	Chessboard board;
	
private:
	void makeChildren(bool isWhite);

	int minimax(bool isWhite, bool toMove, int steps);
	
	std::vector<MinimaxTree*> children;
};

#endif
