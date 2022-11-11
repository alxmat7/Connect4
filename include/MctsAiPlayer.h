#pragma once

#include "Player.h"
#include "Board.h" 
#include <vector>
#include <random>

namespace Connect4
{
    class Node
    {
    private:

        std::vector<Node*> children_;
        std::vector<int> triedMoves_;
        Board board_;
        int visits_;
        int reward_;
        Node* parent_;
        void setParent_(Node* parent);

    public:

        Node(Board& board);
        bool isTerminal() const;
        const Board& getBoard() const; //state.
        const std::vector<Node*>& getChildren() const;
        const std::vector<int>& getTriedMoves() const;
        void addMove(int move);
        void addChild(Node* child);
        int getVisits() const;
        bool isFullyExpanded() const;
        int getReward() const;
        void updateVisits();
        void updateReward(int reward);
        Node* getParent() const;

    };

    class MctsAiPlayer :public Player
    {
    public:

        MctsAiPlayer() = delete;
        MctsAiPlayer(int iterations, int randSeed);
        virtual void play(Board& board) override;
        virtual ~MctsAiPlayer();

    private:

        int iterations_;
        std::vector<Node*> nodes_;
        Node* treePolicy_(Node* v, bool& isAiTurn);
        Node* expand_(Node* v, bool& isAiTurn);
        Node* bestChild(const Node* v, float exploreFactor);
        int defaultPolicy(const Node* v, bool isAiTurn);
        void backup_(Node* v, int reward, bool isAiTurn);

        // Mersenne Twister Random number engine
        // https://cplusplus.com/reference/random/mersenne_twister_engine/
        // Suitable for Monte Carlo Experiments, generates a large series with approximately uniform distribution.
        // Initialize with a hardware based random device engine 
        // static std::random_device randDev
        std::mt19937 mtRand_;
    };
}


