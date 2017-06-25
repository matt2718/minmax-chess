#include "minimax.hpp"

MinimaxTree::MinimaxTree() : board() {

}

MinimaxTree::MinimaxTree(Chessboard cb) {
	board = cb;
}

MinimaxTree::~MinimaxTree() {
	for (MinimaxTree *child : children) {
		delete child;
	}
}

void MinimaxTree::makeMove(Move move) {
	MinimaxTree *newTree = NULL;

	for (MinimaxTree *child : children) {
		Move thisMove = child->board.getLastMove();
		// if this is the move we're making, reuse the subtree
		if (thisMove == move)
			newTree = child;
		else
			delete child;
	}

	if (newTree != NULL) {
		board = newTree->board;
		children = newTree->children;
		newTree->children.clear(); // prevent recursive free in destructor
		delete newTree;
	} else {
		board.makeMove(move);
	}
}

Move MinimaxTree::getBestMove(bool isWhite, int depth) {
	makeChildren(isWhite);

	int maxAdv = -1000000;
	Move move;
	for (MinimaxTree *child : children) {
		int adv = child->minimax(isWhite, !isWhite, depth);
		if (adv > maxAdv) {
			maxAdv = adv;
			move = child->board.getLastMove();
		}
	}

	return move;
}

int MinimaxTree::minimax(bool isWhite, bool toMove, int steps) {
	// no more steps
	if (steps <= 0)
		return board.getAdvantage(isWhite);

	makeChildren(toMove);

	int result;
	
	// opponent to move, so minimize
	if (toMove ^ isWhite) {
		result = 1000000;
		for (MinimaxTree *child : children) {
			int adv = child->minimax(isWhite, !toMove, steps - 1);
			if (adv < result) result = adv;
		}
		
	// player to move, so maximize
	} else {
		result = -1000000;
		for (MinimaxTree *child : children) {
			int adv = child->minimax(isWhite, !toMove, steps - 1);
			if (adv > result) result = adv;
		}
	}

	return result;
}

void MinimaxTree::makeChildren(bool isWhite) {
	// only run if nothing has been added to vector
	if (children.empty()) {
		for (Move move : board.getLegalMoves(isWhite)) {
			MinimaxTree *tree = new MinimaxTree(board);
			tree->makeMove(move);
			children.push_back(tree);
		}
	}
}
