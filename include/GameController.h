#pragma once
#include <memory>

class Board;
class GameView;

class GameController
{
private:

    std::shared_ptr<Board> board_;
    std::shared_ptr<GameView> gameView_;

public:
    GameController();
    void run();
    virtual ~GameController() {};
};

