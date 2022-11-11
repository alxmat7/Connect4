#pragma once
#include <memory>

namespace Connect4
{
    class Board;
    class GameView;

    class GameController
    {
    private:

        std::shared_ptr<Board> board_;
        std::shared_ptr<GameView> gameView_;

    public:
        GameController(bool isSimulation = false);
        void run();
        virtual ~GameController() {};
    };
}
