#ifdef WIN32
#include <windows.h>
#endif
#include <cassert>
#include <algorithm>
#include <iostream>
#include "Board.h"
#include "Globals.h"


Board::Board(int nRows, int nCols) : nRows_{ nRows }, nCols_{ nCols }, board_(nRows_, std::vector<Markers>(nCols_, Markers::NONE)), validLocations_(nCols_)
{
}

Board::Board(std::vector<std::vector<Markers>> &board) : board_{ board }, validLocations_(board[0].size())
{
	nRows_ = board_.size();
	nCols_ = board_[0].size();

	//disregard cache thrashing. Probably faster this way.

	for (int j = 0; j < nCols_; j++)
	{
		int i = 0;
		while((i < nRows_) && (board_[i][j] != Markers::NONE))
		{
			i++;
		}
		validLocations_[j] = i;
	}


}

int Board::getNumCols() const
{
	return nCols_;
}

int Board::getNumRows() const
{
	return nRows_;
}


Board::Markers Board::getWinner() const
{
	// Check diagonal oriented '/'
	for (int r = 0; r <= nRows_ - CONNECT_SIZE; r++)
	{
		for (int c = 0; c <= nCols_ - CONNECT_SIZE; c++)
		{
			auto marker = board_[r][c];
			if (marker != Markers::NONE && marker == board_[r + 1][c + 1] && marker == board_[r + 2][c + 2] && marker == board_[r + 3][c + 3])
			{
				return marker;
			}
		}
	}

	// Check diagonal oriented '\'
	for (int r = 0; r <= nRows_ - CONNECT_SIZE; r++)
	{
		for (int c = CONNECT_SIZE - 1; c < nCols_; c++)
		{
			auto marker = board_[r][c];
			if (marker != Markers::NONE && marker == board_[r + 1][c - 1] && marker == board_[r + 2][c - 2] && marker == board_[r + 3][c - 3])
			{
				return marker;
			}
		}
	}

	// Check rows (horizontal)
	for (int r = 0; r < nRows_; r++)
	{
		for (int c = 0; c <= nCols_ - CONNECT_SIZE; c++)
		{
			auto marker = board_[r][c];
			if (marker != Markers::NONE && marker == board_[r][c + 1] && marker == board_[r][c + 2] && marker == board_[r][c + 3])
			{
				return marker;
			}
		}
	}

	// Check columns (vertical)
	for (int r = 0; r <= nRows_ - CONNECT_SIZE; r++)
	{
		for (int c = 0; c < nCols_; c++)
		{
			auto marker = board_[r][c];
			if (marker != Markers::NONE && marker == board_[r + 1][c] && marker == board_[r + 2][c] && marker == board_[r + 3][c])
			{
				return marker;
			}
		}
	}

	return Markers::NONE;
}

bool Board::dropPiece(int col, Markers marker)
{
	assert(col < nCols_);
	int row = validLocations_[col];
	if (row >= nRows_)
	{
		std::cout << "Cannot drop any more pieces" << std::endl;
		return false;
	}
	board_[row][col] = marker;

	//update available locations
	validLocations_[col]++;
	return true;
}


bool Board::validMovesExist() const
{
	const auto& topRow = board_[nRows_ - 1];
	return (std::find(topRow.begin(), topRow.end(), Markers::NONE) != topRow.end()); // There is at least one empty space (Board::NONE) in top row
}

void Board::print() const
{
#ifdef WIN32
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	int colorAi = 14;
	int colorHuman = 4;
	int colorDefault = 15;
#endif

	std::cout << "           ";
	for (int i = 0; i < getNumCols(); i++)
	{
		std::cout << i << " ";
	}
	std::cout << std::endl;
	for (int i = getNumRows() - 1; i >= 0; i--)
	{
		std::cout << "           ";
		for (int j = 0; j < getNumCols(); j++)
		{
			char printChar = (board_[i][j] == Board::AI_PLAYER ? 'o' : (board_[i][j] == Board::HUMAN_PLAYER ? 'x' : '.'));
#ifdef WIN32
			if (board_[i][j] == Board::AI_PLAYER)
			{
				SetConsoleTextAttribute(hConsole, colorAi);
			}
			else if (board_[i][j] == Board::HUMAN_PLAYER)
			{
				SetConsoleTextAttribute(hConsole, colorHuman);
			}
			else
			{
				SetConsoleTextAttribute(hConsole, colorDefault);
			}
#endif
			std::cout << printChar << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

#ifdef WIN32
	SetConsoleTextAttribute(hConsole, colorDefault);
#endif
}

const std::vector<std::vector<Board::Markers>>& Board::getBoard() const
{
	return board_;
}

const std::vector<int>& Board::getMoves() const
{
	return validLocations_;
}


Board::~Board()
{
}
