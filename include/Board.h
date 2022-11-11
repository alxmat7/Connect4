#pragma once
#include <vector>

namespace Connect4
{
    class Board
    {
    public:
        enum class Markers : char
        {
            AI_PLAYER,
            HUMAN_PLAYER,
            NONE,
        };

        Board(size_t nRows = 6, size_t nCols = 7);
        Board(std::vector<std::vector <Markers>>& board); //for testing an arbitrary starting state.
        size_t getNumCols() const;
        size_t getNumRows() const; //make them inline
        Board::Markers getWinner() const;
        bool dropPiece(int col, Markers marker);
        bool validMovesExist() const;
        bool gameEnded() const;
        void print() const;
        void reset();
        void flipMarkers();
        const std::vector<std::vector <Markers>>& getBoard() const;
        const std::vector<int>& getMoves() const;
        virtual ~Board() {};

    private:

        size_t nCols_;
        size_t nRows_;
        std::vector<std::vector <Markers>> board_;
        std::vector<int> validLocations_;

    };
}
