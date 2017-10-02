#include "player.hpp"
#include "constants.hpp"
#include <cstdlib>
#include <limits>
#include <algorithm>

namespace TICTACTOE
{

GameState Player::play(const GameState &pState, const Deadline &pDue)
{
    //std::cerr << "Processing " << pState.toMessage() << std::endl;

    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);

    if (lNextStates.size() == 0)
        return GameState(pState, Move());

    /*
     * Here you should write your clever algorithms to get the best next move, ie the best
     * next state. This skeleton returns a random move instead.
     */
    int alpha = std::numeric_limits<int>::min();
    int beta = std::numeric_limits<int>::max();
    int v = alpha;
    int player = pState.getNextPlayer() ^ (CELL_X | CELL_O);
    GameState pick;
    for (GameState state : lNextStates)
    {
        v = std::max(v, alphabeta(state, 3, alpha, beta, CELL_X));

        if (alpha < v)
        {
            pick = state;
        }
        alpha = std::max(alpha, v);
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

    int v;
    std::vector<GameState> lNextStates;
    pState.findPossibleMoves(lNextStates);

    if (depth == 0 || lNextStates.size() == 0)
    {
        v = utility(player, pState);
    }
    else if (player == CELL_X)
    {
        v = std::numeric_limits<int>::min();
        for (GameState state : lNextStates)
        {
            v = std::max(v, alphabeta(state, depth - 1, alpha, beta, player ^ (CELL_X | CELL_O)));
            alpha = std::max(alpha, v);
            if (beta <= alpha)
            {
                break;
            }
        }
    }
    else
    {
        v = std::numeric_limits<int>::max();
        for (GameState state : lNextStates)
        {
            v = std::min(v, alphabeta(state, depth - 1, alpha, beta, player ^ (CELL_X | CELL_O)));
            beta = std::min(beta, v);
            if (beta <= alpha)
            {
                break;
            }
        }
    }
    return v;
}

int Player::utility(uint8_t player, const GameState &pState)
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

/*namespace TICTACTOE*/ }
