#pragma once
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

namespace Connect4
{
    class Board;

    class GameView
    {
    private:

        sf::RenderWindow* window_;
        std::shared_ptr<Board> board_;

        size_t nRows_;
        size_t nCols_;
        unsigned int gridSize_;

        unsigned int gameWindowWidth_;
        unsigned int gameWindowHeight_;

        int dropPieceX_;


    public:
        GameView() = delete;
        GameView(std::shared_ptr<Board> board);
        sf::RenderWindow* windowHandle() const;
        void setPiecePosition(int mouseX);
        void draw();
        int gridSize() const;
        virtual ~GameView() {};
    };
}

