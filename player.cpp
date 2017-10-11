#include "player.hpp"
#include "constants.hpp"
#include <cstdlib>
#include <limits>
#include <algorithm>

using std::vector;
using std::endl;
using std::cerr;
using std::max;
using std::min;
using std::numeric_limits;

namespace TICTACTOE
{

Player::Player(){
    //cerr << "Constructor " << endl;
};

GameState Player::play(const GameState &pState, const Deadline &pDue)
{
    //cerr << "Processing " << pState.toMessage() << endl;

    vector<GameState>
        nextStates;
    pState.findPossibleMoves(nextStates);

    if (nextStates.size() == 0)
        return GameState(pState, Move());

    /*
     * Here you should write your clever algorithms to get the best next move, ie the best
     * next state. This skeleton returns a random move instead.
     */

    int bestValue = numeric_limits<int>::min();
    int stateValue;
    int bestStateIdx = -1;
    int alpha = numeric_limits<int>::min();
    int beta = numeric_limits<int>::max();

    int searchDepth = 5;

    for (int i = 0; i < nextStates.size(); i++)
    {

        stateValue = alphabeta(nextStates[i], searchDepth, alpha, beta, 2);
        if (stateValue > bestValue)
        {
            bestValue = stateValue;
            bestStateIdx = i;
            if (pState.getNextPlayer() == CELL_X)
            {
                alpha = max(alpha, bestValue);
            }
        }
        //System.err.println("||||||||best state is at "+i);
    }

    if (pState.getNextPlayer() == CELL_X)
    {
        return nextStates[bestStateIdx];
    }
    else
    {
        return nextStates[rand() % nextStates.size()];
    }
}

int Player::alphabeta(const GameState &pState, int depth, int alpha, int beta, const uint8_t player)
{
    //std::cerr << "Processing " << pState.toMessage() << " " << depth << std::endl;

    // check if already calculated
    auto search = stringMap.find(pState.toMessage());
    if (search != stringMap.end())
    {
        return search->second;
    }

    // initialize necessary variables
    int v;
    vector<GameState> nextStates;
    pState.findPossibleMoves(nextStates);

    // If max depth or no more moves
    if (depth == 0 || nextStates.size() == 0)
    {
        v = utility(pState);
    } // if player = A
    else if (player == CELL_X)
    {
        // v = - inf
        v = numeric_limits<int>::min();
        // for each child in u(A, state)
        for (GameState state : nextStates)
        {
            v = max(v, alphabeta(state, depth - 1, alpha, beta, CELL_O));
            alpha = max(alpha, v);
            if (beta <= alpha)
            {
                break;
            }
        }
    }
    else
    {
        v = numeric_limits<int>::max();
        for (GameState state : nextStates)
        {
            v = min(v, alphabeta(state, depth - 1, alpha, beta, CELL_X));
            beta = min(beta, v);
            if (beta <= alpha)
            {
                break;
            }
        }
    }
    stringMap[pState.toMessage()] = v;
    return v;
}

int Player::utility(const GameState &state)
{
    int player = CELL_X;
    int notplayer = CELL_O;
    vector<int> occupiedX(10, 0);
    vector<bool> blockedX(10, false);

    //Lists to keep score for O
    vector<int> occupiedO(10, 0);
    vector<bool> blockedO(10, false);

    //the first 4 are for the colunms, the following for the rows and the last for the diagonals
    for (int c = 0; c < 4; c++)
    {
        for (int r = 0; r < 4; r++)
        {
            int valueOfCell = state.at(r, c);

            //this one is 1 for X and 2 for O
            if (valueOfCell == player)
            {
                //this is the cell has a value that is not empty and also not player Xs
                blockedO[c] = true;
                blockedO[4 + r] = true;
                if (r == c)
                {
                    blockedO[8] = true;
                }
                if (3 - r == c)
                {
                    blockedO[9] = true;
                }

                //this is if the value is the one that represents the players
                occupiedX[c] = (occupiedX[c] + 1);

                occupiedX[4 + r] = (occupiedX[4 + r] + 1);
                if (r == c)
                {
                    //this is for one of the diagonals
                    occupiedX[8] = (occupiedX[8] + 1);
                }
                if (3 - r == c)
                {
                    //this is for the other of the diagonals
                    occupiedX[9] = (occupiedX[9] + 1);
                }
            }
            else if (valueOfCell == notplayer)
            {
                //this is if the value is the one that represents the players
                occupiedO[c] = (occupiedO[c] + 1);

                occupiedO[4 + r] = (occupiedO[4 + r] + 1);
                if (r == c)
                {
                    //this is for one of the diagonals
                    occupiedO[8] = (occupiedO[8] + 1);
                }
                if (3 - r == c)
                {
                    //this is for the other of the diagonals
                    occupiedO[9] = (occupiedO[9] + 1);
                }

                //this is the cell has a value that is not empty and also not the players
                blockedX[c] = true;
                blockedX[4 + r] = true;
                if (r == c)
                {
                    blockedX[8] = true;
                }
                if (3 - r == c)
                {
                    blockedX[9] = true;
                }
            }
        }
    }
    int SumX = 0;
    for (int i = 0; i < 10; i++)
    {
        if (!blockedX[i])
        {
            SumX += occupiedX[i];
        }
    }

    int SumO = 0;
    for (int i = 0; i < 10; i++)
    {
        if (!blockedO[i])
        {
            SumO += occupiedO[i];
        }
    }

    return (SumX - SumO);
}

/*namespace TICTACTOE*/ }
