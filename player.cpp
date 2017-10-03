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
        lNextStates;
    pState.findPossibleMoves(lNextStates);

    if (lNextStates.size() == 0)
        return GameState(pState, Move());

    /*
     * Here you should write your clever algorithms to get the best next move, ie the best
     * next state. This skeleton returns a random move instead.
     */
    int alpha = numeric_limits<int>::min();
    int beta = numeric_limits<int>::max();
    int v = alpha;
    int player = pState.getNextPlayer() ^ (CELL_X | CELL_O);
    GameState pick;
    for (GameState state : lNextStates)
    {
        v = max(v, alphabeta(state, 3, alpha, beta, CELL_X));

        if (alpha < v)
        {
            pick = state;
        }
        alpha = max(alpha, v);
    }

    if (player == CELL_X)
    {
        return pick;
    }
    else
    {
        return lNextStates[rand() % lNextStates.size()];
    }
    return pick;
}

int Player::alphabeta(const GameState &pState, int depth, int alpha, int beta, const uint8_t player)
{
    //std::cerr << "Processing " << pState.toMessage() << " " << depth << std::endl;
    auto search = stringMap.find(pState.toMessage());
    if (search != stringMap.end())
    {
        return search->second;
    }

    int v;
    vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);

    if (depth == 0 || lNextStates.size() == 0)
    {
        v = utility(player, pState);
    }
    else if (player == CELL_X)
    {
        v = numeric_limits<int>::min();
        for (GameState state : lNextStates)
        {
            v = max(v, alphabeta(state, depth - 1, alpha, beta, player ^ (CELL_X | CELL_O)));
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
        for (GameState state : lNextStates)
        {
            v = min(v, alphabeta(state, depth - 1, alpha, beta, player ^ (CELL_X | CELL_O)));
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

int Player::naive_utility(uint8_t player, const GameState &pState)
{
    uint8_t opponent = player ^ (CELL_X | CELL_O);
    uint playerSum = 0;
    uint value;
    uint occupying;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            value = (((i + j) % 3) != 0) && (i != j) ? 2 : 3;
            occupying = pState.at(i, j);
            if (occupying == player)
            {
                playerSum += value;
            }
            if (occupying == opponent)
            {
                playerSum -= value;
            }
        }
    }
    return playerSum;
}

int Player::utility(uint8_t player, const GameState &pState)
{
    uint8_t opponent = player ^ (CELL_X | CELL_O);
    // rows, columns and diagonals where player can win
    uint playerWinPaths = 0;
    // rows, columns and diagonals where opponent can win
    uint opponentWinPaths = 0;
    // the most marks the player has on a possible win path
    uint max_player = 0;
    // the most marks the opponent has on a possible win path
    uint max_opponent = 0;

    // used to count marks on a possible win path
    uint count_opponent;
    uint count_player;

    for (vector<int> win : winVector)
    {
        for (int windex : win)
        {
            // assume that no one has placed a mark on this path
            count_player = 0;
            count_opponent = 0;
            if (pState.at(windex) == player)
            {
                count_player += 1;
            }
            else if (pState.at(windex) == opponent)
            {
                count_opponent += 1;
            }
        }
        // check if the path belongs to any player
        if (count_opponent == 0 && count_player != 0)
        {
            if (count_player > max_player)
            {
                max_player = count_player;
            }
            playerWinPaths += 1;
        }
        else if (count_player == 0 && count_opponent != 0)
        {
            if (count_opponent > max_opponent)
            {
                max_opponent = count_opponent;
            }
            opponentWinPaths += 1;
        }
    }
    return (max_player - max_opponent) + (playerWinPaths - opponentWinPaths);
}

/*namespace TICTACTOE*/ }
