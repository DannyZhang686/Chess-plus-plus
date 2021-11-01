#include <iostream>
#include <stdlib.h>

// Pieces

class Piece {
  private:
    int file; // Files are left to right (conventially the letters on a board)
    int rank; // Ranks are up to down (numbers on a board)
    int type; // 0 pawn, 1 n, 2 b, 3 r, 4 q, 5 k
    
  public:
    Piece(int a, int b, bool c) {
      file = a;
      rank = b;
      hasMoved = c;
    }
    bool getMoved() {
      return hasMoved;
    }
    int getFile() {
      return file;
    }
    int getRank(){
      return rank;
    }
};