#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>

class Board;

class GameView
{
private:

    sf::RenderWindow* window_;
    std::shared_ptr<Board> board_; 

    int nRows_;
    int nCols_;
    int gridSize_;

    int gameWindowWidth_;
    int gameWindowHeight_;

    int dropPieceX_; 


public:
    GameView() = delete;
    GameView(std::shared_ptr<Board> board);
    sf::RenderWindow* windowHandle() const;
    void setPiecePosition(int mouseX);
    void draw();
    int gridSize() const;
    virtual ~GameView() = default;
};


