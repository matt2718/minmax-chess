#include <vector>
#include "chessboard.hpp"

class MinimaxTree {
public:
	MinimaxTree();
	MinimaxTree(Chessboard board);
	~MinimaxTree();

	// move 
	void makeMove(Move move);

	// estimates the best move for a given player
	Move getBestMove(bool isWhite);
		
private:
	void makeChildren(bool isWhite);

	int minimax(bool isWhite, bool toMove, int steps);
	
	Chessboard cb;
	std::vector<MinimaxTree*> children;
};
