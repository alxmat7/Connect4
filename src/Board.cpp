#ifdef WIN32
#include <windows.h>
#endif
#include <cassert>
#include <algorithm>
#include <iostream>
#include "Board.h"
#include "Globals.h"


Board::Board(size_t nRows, size_t nCols) : nRows_{ nRows }, nCols_{ nCols }, board_(nRows_, std::vector<Markers>(nCols_, Markers::NONE)), validLocations_(nCols_)
{
}

/**
 * For testing only - start with a specified board configuration
 */
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

/**
 * Get the number of columns in the board.
 */
size_t Board::getNumCols() const
{
    return nCols_;
}

/**
 * Get the number of rows in the board.
 */
size_t Board::getNumRows() const
{
    return nRows_;
}

/**
 * Checks if there is a winner based on the current board state.
 * The winner can be AI_PLAYER, HUMAN_PLAYER or NONE (if the game is a tie, or if it is still in progress)
 */
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

/**
 * 'Drop' a connect4 piece (AI_PLAYER or HUMAN_PLAYER) to a particular column. 
 */
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

    //update available locations, because now we have one fewer location to drop a piece.
    validLocations_[col]++;
    return true;
}

/**
 * Check if the board still has emptly slots where a piece can be dropped.
 */
bool Board::validMovesExist() const
{
    const auto& topRow = board_[nRows_ - 1];
    return (std::find(topRow.begin(), topRow.end(), Markers::NONE) != topRow.end()); // There is at least one empty space (Board::NONE) in top row
}

/**
 * Print the board on console for diagnostics. Prints in color on Windows.
 */
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
    for (auto i = 0; i < getNumCols(); i++)
    {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    for (auto i = getNumRows() - 1; i >= 0; i--)
    {
        std::cout << "           ";
        for (auto j = 0; j < getNumCols(); j++)
        {
            char printChar = (board_[i][j] == Markers::AI_PLAYER ? 'o' : (board_[i][j] == Board::Markers::HUMAN_PLAYER ? 'x' : '.'));
#ifdef WIN32
            if (board_[i][j] == Markers::AI_PLAYER)
            {
                SetConsoleTextAttribute(hConsole, colorAi);
            }
            else if (board_[i][j] == Markers::HUMAN_PLAYER)
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

/**
 * Return the vector representing the board.
 */
const std::vector<std::vector<Board::Markers>>& Board::getBoard() const
{
    return board_;
}

/**
 * Returns the locations (row numbers) where the next piece goes.
 * For example, if the current board is
 *  row 3 |. . . . . . .|
 *  row 1 |. . x . . . .|
 *  row 0 |o x o o x . o| <--bottommost row
 *        --------------
 * this method returns [1, 1, 2, 1, 1, 0, 1]
 */
const std::vector<int>& Board::getMoves() const
{
    return validLocations_;
}
