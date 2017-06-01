#include <iostream>

#include "chessboard.hpp"

Chessboard::Chessboard() {
	for (int i = 0; i < 8; i++) {
		white[0][i] = true;
		white[1][i] = true;
		white[6][i] = false;
		white[7][i] = false;
	}
}

Chessboard::~Chessboard() {

}

char Chessboard::makeMove(Move move) {
	// ease of reading
	int rowFrom = move.rowFrom;
	int colFrom = move.colFrom;
	int rowTo = move.rowTo;
	int colTo = move.colTo;
	
	// piece info
	char piece = board[rowFrom][colFrom];
	bool isWhite = white[rowFrom][colFrom];
	char captured = board[rowTo][colTo];
	
	// actually making the move
	board[rowTo][colTo] = piece;
	white[rowTo][colTo] = isWhite;
	board[rowFrom][colFrom] = ' ';

	// move rook if castling
	if (piece == 'k' && colFrom == 4) {
		if (colTo == 2) { // queenside
			if (isWhite && rowFrom == 0 && !whiteRook0Moved) {
				whiteRook0Moved = true;
				board[0][0] = ' ';
				board[0][3] = 'r';
				white[0][3] = true;
			} else if (!isWhite && rowFrom == 7 && !blackRook0Moved) {
				blackRook0Moved = true;
				board[7][0] = ' ';
				board[7][3] = 'r';
				white[7][3] = false;
			}
		} else if (colTo == 6) { // kingside
			if (isWhite && rowFrom == 0 && !whiteRook7Moved) {
				whiteRook7Moved = true;
				board[0][7] = ' ';
				board[0][5] = 'r';
				white[0][5] = true;
			} else if (!isWhite && rowFrom == 7 && !blackRook7Moved) {
				blackRook7Moved = true;
				board[7][7] = ' ';
				board[7][5] = 'r';
				white[7][5] = false;
			}
		}
	}

	// remove pawn if en passant
	if (piece == 'p' && captured == ' ' && colFrom != colTo) {
		if (isWhite) board[rowTo - 1][colTo] = ' ';
		else board[rowTo + 1][colTo] = ' ';
		captured = 'p';
	}

	// Everything below is updating metadata for castling / en passant
	
	// clear any previous en passant flags
	if (isWhite) lastWhitePawn = -1;
	else lastBlackPawn = -1;
	
	// cannot castle after king moves
	if (piece == 'k') {
		if (isWhite) whiteKingMoved = true;
		else blackKingMoved = true;
	}

	// cannot castle after rook moves
	if (piece == 'r') {
		if (isWhite && rowFrom == 0) {
			if (colFrom == 0) whiteRook0Moved = true;
			if (colFrom == 7) whiteRook7Moved = true;
		} else if (!isWhite && rowFrom == 7) {
			if (colFrom == 0) blackRook0Moved = true;
			if (colFrom == 7) blackRook7Moved = true;
		}
	}

	// if a pawn moves 2 squares ahead, en passant is possible
	if (piece == 'p') {
		if (rowFrom == 1 && rowTo == 3) lastWhitePawn = colFrom;
		if (rowFrom == 6 && rowTo == 4) lastBlackPawn = colFrom;
	}
	
	return captured;
}

bool Chessboard::isLegalMove(bool isWhite, Move move) {
	// TODO: make sure king is not put in check
	
	// ease of reading
	int rowFrom = move.rowFrom;
	int colFrom = move.colFrom;
	int rowTo = move.rowTo;
	int colTo = move.colTo;

	int dRow = rowTo - rowFrom;
	int dCol = colTo - colFrom;
	
	// must acutally make move
	if (dRow == 0 && dCol == 0) return false;
	
	// ensure locations are on board
	if (rowFrom < 0 || 7 < rowFrom) return false;
	if (colFrom < 0 || 7 < colFrom) return false;
	if (rowTo < 0 || 7 < rowTo) return false;
	if (colTo < 0 || 7 < colTo) return false;
	
	// must move existing piece of correct color
	if (board[rowFrom][colFrom] == ' ' ||
	    white[rowFrom][colFrom] ^ isWhite) return false;

	// can't attack our own piece
	if (board[rowTo][colTo] != ' ' &&
	    white[rowTo][colTo] == isWhite) return false;
	
	// piece-specific stuff
	switch (board[rowFrom][colFrom]) {
	case 'p': // pawn
		if (isWhite) {
			if (dRow != 1) {
				// moving multiple spaces ahead
				// must be same column, first move, nothing in way
				return (dCol == 0 && rowFrom == 1 && rowTo == 3 &&
				        board[2][colTo] == ' ' && board[3][colTo] == ' ');
			}
			
			if (dCol == 0) {
				// not capturing
				return board[rowTo][colTo] == ' ';
			} else if (dCol == 1 || dCol == -1) {
				// en passant
				if (rowFrom == 4 && lastBlackPawn == colTo) return true;
				// capturing, must be opposite color
				return (board[rowTo][colTo] != ' ' &&
				        !white[rowTo][colTo]);
			} else {
				return false;
			}
		} else {
			if (dRow != - 1) {
				// moving multiple spaces ahead
				// must be same column, first move, nothing in way
				return (dCol == 0 && rowFrom == 6 && rowTo == 4 &&
				        board[5][colTo] == ' ' && board[4][colTo] == ' ');
			}
			
			if (dCol == 0) {
				// not capturing
				return board[rowTo][colTo] == ' ';
			} else if (dCol == 1 || dCol == -1) {
				// en passant
				if (rowFrom == 3 && lastWhitePawn == colTo) return true;
				// capturing, must be opposite color
				return (board[rowTo][colTo] != ' ' &&
				        white[rowTo][colTo]);
			} else {
				return false;
			}			
		}

	case 'r': // rook		
		// must move along rank or file, nothing in way
		return ((dRow == 0 || dCol == 0) && checkLine(move));
		
	case 'n': // knight
		// 2 and 1 or 1 and 2
		return (((dRow == 1 || dRow == -1) && (dCol == 2 || dCol == -2)) ||
		        ((dRow == 2 || dRow == -2) && (dCol == 1 || dCol == -1)));
		
	case 'b': // bishop
		// must move along diagonal, nothing in way
		return ((dRow == dCol || dRow == -dCol) && checkLine(move));
		
	case 'q': // queen
		// must move along rank, file, or diagonal, nothing in way
		return ((dRow == 0 || dCol == 0 || dRow == dCol || dRow == -dCol) &&
		        checkLine(move));

	case 'k': // king
		// castling
		if (dRow == 0 && dCol == -2) { // queenside
			if (isWhite && rowFrom == 0 && !whiteKingMoved) {
				if (whiteRook0Moved) return false;
				// nothing in way
				return (board[0][1] == ' ' &&
				        board[0][2] == ' ' &&
				        board[0][3] == ' ');
			} else if (!isWhite && rowFrom == 7 && !blackKingMoved) {
				if (blackRook0Moved) return false;
				// nothing in way
				return (board[7][1] == ' ' &&
				        board[7][2] == ' ' &&
				        board[7][3] == ' ');
			}
		} else if (dRow == 0 && dCol == 2) { // kingside
			if (isWhite && rowFrom == 0 && !whiteKingMoved) {
				if (whiteRook7Moved) return false;
				// nothing in way
				return (board[0][5] == ' ' &&
				        board[0][6] == ' ');
			} else if (!isWhite && rowFrom == 7 && !blackKingMoved) {
				if (blackRook7Moved) return false;
				// nothing in way
				return (board[7][5] == ' ' &&
				        board[7][6] == ' ');
			}
		}
		
		return (-1 <= dRow && dRow <= 1) && (-1 <= dCol && dCol <= 1);
		
	default:  // empty (or something completely different)
		return false;
	}
}

std::vector<Move> Chessboard::getLegalMoves(int row, int col) {
	std::vector<Move> moves;

	bool isWhite = white[row][col];
	char piece = board[row][col];

	// piece-specific stuff
	switch (board[row][col]) {
	case 'p': // pawn
		if (isWhite && col < 7) {
			//forward
			if (board[row + 1][col] == ' ')
				moves.push_back({row, col, row + 1, col});

			if (col > 0) {
				// capture left
				if (board[row + 1][col - 1] != ' ' &&
				    white[row + 1][col - 1])
					moves.push_back({row, col, row + 1, col - 1});
				// en passant
				if (board[row + 1][col - 1] == ' ' &&
				    row == 4 && lastBlackPawn == col - 1)
					moves.push_back({row, col, row + 1, col - 1});
			}
			
			if (col < 7) {
				// capture right
				if (board[row + 1][col + 1] != ' ' &&
				    white[row + 1][col + 1])
					moves.push_back({row, col, row + 1, col + 1});
				// en passant
				if (board[row + 1][col + 1] == ' ' &&
				    row == 4 && lastBlackPawn == col + 1)
					moves.push_back({row, col, row + 1, col + 1});
			}
		} else if (!isWhite && row > 0) {
			//forward
			if (board[row - 1][col] == ' ')
				moves.push_back({row, col, row - 1, col});

			if (col > 0) {
				// capture left
				if (board[row - 1][col - 1] != ' ' &&
				    white[row - 1][col - 1])
					moves.push_back({row, col, row - 1, col - 1});
				// en passant
				if (board[row - 1][col - 1] == ' ' &&
				    row == 3 && lastWhitePawn == col - 1)
					moves.push_back({row, col, row - 1, col - 1});
			}
			
			if (col < 7) {
				// capture right
				if (board[row - 1][col + 1] != ' ' &&
				    white[row - 1][col + 1])
					moves.push_back({row, col, row - 1, col + 1});
				// en passant
				if (board[row - 1][col + 1] == ' ' &&
				    row == 3 && lastWhitePawn == col + 1)
					moves.push_back({row, col, row - 1, col + 1});
			}
		}

		break;
	case 'r': // rook
		// front
		for (int r = row + 1; checkDest(isWhite, r, col); r++)
			moves.push_back({row, col, r, col});

		// back
		for (int r = row - 1; checkDest(isWhite, r, col); r--)
			moves.push_back({row, col, r, col});

		// right
		for (int c = col + 1; checkDest(isWhite, row, c); c++)
			moves.push_back({row, col, row, c});

		// left
		for (int c = col - 1; checkDest(isWhite, row, c); c--)
			moves.push_back({row, col, row, c});

		break;
	case 'n': // knight
		if (checkDest(isWhite, row + 2, col - 1))
			moves.push_back({row, col, row + 2, col - 1});

		if (checkDest(isWhite, row + 2, col + 1))
			moves.push_back({row, col, row + 2, col + 1});

		if (checkDest(isWhite, row + 1, col - 2))
			moves.push_back({row, col, row + 1, col - 2});

		if (checkDest(isWhite, row + 1, col + 2))
			moves.push_back({row, col, row + 1, col + 2});

		if (checkDest(isWhite, row - 1, col - 2))
			moves.push_back({row, col, row - 1, col - 2});

		if (checkDest(isWhite, row - 1, col + 2))
			moves.push_back({row, col, row - 1, col + 2});

		if (checkDest(isWhite, row - 2, col - 1))
			moves.push_back({row, col, row - 2, col - 1});

		if (checkDest(isWhite, row - 2, col + 1))
			moves.push_back({row, col, row - 2, col + 1});
		
		break;
	case 'b': // bishop
		// front right
		for (int r = row + 1, c = col + 1; checkDest(isWhite, r, c); r++, c++)
			moves.push_back({row, col, r, c});

		// front left
		for (int r = row + 1, c = col - 1; checkDest(isWhite, r, c); r++, c--)
			moves.push_back({row, col, r, c});

		// back right
		for (int r = row - 1, c = col + 1; checkDest(isWhite, r, c); r--, c++)
			moves.push_back({row, col, r, c});
		
		// back left
		for (int r = row - 1, c = col - 1; checkDest(isWhite, r, c); r--, c--)
			moves.push_back({row, col, r, c});
		
		break;
	case 'q': // queen
		for (int rdir = -1; rdir <= 1; rdir++) {
			for (int cdir = -1; cdir <= 1; cdir++) {
				for (int r = row + rdir, c = col + cdir;
				     checkDest(isWhite, r, c); r+= rdir, c+= cdir)
					moves.push_back({row, col, r, c});
			}
		}

	case 'k': // king
		for (int rdir = -1; rdir <= 1; rdir++) {
			for (int cdir = -1; cdir <= 1; cdir++) {
				if (checkDest(isWhite, row + rdir, col + cdir))
					moves.push_back({row, col, row + rdir, col + cdir});
			}
		}

		// castling
		if (isWhite && !whiteKingMoved) {
			// queenside
			if (!whiteRook0Moved &&
			    board[0][1] == ' ' &&
			    board[0][2] == ' ' &&
			    board[0][3] == ' ')
				moves.push_back({0, 4, 0, 2});
			// kingside
			if (!whiteRook7Moved &&
			    board[0][5] == ' ' &&
			    board[0][6] == ' ')
				moves.push_back({0, 4, 0, 6});
		} else if (isBlack && !blackKingMoved) {
			// queenside
			if (!blackRook0Moved &&
			    board[7][1] == ' ' &&
			    board[7][2] == ' ' &&
			    board[7][3] == ' ')
				moves.push_back({7, 4, 7, 2});
			// kingside
			if (!blackRook7Moved &&
			    board[7][5] == ' ' &&
			    board[7][6] == ' ')
				moves.push_back({7, 4, 7, 6});
		}

		break;
	default:  // empty (or something completely different)

	}

	return moves;
}

std::vector<Move> Chessboard::getLegalMoves(bool isWhite) {
	std::vector<Move> moves;

	for (int row = 0; row < 8; row++) {
		for (int col = 0; col < 8; col++) {
			if (board[row][col] != ' ' && white[row][col] == isWhite) {
				// append moves for this square to vector
				std::vector<Moves> tmp = getLegalMoves(row, col);
				moves.insert(moves.end(), tmp.begin(), tmp.end());
			}
		}
	}
	
	return moves;
}
	
char Chessboard::getPiece(int row, int col) {
	return board[row][col];
}

bool Chessboard::getColor(int row, int col) {
	return white[row][col];
}

char Chessboard::removePiece(int row, int col) {
	char tmp = board[row][col];
	board[row][col] = ' ';
	return tmp;
}

bool Chessboard::addPiece(char piece, bool isWhite, int row, int col) {
	if (board[row][col] != ' ') return false;
	board[row][col] = piece;
	white[row][col] = isWhite;
	return true;
}

void Chessboard::printBoard(bool isWhite) {	
	std::string reset = "\033[0m";
	std::string border1 = "\033[30;46m"; // black text, cyan
	std::string border2 = "\033[37;44m"; // white text, blue
	std::string square1 = "\033[43m"; // yellow
	std::string square2 = "\033[41m"; // red
	std::string pieceWhite = "\033[30;47m"; // black text, white
	std::string pieceBlack = "\033[37;40m"; // white text, black
	int width = 5;
	int height = 3;
	
	for (int r = 0; r < height; r++) {
		// left top
		std::cout << border1;
		for (int i = 0; i < width; i++)
			std::cout << " ";

		// top border
		for (int c = 0; c < 8 * width; c++) {
			int col = c / width;
			std::cout << (col % 2 ? border1 : border2);
			if (r % height == height / 2 && c % width == width / 2) {
				std::cout << (char)(isWhite ? 'a' + col : 'h' - col);
			} else {
				std::cout << ' ';
			}
		}

		// right top 
		std::cout << border2;
		for (int i = 0; i < width; i++)
			std::cout << " ";
		std::cout << reset << std::endl;
	}
	
	for (int r = 0; r < 8 * height; r++) {
		int row = 7 - r / height;
		if (!isWhite) row = 7 - row;
		// left border
		std::cout << (row % 2 ^ isWhite ? border1 : border2);
		for (int i = 0; i < width; i++) {
			if (r % height == height / 2 && i == width / 2) {
				std::cout << row + 1;
			} else {
				std::cout << " ";
			}
		}
			
		for (int c = 0; c < 8 * width; c++) {
			int col = c / width;
			if (!isWhite) col = 7 - col;
			// background color
			std::cout << ((row + col) % 2 ? square1 : square2);

			char piece = board[row][col];
			if (r % height == height / 2 && c % width == width / 2 &&
			    piece != ' ') {
				// convert non-pawns to uppercase
				if (piece != 'p' && piece != ' ') piece += ('A' - 'a');
				
				std::cout << (white[row][col] ? pieceWhite : pieceBlack);
				std::cout << (char)(piece);
			} else {
				std::cout << ' ';
			}
		}

		// right border
		std::cout << (row % 2 ^ isWhite ? border2 : border1);
		for (int i = 0; i < width; i++) {
			if (r % height == height / 2 && i == width / 2) {
				std::cout << row + 1;
			} else {
				std::cout << " ";
			}
		}
		std::cout << reset << std::endl;
	}
	
	for (int r = 0; r < height; r++) {
		// left bottom
		std::cout << border2;
		for (int i = 0; i < width; i++)
			std::cout << " ";

		// bottom border
		for (int c = 0; c < 8 * width; c++) {
			int col = c / width;
			std::cout << (col % 2 ? border2 : border1);
			if (r % height == height / 2 && c % width == width / 2) {
				std::cout << (char)(isWhite ? 'a' + col : 'h' - col);
			} else {
				std::cout << ' ';
			}
		}

		// right bottom 
		std::cout << border1;
		for (int i = 0; i < width; i++)
			std::cout << " ";
		std::cout << reset << std::endl;
	}
}

bool Chessboard::checkLine(Move move) {
	// ease of reading
	int rowFrom = move.rowFrom;
	int colFrom = move.colFrom;
	int rowTo = move.rowTo;
	int colTo = move.colTo;
	
	// -1 if back, 0 if none, 1 if forward
	int rowDir = 2 * (rowFrom < rowTo) - (rowFrom != rowTo);
	int colDir = 2 * (colFrom < colTo) - (colFrom != colTo);

	// check if nothing in way
	int row = rowFrom + rowDir;
	int col = colFrom + colDir;
	while (row != rowTo || col != colTo) {
		if (board[row][col] != ' ') return false;
		row += rowDir;
		col += colDir;
	}
	return true;
}


bool Chessboard::checkDest(bool isWhite, int row, int col) {
	if (row < 0 || 8 <= row || col < 0 || 8 <= col) return false;
	return (board[row][col] == ' ' || white[row][col])
}
