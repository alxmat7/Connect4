#include <iostream>
#include <cassert>
#include <chrono>
#include <string>
#include <thread>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "GameController.h"
#include "GameView.h"
#include "MctsAiPlayer.h"
#include "MiniMaxAiPlayer.h"
namespace Connect4
{
    GameController::GameController(bool isSimulation) : board_(std::make_shared<Board>()), gameView_(isSimulation ? nullptr : std::make_shared<GameView>(board_))
    {
    }

    /**
     * Run the game loop
     */
    void GameController::run()
    {
        // if we are running a simulation
        if (gameView_ == nullptr)
        {
            board_->reset();

            int numSims = 10000;
            int numAIWins = 0;
            int numShadowAIWins = 0;
            int numTies = 0;

            MiniMaxAiPlayer* shadowAiPlayer = new MiniMaxAiPlayer(8);
            for (int i = 0; i < numSims; i++)
            {
                //Created on heap because this can cause the allocated stack to be filled up.
                // TODO: Need code to reset random number generator engine instead of deleting objects in loop.
                MctsAiPlayer* aiPlayer = new MctsAiPlayer(5000, i);

                auto winner = Board::Markers::NONE;

                //alternating first player.
                bool fp = ((i % 2) == 0);

                while (true)
                {
                    if (fp)
                    {
                        //simulated Human player.
                        board_->flipMarkers();
                        shadowAiPlayer->play(*board_);
                        board_->flipMarkers();
                    }
                    else
                    {
                        aiPlayer->play(*board_);
                    }
                    //board_->print();
                    winner = board_->getWinner();
                    if (winner == Board::Markers::HUMAN_PLAYER)
                    {
                        numShadowAIWins++;
                        break;
                    }
                    else if (winner == Board::Markers::AI_PLAYER)
                    {
                        numAIWins++;
                        break;
                    }
                    else if (winner == Board::Markers::NONE && !board_->validMovesExist())
                    {
                        numTies++;
                        break;
                    }
                    fp = !fp;
                }

                board_->reset();
                delete aiPlayer;

                std::cout << "Simulation # " << i << ":" << std::endl;
                float shadowAIWinPerc = 100.0f * numShadowAIWins / (i + 1);
                float aiWinPerc = 100.0f * numAIWins / (i + 1);
                float tiePerc = 100.0f * numTies / (i + 1);

                std::cout << "Shadow AI wins: " << shadowAIWinPerc << " %" << std::endl;
                std::cout << "AI wins:        " << aiWinPerc << " %" << std::endl;
                std::cout << "Ties:           " << tiePerc << " %" << std::endl;
                std::cout << "Total:          " << shadowAIWinPerc + aiWinPerc + tiePerc << " %" << std::endl;
                std::cout << "----------------------" << std::endl;
            }
            delete shadowAiPlayer;
            return;
        }


        //create opponent;
        //constexpr int miniMaxDepth{ 4 }; //change the depth to increase or decrease look-up depth.
        //MiniMaxAiPlayer aiPlayer(miniMaxDepth);
        MctsAiPlayer aiPlayer(8000, 45);

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
                            validDrop = board_->dropPiece(col, Board::Markers::HUMAN_PLAYER);
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
                    aiPlayer.play(*board_);
#ifndef NDEBUG			
                    //move th is code to inside play with NDEBUG
                    //miniMaxAIPlayer.playNoAlphaBeta(brdCopy);
#endif
                    humanPlayerTurn = true;
                }
            }

            gameView_->draw();

            //Do we have a winner?
            auto winner = board_->getWinner();

            if (winner == Board::Markers::HUMAN_PLAYER)
            {
                std::cout << msgHumanWins << std::endl;
                gameEnded = true;
                msg = msgHumanWins;
            }
            else if (winner == Board::Markers::AI_PLAYER)
            {
                std::cout << msgAiWins << std::endl;
                gameEnded = true;
                msg = msgAiWins;
            }
            else if (winner == Board::Markers::NONE && !board_->validMovesExist())
            {
                std::cout << msgTie << std::endl;
                gameEnded = true;
                msg = msgTie;
            }

            if (gameEnded)
            {
                using std::chrono::operator""s;
                std::this_thread::sleep_until(std::chrono::system_clock::now() + 10s);
                std::cout << msg << std::endl;
                window->close();
            }
        }
    }
}