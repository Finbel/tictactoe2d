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

    int alpha = numeric_limits<int>::min();
    int beta = numeric_limits<int>::max();

    int v = alpha;      // will hold our best value
    int stateValue = 0; // will hold the temporary value

    int bestStateIndex = -1;

    for (uint i = 0; i < nextStates.size(); i++)
    {
        auto state = nextStates[i];
        stateValue = alphabeta(state, 3, alpha, beta, CELL_X);
        if (stateValue > v)
        {
            v = stateValue;
            bestStateIndex = i;
        }
        alpha = max(alpha, v);
    }

    return nextStates[bestStateIndex];
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

    int player = 2;

    //sum of the number of marks in each row, column, diagonals that do not
    //contain any of the opponents marks.

    int playerCellValue = 3 - player;
    int OtherPlayerCellValue = player;
    //this is 1 for player with X and 2 for player with O
    vector<int> differentSums(10, 0);
    vector<bool> nonNulledLine(10, false);
    //the first 4 are for the colunms, the following for the rows and the last for the diagonals
    for (int c = 0; c < 4; c++)
    {
        for (int r = 0; r < 4; r++)
        {
            int valueOfCell = state.at(r, c);

            //this one is 1 for X and 2 for O
            if (valueOfCell == playerCellValue)
            {
                //this is if the value is the one that represents the players
                differentSums[c] = (differentSums[c] + 1);

                differentSums[4 + r] = (differentSums[4 + r] + 1);
                if (r == c)
                {
                    //this is for one of the diagonals
                    differentSums[8] = (differentSums[8] + 1);
                }
                if (3 - r == c)
                {
                    //this is for the other of the diagonals
                    differentSums[9] = (differentSums[9] + 1);
                }
            }
            else if (valueOfCell == OtherPlayerCellValue)
            {
                //this is the cell has a value that is not empty and also not the players
                nonNulledLine[c] = true;
                nonNulledLine[4 + r] = true;
                if (r == c)
                {
                    nonNulledLine[8] = true;
                }
                if (3 - r == c)
                {
                    nonNulledLine[9] = true;
                }
            }
        }
    }
    int returnSum = 0;
    for (int i = 0; i < 10; i++)
    {
        if (!nonNulledLine[i])
        {
            returnSum += differentSums[i];
        }
    }
    return returnSum;
}

/*namespace TICTACTOE*/ }
