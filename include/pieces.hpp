#ifndef _PIECES_HPP_
#define _PIECES_HPP_

#include "main.hpp"

class Piece {
  protected:
    bool white; // Colour of a piece (true = white, false = black)
    char type; // p = pawn, n = knight, b = bishop, r = rook, q = queen, k = king, x = empty
    bool hasMoved; // Pawns can move twice on their first move so this is a special distinction
    
    // This variable is always zero for non-pawn pieces
    // For pawns, it is set to 2 when a pawn moves two
    // squares and decremented by 1 every ply (one move
    // by one player) to a minimum of 0. When the
    // variable is positive, an en passant capture of
    // the pawn is permitted because the opponent has
    // just moved it.
    int ePCountDown;

  public:
    // Constructors
    Piece();
    Piece(bool, char, bool);

    // Accessors
    bool getWhite();
    char getType();
    bool getHasMoved();    
    int getEPCountDown(); // For en passant only

    // Mutators
    void setWhite(bool);
    void setType(char);
    void setHasMoved(bool);
    void setEPCountDown(int); // For en passant only
};

class Cell {
  protected:
    int file; // Files run up and down (conventially the letters on a board)
    int rank; // Ranks run left and right (numbers on a board)
    Piece piece; // Piece on the cell
  
  public:
    // Constructors
    Cell();
    Cell(int, int, Piece);
    
    // Accessors
    int getFile();
    int getRank();
    Piece& getPiece();
    
    // Mutators
    void setPiece(Piece);
};

class Pawn : public Piece { // ***INHERITS*** certain properties of a piece (position, type)
  public:
    // Constructors
    Pawn(bool, bool);

    // Accessors
    bool getHasMoved();

    // Mutators
    void setHasMoved(bool);

    //Methods
    int validMove(std::string, std::string);
};

class Knight : public Piece {
  public:
    // Constructor
    Knight(bool);
    int validMove(std::string, std::string);
};

class Bishop : public Piece {
  public:
    Bishop(bool);
    int validMove(std::string, std::string);
};

class Rook : public Piece {
  public:
    Rook(bool, bool);
    int validMove(std::string, std::string);
};

class Queen : public Piece {
  public:
    Queen(bool);
    int validMove(std::string, std::string);
};

class King : public Piece {
  public:
    King(bool, bool);
    int validMove(std::string, std::string);
};

// Converters
Pawn convertToPawn(Piece);
Knight convertToKnight(Piece);
Bishop convertToBishop(Piece);
Rook convertToRook(Piece);
Queen convertToQueen(Piece);
King convertToKing(Piece);

#endif //_PIECES_HPP_