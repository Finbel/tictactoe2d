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
  std::vector<std::vector<int>> winVector{
      {0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11}, {12, 13, 14, 15}, //rows
      {0, 4, 8, 12},
      {1, 5, 9, 13},
      {2, 6, 10, 14},
      {3, 7, 11, 15}, //columns
      {0, 5, 10, 15},
      {3, 6, 9, 12} //diagonals
  };

  Player();
  ///perform a move
  ///\param pState the current state of the board
  ///\param pDue time before which we must have returned
  ///\return the next state the board is in after our move
  GameState play(const GameState &pState, const Deadline &pDue);
  int alphabeta(const GameState &pState, int depth, int alpha, int beta, const uint8_t player);
  int utility(const GameState &pState);
};

/*namespace TICTACTOE*/ }

#endif
