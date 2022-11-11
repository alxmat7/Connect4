#pragma once

namespace Connect4
{
    class Board;
    class Player
    {
    public:

        Player() = default;
        virtual void play(Board& board) = 0;
        virtual ~Player() {};
    };
}
