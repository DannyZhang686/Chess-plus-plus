#ifndef _SPECIAL_RULES_HPP_
#define _SPECIAL_RULES_HPP_

#include "main.hpp"

bool castlingCheck(std::string, std::string, Cell, Cell);
void ePCountDecrease();
int enPassantCheck(std::string, Cell, int);
void pawnPromotion(Game&);

#endif //_SPECIAL_RULES_HPP_