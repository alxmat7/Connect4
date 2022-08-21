#include "MiniMaxAiPlayer.h"
#include "Board.h"
#include "Globals.h"
#include <cassert>
#include <climits>
#include <iostream>
#include <algorithm>
#include <chrono> 

MiniMaxAiPlayer::MiniMaxAiPlayer(int depth) : depth_{depth}, WINNING_SCORE{ 1000 }
{

}

void MiniMaxAiPlayer::play(Board &board)
{
    int bestMove = -1;
#ifndef NDEBUG
    auto t1 = std::chrono::high_resolution_clock::now();
#endif
    miniMax_(board, bestMove, depth_, INT_MIN, INT_MAX, true);
#ifndef NDEBUG
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << std::endl;
    std::cout<< "Minimax computation time: "<< duration<< " microseconds ~ " << duration/1000000 << " seconds" << std::endl;
    std::cout << "AI drops piece on column " << bestMove << "." << std::endl;
#endif
    board.dropPiece(bestMove, Board::AI_PLAYER);
}

void MiniMaxAiPlayer::playNoAlphaBeta(Board& board)
{

    int bestMove = -1;
    auto t1 = std::chrono::high_resolution_clock::now();
    miniMaxBasic(board, bestMove, depth_, true);
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << std::endl;
    std::cout << "Minimax(no alpha beta) computation time: " << duration << " microseconds ~ " << duration / 1000000 << " seconds" << std::endl;
    std::cout << "AI (no alpha beta) drops piece on column " << bestMove << "." << std::endl;
}

/**
* Minimax with alpha-beta pruning. Significantly faster than plain vanilla minimax. 
*/
int MiniMaxAiPlayer::miniMax_(const Board& currentBoard, int &bestMove, int depth, int alpha, int beta, bool isMaximizingPlayer)
{

    //Check if there are any more valid moves.
    bool validMovesExist = currentBoard.validMovesExist();
    if (depth == 0 || !validMovesExist)
    {
        auto winner = currentBoard.getWinner();
        if (winner == Board::AI_PLAYER) //Did the AI win?
        {
            return WINNING_SCORE; 
        }
        else if (winner == Board::HUMAN_PLAYER) //Did the human Player win?
        {
            return -WINNING_SCORE;
        }
        else if (!validMovesExist && winner == Board::NONE) //Did the game end in a tie?
        {
            return 0;
        }
        else //game is still in progress
        {
            assert(depth == 0);
            return computeScore_(currentBoard);
        }
    }

    int bestValue;

    if (isMaximizingPlayer)
    {
        bestValue = INT_MIN;
        const std::vector<int>& rowInColumn = currentBoard.getMoves();
        for (int col = 0 ; col < rowInColumn.size() ; col++)
        {
            if (rowInColumn[col] >= currentBoard.getNumRows())
            {
                continue;
            }
            //make a copy of the board and apply the move
            //TODO: apply move and undo move instead of making a copy. This is especially important for larger games.
            Board temp = currentBoard; 

            temp.dropPiece(col, Board::AI_PLAYER);

            int tempBestMove; //it seems you can pass in bestMove. it functions very much like a global variable.

            int score = miniMax_(temp, tempBestMove, depth - 1, alpha, beta, false);
            if (score > bestValue)
            {
                bestValue = score;
                bestMove = col;
            }
            alpha = std::max(alpha, bestValue);
            if (beta <= alpha)
            {
                break;
            }
        }
        return bestValue; // well bestValue can be returned at the end of the function, but this is good for readability.
    }
    else /*if not the maximizer*/
    {
        bestValue = INT_MAX;
        const std::vector<int>& rowInColumn = currentBoard.getMoves();
        for (int col = 0; col < rowInColumn.size(); col++)
        {
            if (rowInColumn[col] >= currentBoard.getNumRows())
            {
                continue;
            }
            //make a copy of the board and apply the move
            Board temp = currentBoard;

            temp.dropPiece(col, Board::HUMAN_PLAYER);

            int tempBestMove; //it seems you can pass in bestMove. it functions very much like a global variable.

            int score = miniMax_(temp, tempBestMove, depth - 1, alpha, beta, true);
            if (score < bestValue)
            {
                bestValue = score;
                bestMove = col;
            }
            beta = std::min(beta, bestValue);
            if (beta <= alpha)
            {
                break;
            }
        }
        return bestValue; // well bestValue can be returned at the end of the function, but this is good for readability.
    }
}

/**
* This is the basic minimax code. It can really be combined with miniMax code to avoid code duplication, but having
* it separately allows anyone to understand the basic Minimax AI algorithm.
*/
int MiniMaxAiPlayer::miniMaxBasic(const Board& currentBoard, int& bestMove, int depth, bool isMaximizingPlayer)
{
    //Check if there are any more valid moves.
    bool validMovesExist = currentBoard.validMovesExist();
    if (depth == 0 || !validMovesExist)
    {
        auto winner = currentBoard.getWinner();
        //currentBoard.print();
        if (winner == Board::AI_PLAYER) //Did the AI win?
        {
            return WINNING_SCORE; //Changed to reflect depth
        }
        else if (winner == Board::HUMAN_PLAYER) //Did the human Player win?
        {
            return -WINNING_SCORE; //changed to reflect depth.
        }
        else if (!validMovesExist && winner == Board::NONE) //Did the game end in a tie?
        {
            return 0;
        }
        else //game is still in progress
        {
            assert(depth == 0);
            return computeScore_(currentBoard);
        }
    }

    int bestValue;

    if (isMaximizingPlayer)
    {
        bestValue = INT_MIN;
        const std::vector<int>& rowInColumn = currentBoard.getMoves();
        for (int col = 0; col < rowInColumn.size(); col++)
        {
            if (rowInColumn[col] >= currentBoard.getNumRows())
            {
                continue;
            }
            //make a copy of the board and apply the move
            Board temp = currentBoard;

            temp.dropPiece(col, Board::AI_PLAYER);
            //temp.print();

            int tempBestMove; //it seems you can pass in bestMove. it functions very much like a global variable.

            int score = miniMaxBasic(temp, tempBestMove, depth - 1, false);
            if (score > bestValue)
            {
                bestValue = score;
                bestMove = col;
            }
        }
        return bestValue; // well bestValue can be returned at the end of the function, but this is good for readability.
    }
    else  /*if not the maximizer*/
    {
        bestValue = INT_MAX;
        const std::vector<int>& rowInColumn = currentBoard.getMoves();
        for (int col = 0; col < rowInColumn.size(); col++)
        {
            if (rowInColumn[col] >= currentBoard.getNumRows())
            {
                continue;
            }
            //make a copy of the board and apply the move
            Board temp = currentBoard;

            temp.dropPiece(col, Board::HUMAN_PLAYER);
            //temp.print();

            int tempBestMove; //it seems you can pass in bestMove. it functions very much like a global variable.

            int score = miniMaxBasic(temp, tempBestMove, depth - 1, true);
            if (score < bestValue)
            {
                bestValue = score;
                bestMove = col;
            }
        }
        return bestValue; // well bestValue can be returned at the end of the function, but this is good for readability.
    }
}


int MiniMaxAiPlayer::computeScore_(const Board& board) const
{
    int score = 0;
    const auto& board_ = board.getBoard();
    size_t nRows = board_.size();
    size_t nCols = board_[0].size();


    //Uncomment the block below to try a heurisitic function that gives more weight to center area (like in chess strategy)
    /*
    // Center column
    
    auto cColumn = nCols / 2;
    int cCount = 0;
    for (auto r = 0; r < nRows; r++)
    {
        cCount += (board_[r][cColumn] == Board::AI_PLAYER);
        //cCount -= (board_[r][cColumn] == Board::HUMAN_PLAYER);
    }
    score += cCount * 3;
    
    // Left of center
    cColumn = nCols / 2 - 1;
    cCount = 0;
    for (auto r = 0; r < nRows; r++)
    {
        cCount += (board_[r][cColumn] == Board::AI_PLAYER);
        cCount -= (board_[r][cColumn] == Board::HUMAN_PLAYER);
    }
    score += cCount * 2;

    // Right of center
    cColumn = nCols / 2 + 1;
    cCount = 0;
    for (auto r = 0; r < nRows; r++)
    {
        cCount += (board_[r][cColumn] == Board::AI_PLAYER);
        cCount -= (board_[r][cColumn] == Board::HUMAN_PLAYER);
    }
    score += cCount * 2;
    */

    // Check diagonal oriented '/'
    for (auto r = 0; r <= nRows - CONNECT_SIZE; r++)
    {
        for (auto c = 0; c <= nCols - CONNECT_SIZE; c++)
        {
            score += lineScore_(board_[r][c], board_[r + 1][c + 1], board_[r + 2][c + 2], board_[r + 3][c + 3]);
        }
    }

    // Check diagonal oriented '\'
    for (auto r = 0; r <= nRows - CONNECT_SIZE; r++)
    {
        for (auto c = CONNECT_SIZE - 1; c < nCols; c++)
        {
            score += lineScore_(board_[r][c], board_[r + 1][c - 1] ,board_[r + 2][c - 2],board_[r + 3][c - 3]);
        }
    }

    // Check rows (horizontal)
    for (auto r = 0; r < nRows; r++)
    {
        for (auto c = 0; c <= nCols - CONNECT_SIZE; c++)
        {
            score += lineScore_(board_[r][c], board_[r][c + 1], board_[r][c + 2], board_[r][c + 3]);
        }
    }

    // Check columns (vertical)
    for (auto r = 0; r <= nRows - CONNECT_SIZE; r++)
    {
        for (auto c = 0; c < nCols; c++)
        {
            score += lineScore_(board_[r][c], board_[r + 1][c], board_[r + 2][c], board_[r + 3][c]);
        }
    }
    
    return score;
}

//Write a lambda instead.
int MiniMaxAiPlayer::count_(Board::Markers m1, Board::Markers m2, Board::Markers m3, Board::Markers m4, Board::Markers m) const
{
    return (m == m1) + (m == m2) + (m == m3) + (m == m4);
}

int MiniMaxAiPlayer::lineScore_(Board::Markers m1, Board::Markers m2, Board::Markers m3, Board::Markers m4) const
{
    int score = 0;
    
    int numAiMarkers = count_(m1, m2, m3, m4, Board::AI_PLAYER);
    int numHumanMarkers = count_(m1, m2, m3, m4, Board::HUMAN_PLAYER);
    int numEmptyMarkers = CONNECT_SIZE - numAiMarkers - numHumanMarkers;

    //opponent is always human Player.
    if (numAiMarkers == 4)
        return WINNING_SCORE; // I think we never come here.
    else if (numHumanMarkers == 4)
        return -WINNING_SCORE; // I dont think we ever come here. Need an assert statement here.
    else if (numAiMarkers == 3 && numEmptyMarkers == 1)
        return 5;
    else if (numAiMarkers == 2 && numEmptyMarkers == 2)
        return 2;
    else if (numHumanMarkers == 3 && numEmptyMarkers == 1)
        return -4;
    else
        return score;
}


