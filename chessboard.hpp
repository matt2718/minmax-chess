#ifndef _CHESSBOARD_HPP
#define _CHESSBOARD_HPP

#include <vector>

struct Move {
	int rowFrom;
	int colFrom;
	int rowTo;
	int colTo;

	bool operator==(const Move& other);
};

class Chessboard {
public:
	Chessboard();
	~Chessboard();

	// makes move, returns captured piece
	char makeMove(Move move);

	// returns whether move is legal for given player
	bool isLegalMove(bool isWhite, Move move);

	// retuns vector of legal moves for piece
	std::vector<Move> getLegalMoves(int row, int col);
	
	// returns vector of legal moves for player
	std::vector<Move> getLegalMoves(bool isWhite);

	// previous move made
	Move getLastMove();
	
	// returns piece at given location
	char getPiece(int row, int col);

	// returns color of piece at location or last piece that was there
	bool getColor(int row, int col);
	
	// adds piece to board, returns true if successful
	bool addPiece(char piece, bool isWhite, int row, int col);

	// removes piece from board, returns piece
	char removePiece(int row, int col);

	// returns how much the board favors a player
	int getAdvantage(bool isWhite);

	// returns how much a move favors a player
	int getAdvChange(Move move, bool isWhite);

	// prints the board (duh)
	void printBoard(bool isWhite);
	
private:
	// calculates how much the boad favors white
	int advCalc();
	
	// determines if line is clear between endpoints
	bool checkLine(Move move);

	// determines if piece can move into space
	bool checkDest(bool isWhite, int row, int col);

	// previous move made
	Move lastMove = {-1, -1, -1, -1};
	
	// track whether castling is possible
	bool whiteKingMoved, blackKingMoved;
	bool whiteRook0Moved, blackRook0Moved;
	bool whiteRook7Moved, blackRook7Moved;

	// if pawn was moved 2 ahead last turn (en passant)
	int lastWhitePawn = -1;
	int lastBlackPawn = -1;

	// how much the board favors white
	int whiteAdv = 0;

	// piece array
	char board[8][8] = {
		{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}, // white
		{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'}, // white
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
		{'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'}, // black
		{'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'}  // black
	};

	// color of pieces on board
	bool white[8][8] = {
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 1, 1, 1, 1, 1, 1, 1},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0, 0}
	};
};

#endif
