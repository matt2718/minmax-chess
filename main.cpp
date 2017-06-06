#include <iostream>

#include "chessboard.hpp"

void printMoves(Chessboard *cb);

int main(int argc, char **argv) {
	Chessboard *cb = new Chessboard();

	char ch1, ch2, ch3, ch4;

	cb->printBoard(true);
	printMoves(cb);
	
	std::cout << ">>>> ";
	std::cin >> ch1;
	while (ch1 != 'q' && !std::cin.eof()) {
		std::cin >> ch2 >> ch3 >> ch4;
		std::cin.ignore(1<<30, '\n');
		Move m;
		m.colFrom = ch1 - 'a';
		m.rowFrom = ch2 - '1';
		m.colTo = ch3 - 'a';
		m.rowTo = ch4 - '1';

		if (cb->isLegalMove(true, m))
			std::cout << "White: yes " << cb->makeMove(m) << std::endl;
		else
			std::cout << "White: no" << std::endl;


		if (cb->isLegalMove(false, m))
			std::cout << "Black: yes " << cb->makeMove(m) << std::endl;
		else
			std::cout << "Black: no" << std::endl;

		cb->printBoard(true);
		printMoves(cb);
		
		std::cout << ">>>> ";
		std::cin >> ch1;
	}
	if (std::cin.eof()) std::cout << std::endl; // newline if ^D
	
	delete cb;
	return 0;
}

void printMoves(Chessboard *cb) {
	std::vector<Move> whiteMoves = cb->getLegalMoves(true);
	std::vector<Move> blackMoves = cb->getLegalMoves(false);

	std::cout << "WHITE   BLACK" << std::endl;
	std::cout << "-----   -----" << std::endl;
	for (int i = 0; i < whiteMoves.size() || i < blackMoves.size(); i++) {
		if (i < whiteMoves.size()) {
			Move m = whiteMoves[i];
			std::cout << (char)('a' + m.colFrom) << (1 + m.rowFrom);
			std::cout << (char)('a' + m.colTo)   << (1 + m.rowTo);
		}
		std::cout << "    ";
		if (i < blackMoves.size()) {
			Move m = blackMoves[i];
			std::cout << (char)('a' + m.colFrom) << (1 + m.rowFrom);
			std::cout << (char)('a' + m.colTo)   << (1 + m.rowTo);
		}
		
		std::cout << std::endl;
	}
}
