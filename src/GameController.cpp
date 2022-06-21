#include <iostream>
#include <cassert>
#include <chrono>
#include <string>
#include <thread>
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
	int miniMaxDepth = 2;
	MiniMaxAiPlayer miniMaxAIPlayer(miniMaxDepth);

	auto window = gameView_->windowHandle();
	assert(window);

	bool humanPlayerTurn = true;
	bool gameEnded = false;

	while (window->isOpen())
	{

		const std::string msgTie = "Game ended in a tie";
		const std::string msgAiWins = "AI Wins";
		const std::string msgHumanWins = "You Win!";
		std::string msg;

		if (humanPlayerTurn)
		{
			sf::Event event;
			while (window->pollEvent(event))
			{
				if (event.type == sf::Event::MouseMoved)
				{
					gameView_->setPiecePosition(event.mouseMove.x);
				}

				if (event.type == sf::Event::MouseButtonPressed)
				{
					if (event.mouseButton.button == sf::Mouse::Left)
					{
#ifndef NDEBUG
						std::cout << "the right button was pressed" << std::endl;
						std::cout << "mouse x: " << event.mouseButton.x << std::endl;
						std::cout << "mouse y: " << event.mouseButton.y << std::endl;
#endif // !NDEBUG
						int col = event.mouseButton.x / gameView_->gridSize();
						bool validDrop = false;
						validDrop = board_->dropPiece(col, Board::HUMAN_PLAYER);
						if (validDrop == false)
						{
							continue;
						}
						humanPlayerTurn = false;
					}
				}

				if (event.type == sf::Event::Closed)
				{
					window->close();
				}

			}
		}
		else
		{
			//ai Player Plays now
			if (humanPlayerTurn == false)
			{
				Board brdCopy = *board_;
				miniMaxAIPlayer.play(*board_);
#ifndef NDEBUG			
				miniMaxAIPlayer.playNoAlphaBeta(brdCopy);
#endif
				humanPlayerTurn = true;
			}
		}

		gameView_->draw();
		
		//Do we have a winner?
		auto winner = board_->getWinner();

		if (winner == Board::HUMAN_PLAYER)
		{
			std::cout << msgHumanWins << std::endl;
			gameEnded = true;
			msg = msgHumanWins;
		}
		else if (winner == Board::AI_PLAYER)
		{
			std::cout << msgAiWins << std::endl;
			gameEnded = true;
			msg = msgAiWins;
		}
		else if (winner == Board::NONE && !board_->validMovesExist())
		{
			std::cout << msgTie << std::endl;
			gameEnded = true;
			msg = msgTie;
		}

		if (gameEnded)
		{
			using std::chrono::operator""s;
			std::this_thread::sleep_until(std::chrono::system_clock::now() + 2s);
			std::cout << msg << std::endl;
			window->close();
		}
	}
}