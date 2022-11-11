#include "MctsAiPlayer.h"
#include <cmath>
#include <cassert>

namespace Connect4
{
    MctsAiPlayer::MctsAiPlayer(int iterations, int randSeed) : iterations_{ iterations }, mtRand_{ randSeed }{}

    void MctsAiPlayer::play(Board& board)
    {
        Node root(board);
        for (int iter = 0; iter < iterations_; iter++)
        {
            bool isAiTurn = true;
            Node* nd = treePolicy_(&root, isAiTurn);
            int reward = defaultPolicy(nd, isAiTurn);
            backup_(nd, reward, isAiTurn); //no need to pass paramenters... just pass reward based on whether its aiturn
        }

        Node* bChild = bestChild(&root, 0);
        board = bChild->getBoard();
    }

    MctsAiPlayer::~MctsAiPlayer()
    {
        for (int i = 0; i < nodes_.size(); ++i)
        {
            delete nodes_[i];
        }
    }
    /**
    *   function TREEPOLICY(v)
    *       while v is nonterminal do
    *           if v not fully expanded then
    *               return EXPAND(v)
    *           else
    *               v <- BESTCHILD(v, Cp)
    *       return v
    */
    Node* MctsAiPlayer::treePolicy_(Node* v, bool& isAiTurn)
    {
        while (v->isTerminal() == false)
        {
            //if not fully expanded

            //remove isAiTurn from expand_ and add isAiTurn = ~isAiTurn here.. make sure it works.

            if (v->isFullyExpanded() == false)
            {
                return expand_(v, isAiTurn);
            }
            else
            {
                isAiTurn = !isAiTurn;
                v = bestChild(v, 2.0); //explore factor of 2.0 change this to something else!!!!!
            }
        }
        return v;
    }

    Node* MctsAiPlayer::expand_(Node* v, bool& isAiTurn)
    {
        auto board = v->getBoard(); //make a copy.
        const auto& rowInColumn = board.getMoves();
        //assert here. v should not be 7 or more.

        static int nRows = board.getNumRows(); //in play , initialize nRows and nCols.
        static int nCols = board.getNumCols();

        //create a method for getting valid moves, because this is repeated code.
        auto& moves = board.getMoves();
        std::vector<int> validMoves;
        for (int i = 0; i < moves.size(); i++)
        {
            if (moves[i] < nRows)
            {
                validMoves.push_back(i);
            }
        }

        auto& triedMoves = v->getTriedMoves();
        int nextMove = -1;
        for (int move : validMoves)
        {
            if (std::find(triedMoves.begin(), triedMoves.end(), move) == triedMoves.end())
            {
                nextMove = move;
                break;
            }
        }

        assert(nextMove != -1);

        board.dropPiece(nextMove, isAiTurn ? Board::Markers::AI_PLAYER : Board::Markers::HUMAN_PLAYER);
        v->addMove(nextMove);

        Node* childNode = new Node(board);
        nodes_.push_back(childNode);
        v->addChild(childNode);


        isAiTurn = !isAiTurn;

        return childNode;
    }

    Node* MctsAiPlayer::bestChild(const Node* v, float exploreFactor)
    {
        const auto& children = v->getChildren();
        Node* bestChild = nullptr;
        double bestUcb1Value = -std::numeric_limits<double>::max();

        //if float is used instead of double, we can have a different result with even with a
        //relatively small number of visits. For example with visits = 710 and reward = -43
        //ucb value is 0.23950075301512672 with double, and 0.239500761 with float.
        //With visits = 664 and reward = -47, the values are 0.23950075347874555 and 0.239500761
        //Notice that the values with float calculations are the same.

        for (Node* child : children)
        {
            int visits = child->getVisits();
            int reward = child->getReward();
            double exploit = (reward * 1.0 / visits); //we start with visits = 1, so we wont have to worry about divide by zero error.
            double explore = std::sqrt(2.0 * (std::log(v->getVisits())) / visits);
            double ucb1Value = exploit + exploreFactor * explore;

            if (ucb1Value > bestUcb1Value)
            {
                bestUcb1Value = ucb1Value;
                bestChild = child;
            }
        }
        return bestChild;
    }

    int MctsAiPlayer::defaultPolicy(const Node* v, bool isAiTurn)
    {
        int numRows = v->getBoard().getNumRows();
        int numCols = v->getBoard().getNumCols();
        Board brd = v->getBoard(); //make a copy. We are going to modify this.
        while (brd.gameEnded() == false) //check if state(board) is non-terminal.
        {
            auto& moves = brd.getMoves();
            std::vector<int> validMoves;
            for (int i = 0; i < moves.size(); i++)
            {
                if (moves[i] < numRows)
                {
                    validMoves.push_back(i);
                }
            }
            //uniform_int_distribution is a lightweight object and doesn't introduce a bottleneck. This
            //was benchmarked against a custom random generator that doesnt require changing range everytime it is called.
            int upperBound = validMoves.size() - 1;
            int randTurnIndex = std::uniform_int_distribution<int>{ 0, upperBound }(mtRand_); // range [0, upperBound]
            int col = validMoves[randTurnIndex];

            brd.dropPiece(col, isAiTurn ? Board::Markers::AI_PLAYER : Board::Markers::HUMAN_PLAYER);
            isAiTurn = !isAiTurn;
        }
        auto winner = brd.getWinner();
        if (winner == Board::Markers::AI_PLAYER)
        {
            return 1;
        }
        else if (winner == Board::Markers::HUMAN_PLAYER)
        {
            return -1;
        }
        else
        {
            return 0;
        }
        return 0;
    }

    void MctsAiPlayer::backup_(Node* v, int reward, bool isAiTurn)
    {
        reward = isAiTurn ? -reward : reward;
        while (v)
        {
            v->updateVisits();
            v->updateReward(reward);
            v = v->getParent();
            reward = -reward;
        }
    }

    const Board& Node::getBoard() const
    {
        return board_;
    }

    const std::vector<Node*>& Node::getChildren() const
    {
        return children_;
    }

    const std::vector<int>& Node::getTriedMoves() const
    {
        return triedMoves_;
    }

    void Node::addMove(int move)
    {
        triedMoves_.push_back(move);
    }

    void Node::addChild(Node* child)
    {
        children_.push_back(child);
        child->setParent_(this);
    }

    int Node::getVisits() const
    {
        return visits_;
    }

    bool Node::isFullyExpanded() const
    {
        auto& moves = board_.getMoves();
        auto numRows = board_.getNumRows();
        int numValidMoves = 0;
        for (int i = 0; i < moves.size(); i++)
        {
            if (moves[i] < numRows)
            {
                numValidMoves++;
            }
        }
        return (numValidMoves == children_.size());
    }

    int Node::getReward() const
    {
        return reward_;
    }

    void Node::updateVisits()
    {
        visits_++;
    }

    void Node::updateReward(int reward)
    {
        reward_ += reward;
    }

    Node* Node::getParent() const
    {
        return parent_;
    }

    void Node::setParent_(Node* parent)
    {
        parent_ = parent;
    }

    Node::Node(Board& board)
    {
        board_ = board;
        parent_ = nullptr;
        visits_ = 1; // 0 in the algorithm, but this doesn't affect gameplay when number of simulations is sufficiently large.
        reward_ = 0;
    }

    bool Node::isTerminal() const
    {
        return board_.gameEnded();
    }
}