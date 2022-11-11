#include "GameView.h"
#include "Board.h"

namespace Connect4
{
    /**
     * Set up display parameters based on board size.
     */
    GameView::GameView(std::shared_ptr<Board> board)
    {
        nRows_ = board->getNumRows();
        nCols_ = board->getNumCols();
        board_ = board;

        auto nColsWindow = nCols_;
        auto nRowsWindow = nRows_ + 1; //+1 because we need one row free for the drop position

        auto screenWidth = sf::VideoMode::getDesktopMode().width;
        auto screenHeight = sf::VideoMode::getDesktopMode().height;

        //scale down to a reasonable size
        auto windowWidth = 0.5 * screenWidth;
        auto windowHeight = 0.5 * screenHeight;

        // cast to remove ambiguity. Some of these are narrowing conversions.
        gridSize_ = std::min(static_cast<unsigned int>(windowWidth / nColsWindow), static_cast<unsigned int>(windowHeight / nRowsWindow));
        gameWindowWidth_ = static_cast<unsigned int>(gridSize_ * nColsWindow);
        gameWindowHeight_ = static_cast<unsigned int>(gridSize_ * nRowsWindow);

        dropPieceX_ = 0;

        window_ = new sf::RenderWindow(sf::VideoMode(gameWindowWidth_, gameWindowHeight_), "Connect 4");

    }

    /**
     * Return the window.
     */
    sf::RenderWindow* GameView::windowHandle() const
    {
        return window_;
    }

    /**
     * Set the position of the piece to drop based on where the mouse is.
     * We are only interested in the position along the horizontal axis. The user is not allowed to
     * move the piece vertically.
     */
    void GameView::setPiecePosition(int mouseX)
    {
        dropPieceX_ = mouseX;
    }

    /**
     * Display the board.
     */
    void GameView::draw()
    {
        window_->clear();
        sf::RectangleShape blueBackground;
        blueBackground.setSize(sf::Vector2f(nCols_ * gridSize_, nRows_ * gridSize_));
        blueBackground.setFillColor(sf::Color::Blue);
        blueBackground.setPosition(0, gridSize_);

        window_->draw(blueBackground);

        sf::CircleShape pieceToDrop(gridSize_ / 2);
        pieceToDrop.setFillColor(sf::Color::Yellow);
        pieceToDrop.setPosition(dropPieceX_ - gridSize_ / 2, 0);

        window_->draw(pieceToDrop);

        for (int c = 0; c < nCols_; c++)
        {
            for (int r = 0; r < nRows_; r++)
            {
                sf::CircleShape gridCircle(gridSize_ / 2);
                gridCircle.setPosition(c * gridSize_, (nRows_ - r) * gridSize_);

                auto gameBoard = board_->getBoard();
                auto color = (gameBoard[r][c] == Board::Markers::NONE) ? sf::Color::Black : (gameBoard[r][c] == Board::Markers::AI_PLAYER) ? sf::Color::Red : sf::Color::Yellow;
                gridCircle.setFillColor(color);

                window_->draw(gridCircle);
            }
        }
        for (int i = 1; i < nCols_; i++)
        {
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(i * gridSize_, gridSize_)),
                sf::Vertex(sf::Vector2f(i * gridSize_, gameWindowHeight_)) //second is y (height)
            };

            window_->draw(line, 2, sf::Lines);
        }
        for (int i = 1; i <= nRows_; i++)
        {
            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(0, i * gridSize_)),
                sf::Vertex(sf::Vector2f(gameWindowWidth_, i * gridSize_)) //second is y (height)
            };

            window_->draw(line, 2, sf::Lines);
        }

        window_->display();
    }

    /**
     * Return the grid size (width of each column or row)
     */
    int GameView::gridSize() const
    {
        return gridSize_;
    }
}