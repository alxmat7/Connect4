#include "GameView.h"
#include "Board.h"

GameView::GameView(std::shared_ptr<Board> board)
{
    nRows_ = board->getNumRows();
	nCols_ = board->getNumCols();
	board_ = board;

	int nColsWindow = nCols_;
	int nRowsWindow = nRows_ + 1; //+1 because we need one row free for the drop position

	auto screenWidth = sf::VideoMode::getDesktopMode().width;
	auto screenHeight = sf::VideoMode::getDesktopMode().height;

	//scale down to a reasonable size
	auto windowWidth = 0.5 * screenWidth;
	auto windowHeight = 0.5 * screenHeight;

	gridSize_ = std::min(windowWidth / nColsWindow, windowHeight / nRowsWindow);

	gameWindowWidth_ = gridSize_ * nColsWindow;
	gameWindowHeight_ = gridSize_ * nRowsWindow;

	dropPieceX_ = 0;

	window_ = new sf::RenderWindow(sf::VideoMode(gameWindowWidth_, gameWindowHeight_), "Connect 4");

}

sf::RenderWindow* GameView::windowHandle() const
{
	return window_; //how do you return const pointer?
}

void GameView::setPiecePosition(int mouseX)
{
	dropPieceX_ = mouseX;
}

void GameView::draw()
{

	window_->clear();
	sf::RectangleShape blueBackground;
	blueBackground.setSize(sf::Vector2f(nCols_ * gridSize_, nRows_ * gridSize_));
	blueBackground.setFillColor(sf::Color::Blue);
	blueBackground.setPosition(0, gridSize_);

    window_->draw(blueBackground);


	sf::CircleShape pieceToDrop(gridSize_ / 2);
	pieceToDrop.setFillColor(sf::Color::Red);

	pieceToDrop.setPosition(dropPieceX_ - gridSize_ / 2, 0);
	
	window_->draw(pieceToDrop);

	for (int c = 0; c < nCols_; c++)
	{
		for (int r = 0; r < nRows_; r++)
		{
			sf::CircleShape gridCircle(gridSize_ / 2);
			gridCircle.setPosition(c * gridSize_, (nRows_ - r) * gridSize_);

			auto gameBoard = board_->getBoard();
			auto color = (gameBoard[r][c] == Board::NONE) ? sf::Color::Black : (gameBoard[r][c] == Board::AI_PLAYER) ? sf::Color::Yellow : sf::Color::Red;


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

int GameView::gridSize() const
{
	return gridSize_;
}
