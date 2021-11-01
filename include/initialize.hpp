#ifndef _INITIALIZE_HPP_
#define _INITIALIZE_HPP_

#include <map>
#include "pieces.hpp"
#include "game.hpp"

extern std::map<std::string, Cell> chessBoard;
extern Board board;

void initialize();

#endif //_INITIALIZE_HPP_