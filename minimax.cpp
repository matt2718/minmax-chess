#include "minimax.hpp"

MinimaxTree::MinimaxTree() {

}

MinimaxTree::MinimaxTree(Chessboard board) {
	cb = board;
}

MinimaxTree::~MinimaxTree() {
	for (MinimaxTree *child : children)
		delete child;
}

void MinimaxTree::makeMove(Move move) {
	MinimaxTree *newTree = NULL;

	for (MinimaxTree *child : children) {
		Move thisMove = child->cb.getLastMove();
		// if this is the move we're making, reuse the tree
		if (thisMove.rowFrom == move.rowFrom &&
		    thisMove.colFrom == move.colFrom &&
		    thisMove.rowTo == move.rowTo &&
		    thisMove.colTo == move.colTo)
			newTree = child;
		else
			delete child;
	}

	if (newTree != NULL) {
		cb = newTree->cb;
		children = newTree->children;
		delete newTree;
	} else {
		cb.makeMove(move);
	}
}

Move MinimaxTree::getBestMove(bool isWhite) {
	makeChildren(isWhite);

	int maxAdv = -1000000;
	Move move;
	for (MinimaxTree *child : children) {
		int adv = child->minimax(isWhite, !isWhite, 4);
		if (adv > maxAdv) {
			maxAdv = adv;
			move = child->cb.getLastMove();
		}
	}

	return move;
}

int MinimaxTree::minimax(bool isWhite, bool toMove, int steps) {
	// no more steps
	if (steps <= 0)
		return cb.getAdvantage(isWhite);

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
		for (Move move : cb.getLegalMoves(isWhite)) {
			MinimaxTree *tree = new MinimaxTree(cb);
			tree->makeMove(move);
			children.push_back(tree);
		}
	}
}
