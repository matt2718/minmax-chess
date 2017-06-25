#include <iostream>
#include "chessboard.hpp"
#include "minimax.hpp"

void printMoves(Chessboard *cb);
void minimaxTest();

int main(int argc, char **argv) {
	//minimaxTest();

	MinimaxTree tree;

	char ch1, ch2, ch3, ch4;
	
	tree.board.printBoard(true);
	
	std::cout << ">>>> ";
	std::cin >> ch1;
	while (ch1 != 'q' && !std::cin.eof()) {
		std::cin >> ch2 >> ch3 >> ch4;
		std::cin.ignore(1<<30, '\n');  // flush stdin
		Move whiteMove;
		whiteMove.colFrom = ch1 - 'a';
		whiteMove.rowFrom = ch2 - '1';
		whiteMove.colTo = ch3 - 'a';
		whiteMove.rowTo = ch4 - '1';

		if (tree.board.isLegalMove(true, whiteMove)) {
			tree.makeMove(whiteMove);
			tree.board.printBoard(true);
			Move blackMove = tree.getBestMove(false, 4);
			tree.makeMove(blackMove);
			tree.board.printBoard(true);
		} else {
			std::cout << "Not a legal move" << std::endl;
		}

		//printMoves(tree.board);
		//std::cout << "White adv: " << tree.board.getAdvantage(true) << std::endl;

		std::cout << ">>>> ";
		std::cin >> ch1;
	}
	if (std::cin.eof()) std::cout << std::endl; // newline if ^D
	
	return 0;
}

void minimaxTest() {
	Chessboard board;
	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			board.removePiece(row, col);
		}
	}

	// B . .
	// . R .
	// . . .
	// n . .
	board.addPiece('n', true, 0, 0);
	board.addPiece('r', false, 2, 1);
	board.addPiece('b', false, 3, 2);

	MinimaxTree *tree = new MinimaxTree(board);
	Move move1 = tree->getBestMove(true, 1);
	Move move2 = tree->getBestMove(true, 2);

	std::cout << "(" << move1.rowFrom << "," << move1.colFrom << ") to "
	          << "(" << move1.rowTo << "," << move1.colTo << ")" << std::endl;
	std::cout << "(" << move2.rowFrom << "," << move2.colFrom << ") to "
	          << "(" << move2.rowTo << "," << move2.colTo << ")" << std::endl;
	
	delete tree;
}

void printMoves(Chessboard *board) {
	std::vector<Move> whiteMoves = board->getLegalMoves(true);
	std::vector<Move> blackMoves = board->getLegalMoves(false);

	std::cout << "WHITE | BLACK" << std::endl;
	std::cout << "------+------" << std::endl;
	for (int i = 0; i < whiteMoves.size() || i < blackMoves.size(); i++) {
		if (i < whiteMoves.size()) {
			Move m = whiteMoves[i];
			std::cout << (char)('a' + m.colFrom) << (1 + m.rowFrom);
			std::cout << (char)('a' + m.colTo)   << (1 + m.rowTo);
		} else {
			std::cout << "    ";
		}
		
		std::cout << "  | ";
		if (i < blackMoves.size()) {
			Move m = blackMoves[i];
			std::cout << (char)('a' + m.colFrom) << (1 + m.rowFrom);
			std::cout << (char)('a' + m.colTo)   << (1 + m.rowTo);
		}
		
		std::cout << std::endl;
	}
}
