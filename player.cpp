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

    int bestValue = -2147483648;
    int stateValue;
    int bestStateIdx = -1;
    int alpha = -2147483648;
    int beta = 2147483647;

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
    //sum of the number of marks in each row, column, diagonals that do not
    //contain any of the opponents marks.
    // if (state.getNextPlayer() == CELL_O)
    // {
    //     //current player is X
    //     player = CELL_X;
    //     notplayer = CELL_O;
    // }
    // else
    // {
    //     // current player is O
    //     player = CELL_O;
    //     notplayer = CELL_X;
    // }
    //Lists to keep score for X
    vector<int> differentSumsX(10, 0);
    vector<bool> nonNulledLineX(10, false);

    //Lists to keep score for O
    vector<int> differentSumsO(10, 0);
    vector<bool> nonNulledLineO(10, false);

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
                nonNulledLineO[c] = true;
                nonNulledLineO[4 + r] = true;
                if (r == c)
                {
                    nonNulledLineO[8] = true;
                }
                if (3 - r == c)
                {
                    nonNulledLineO[9] = true;
                }

                //this is if the value is the one that represents the players
                differentSumsX[c] = (differentSumsX[c] + 1);

                differentSumsX[4 + r] = (differentSumsX[4 + r] + 1);
                if (r == c)
                {
                    //this is for one of the diagonals
                    differentSumsX[8] = (differentSumsX[8] + 1);
                }
                if (3 - r == c)
                {
                    //this is for the other of the diagonals
                    differentSumsX[9] = (differentSumsX[9] + 1);
                }
            }
            else if (valueOfCell == notplayer)
            {
                //this is if the value is the one that represents the players
                differentSumsO[c] = (differentSumsO[c] + 1);

                differentSumsO[4 + r] = (differentSumsO[4 + r] + 1);
                if (r == c)
                {
                    //this is for one of the diagonals
                    differentSumsO[8] = (differentSumsO[8] + 1);
                }
                if (3 - r == c)
                {
                    //this is for the other of the diagonals
                    differentSumsO[9] = (differentSumsO[9] + 1);
                }

                //this is the cell has a value that is not empty and also not the players
                nonNulledLineX[c] = true;
                nonNulledLineX[4 + r] = true;
                if (r == c)
                {
                    nonNulledLineX[8] = true;
                }
                if (3 - r == c)
                {
                    nonNulledLineX[9] = true;
                }
            }
        }
    }
    int returnSumX = 0;
    for (int i = 0; i < 10; i++)
    {
        if (!nonNulledLineX[i])
        {
            returnSumX += differentSumsX[i];
        }
    }

    int returnSumO = 0;
    for (int i = 0; i < 10; i++)
    {
        if (!nonNulledLineO[i])
        {
            returnSumO += differentSumsO[i];
        }
    }

    // O = opponent
    // X = player

    return (returnSumX - returnSumO);
}

/*namespace TICTACTOE*/ }
