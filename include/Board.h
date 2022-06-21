#pragma once
#include <vector>

class Board
{

public:

	enum Markers : char
	{
		AI_PLAYER,
		HUMAN_PLAYER,
		NONE,
	};

	Board(int nRows = 6, int nCols = 7);
	Board(std::vector<std::vector <Markers>> &board); //for testing an arbitrary starting state.
	int getNumCols() const;
    int getNumRows() const; //make them inline
	Board::Markers getWinner() const;
    bool dropPiece(int col, Markers marker);
	bool validMovesExist() const;
    void print() const;
	const std::vector<std::vector <Markers>> &getBoard() const;
    const std::vector<int>& getMoves() const;
	virtual ~Board();

private:

	int nCols_;
	int nRows_;
	std::vector<std::vector <Markers>> board_;
    std::vector<int> validLocations_;

};

