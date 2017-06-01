#include <iostream>

#include "chessboard.hpp"

int main(int argc, char **argv) {
	Chessboard *cb = new Chessboard();

	char ch1, ch2, ch3, ch4;

	cb->printBoard(true);
	std::cout << ">>>> ";
	std::cin >> ch1;
	while (ch1 != 'q') {
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
		std::cout << ">>>> ";
		std::cin >> ch1;
	}
	
	delete cb;
	return 0;
}
