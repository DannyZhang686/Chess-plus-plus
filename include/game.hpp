#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <vector>
#include "main.hpp"

class Player {
  protected:
    std::string name;
    bool white = false;
  public:
    // Constructors
    Player(std::string, bool);
    Player();

    // Accessors
    std::string getName();
    bool getWhite();
    
    // Mutators
    void setName(std::string);
    void setWhite(bool);
};

class Board {
  protected:
    std::vector<std::vector<Cell>> board;
  public:
    // Constructor
    Board();

    // Accessor
    Cell getCell(int, int);
};

class Game {
  protected:
    Board board;
    Player Player1;
    Player Player2;
    std::vector<std::string> moves;
    bool static whiteInCheck;
    bool static blackInCheck;
    bool static whiteInCheckmate;
    bool static blackInCheckmate;
    bool static whiteInStalemate;
    bool static blackInStalemate;
  public:
    // Constructor
    Game();
    
    // Accessors
    Player getPlayer(int);
    Board getBoard();
    std::vector<std::string> getMoves();
    bool static getWhiteInCheck();
    bool static getBlackInCheck();
    bool static getWhiteInCheckmate();
    bool static getBlackInCheckmate();
    bool static getWhiteInStalemate();
    bool static getBlackInStalemate();
    
    // (Interactive) mutators
    void setNames();
    void swapNames();
    void setMoves(std::string);
    void editMove(std::string, int);
    void clearMoves();
    void static setWhiteInCheck(bool);
    void static setBlackInCheck(bool);
    void static setWhiteInCheckmate(bool);
    void static setBlackInCheckmate(bool);
    void static setWhiteInStalemate(bool);
    void static setBlackInStalemate(bool);

    // Turn processors
    int processTurn1();
    int processTurn2();
    void static updateState();
    void static mateCheck(bool);
};

#endif //_GAME_HPP_