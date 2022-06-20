#pragma once

#include "Player.h"
#include "Board.h" 

//CHECK method capitalization.. google style guide.

class MiniMaxAiPlayer :public Player
{
public:

	MiniMaxAiPlayer() = delete;
	MiniMaxAiPlayer(int depth);
	virtual void play(Board& board) override;
	virtual void playNoAlphaBeta(Board& board);
	virtual ~MiniMaxAiPlayer() = default;

private:
	int computeScore_(const Board& board) const;
	int count_(Board::Markers m1, Board::Markers m2, Board::Markers m3, Board::Markers m4, Board::Markers m) const;
	int lineScore_(Board::Markers m1, Board::Markers m2, Board::Markers m3, Board::Markers m4) const;
	int miniMax_(const Board& currentBoard, int& bestMove, int depth, int alpha, int beta, bool isMaximizingPlayer);
	int miniMaxBasic(const Board& currentBoard, int& bestMove, int depth, bool isMaximizingPlayer);
	const int depth_;
    const int WINNING_SCORE;

};

