#include <iostream>
#include <cassert>
#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include "GameController.h"
#include "GameView.h"
#include "MiniMaxAiPlayer.h"


GameController::GameController(): board_(std::make_shared<Board>()), gameView_(std::make_shared<GameView>(board_))
{
}

void GameController::run()
{

	//create opponent;
	MiniMaxAiPlayer miniMaxAIPlayer(2);

	auto window = gameView_->windowHandle();
	assert(window);

	bool humanPlayerTurn = true;
	bool gameEnded = false;

	while (window->isOpen())
	{

		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::MouseMoved)
			{
				gameView_->setPiecePosition(event.mouseMove.x);
			}

			if (!gameEnded && humanPlayerTurn && (event.type == sf::Event::MouseButtonPressed))
			{
				if (event.mouseButton.button == sf::Mouse::Left)
				{
#ifndef NDEBUG
					std::cout << "the right button was pressed" << std::endl;
					std::cout << "mouse x: " << event.mouseButton.x << std::endl;
					std::cout << "mouse y: " << event.mouseButton.y << std::endl;
#endif // !NDEBUG
					int col = event.mouseButton.x / gameView_->gridSize();
					board_->dropPiece(col, Board::HUMAN_PLAYER);
					humanPlayerTurn = false;
				}
			}

			if (event.type == sf::Event::Closed)
			{
				window->close(); //should call the destcurctor?
			}

		}

		//Do we have a winner?
		auto winner = board_->getWinner();
		if (winner == Board::AI_PLAYER)
		{
			std::cout << "AI Player wins" << std::endl;
			gameEnded = true;
		}
		else if (winner == Board::HUMAN_PLAYER)
		{
			std::cout << "Human Player wins" << std::endl;
			gameEnded = true;
		}
		else if (winner == Board::NONE && !board_->validMovesExist())
		{
			std::cout << "Game ended in a tie" << std::endl;
			gameEnded = true;
		}

		//ai Player Plays now
		if (!gameEnded && (humanPlayerTurn == false))
		{
			Board brdCopy = *board_;
			miniMaxAIPlayer.play(*board_);
#ifndef NDEBUG			
            miniMaxAIPlayer.playNoAlphaBeta(brdCopy);
#endif
			humanPlayerTurn = true;
		}


		//this should happenw hen some method like 'rendor' is called on GameView.
		gameView_->draw();

		//Do we have a winner?
		winner = board_->getWinner();
		if (winner == Board::AI_PLAYER)
		{
			std::cout << "AI Player wins" << std::endl;
			gameEnded = true;
		}
		else if (winner == Board::HUMAN_PLAYER)
		{
			std::cout << "Human Player wins" << std::endl;
			gameEnded = true;
		}
		else if (winner == Board::NONE && !board_->validMovesExist())
		{
			std::cout << "Game ended in a tie" << std::endl;
			gameEnded = true;
		}
	}
	

}

