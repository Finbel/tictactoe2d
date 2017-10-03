#ifndef _TICTACTOE_PLAYER_HPP_
#define _TICTACTOE_PLAYER_HPP_

#include "constants.hpp"
#include "deadline.hpp"
#include "move.hpp"
#include "gamestate.hpp"
#include <vector>
#include <unordered_map>
#include <string>

namespace TICTACTOE
{

class Player
{
public:
  std::unordered_map<std::string, int> stringMap;
  Player();
  ///perform a move
  ///\param pState the current state of the board
  ///\param pDue time before which we must have returned
  ///\return the next state the board is in after our move
  GameState play(const GameState &pState, const Deadline &pDue);
  int alphabeta(const GameState &pState, int depth, int alpha, int beta, const uint8_t player);
  int naive_utility(uint8_t player, const GameState &pState);
};

/*namespace TICTACTOE*/ }

#endif
