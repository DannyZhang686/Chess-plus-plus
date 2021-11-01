// Sets up the game (board with pieces)

#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include "include/main.hpp"
using namespace std;

// We are using a map which links coordinates to a Cell object
// We use a coordinate to find a corresponding cell like so:
// chessBoard.find(COORDINATE_HERE)->second
// ->second indicates that we want to get the Cell
// Usually, getPiece() will be called afterwards to get whatever's on that square
map<string, Cell> chessBoard;
Board board;

// Set up the board
void initialize() {
  // Reset game flags
  Game::setWhiteInCheck(false); 
  Game::setBlackInCheck(false);
  Game::setWhiteInCheckmate(false);
  Game::setBlackInCheckmate(false);
  Game::setWhiteInStalemate(false);
  Game::setBlackInStalemate(false);
  chessBoard.clear(); // Wipe the chessboard
  string curCoordinates; // Coordinates of current piece
  string pieceOrder = "rnbqkbnr"; // Order of pieces in initial state
  for (int i = 1; i <= 8; i++) {
    for (int j = 1; j <= 8; j++) {
      // To start, make everything empty squares
      Cell curCell(i, j, Piece()); 
      string tempFile(1, (char) (i + 96));      
      curCoordinates = (tempFile) + to_string(j);
      chessBoard.insert(make_pair(curCoordinates, curCell));
    }
  }

  for (int i = 1; i <= 8; i++) { // Then, put the pawns and pieces in
    // Creating White pawns
    string tempFile(1, (char) (i + 96));
    chessBoard.find((tempFile) + "2")->second.getPiece().setType('p');
    chessBoard.find((tempFile) + "2")->second.getPiece().setWhite(true);
    
    // Black pawns
    chessBoard.find((tempFile) + "7")->second.getPiece().setType('p');
    chessBoard.find((tempFile) + "7")->second.getPiece().setWhite(false);

    // White pieces
    chessBoard.find((tempFile) + "1")->second.getPiece().setType(pieceOrder[i-1]); // Because pieceOrder is 0-indexed
    chessBoard.find((tempFile) + "1")->second.getPiece().setWhite(true);

    // Black pieces
    chessBoard.find((tempFile) + "8")->second.getPiece().setType(pieceOrder[i-1]);
    chessBoard.find((tempFile) + "8")->second.getPiece().setWhite(false);
  }


  // Castling Debugging position
/*
  chessBoard.find("e1")->second.getPiece().setType('k');
  chessBoard.find("e1")->second.getPiece().setWhite(true);

  chessBoard.find("h1")->second.getPiece().setType('r');
  chessBoard.find("h1")->second.getPiece().setWhite(true);

  chessBoard.find("a1")->second.getPiece().setType('r');
  chessBoard.find("a1")->second.getPiece().setWhite(true);

  chessBoard.find("a6")->second.getPiece().setType('n');
  chessBoard.find("a6")->second.getPiece().setWhite(true);

  chessBoard.find("a3")->second.getPiece().setType('n');
  chessBoard.find("a3")->second.getPiece().setWhite(false);

  chessBoard.find("e8")->second.getPiece().setType('k');
  chessBoard.find("e8")->second.getPiece().setWhite(false);
  
  chessBoard.find("a8")->second.getPiece().setType('r');
  chessBoard.find("a8")->second.getPiece().setWhite(false);

  chessBoard.find("h8")->second.getPiece().setType('r');
  chessBoard.find("h8")->second.getPiece().setWhite(false);
*/

  // Promotion debugging position
  /*
  chessBoard.find("e1")->second.getPiece().setType('k');
  chessBoard.find("e1")->second.getPiece().setWhite(true);

  chessBoard.find("g7")->second.getPiece().setType('p');
  chessBoard.find("g7")->second.getPiece().setWhite(true);

  chessBoard.find("a1")->second.getPiece().setType('n');
  chessBoard.find("a1")->second.getPiece().setWhite(true);

  chessBoard.find("e8")->second.getPiece().setType('k');
  chessBoard.find("e8")->second.getPiece().setWhite(false);

  chessBoard.find("b2")->second.getPiece().setType('p');
  chessBoard.find("b2")->second.getPiece().setWhite(false);

  chessBoard.find("h8")->second.getPiece().setType('n');
  chessBoard.find("h8")->second.getPiece().setWhite(false);
*/

  // Checkmate debugging position
 /* chessBoard.find("h1")->second.getPiece().setType('k');
  chessBoard.find("h1")->second.getPiece().setWhite(true);

  chessBoard.find("h2")->second.getPiece().setType('p');
  chessBoard.find("h2")->second.getPiece().setWhite(true);

  chessBoard.find("g2")->second.getPiece().setType('p');
  chessBoard.find("g2")->second.getPiece().setWhite(true);

  chessBoard.find("f2")->second.getPiece().setType('p');
  chessBoard.find("f2")->second.getPiece().setWhite(true);

  chessBoard.find("e8")->second.getPiece().setType('k');
  chessBoard.find("e8")->second.getPiece().setWhite(false);

  chessBoard.find("a8")->second.getPiece().setType('r');
  chessBoard.find("a8")->second.getPiece().setWhite(false);*/

  // Stalemate debugging position
/*
  chessBoard.find("h1")->second.getPiece().setType('k');
  chessBoard.find("h1")->second.getPiece().setWhite(true);

  chessBoard.find("d2")->second.getPiece().setType('k');
  chessBoard.find("d2")->second.getPiece().setWhite(false);

  chessBoard.find("e2")->second.getPiece().setType('q');
  chessBoard.find("e2")->second.getPiece().setWhite(false);
*/
  // En passant debugging
/*
  chessBoard.find("e1")->second.getPiece().setType('k');
  chessBoard.find("e1")->second.getPiece().setWhite(true);

  chessBoard.find("e8")->second.getPiece().setType('k');
  chessBoard.find("e8")->second.getPiece().setWhite(false);

  chessBoard.find("e4")->second.getPiece().setType('p');
  chessBoard.find("e4")->second.getPiece().setWhite(false);

  chessBoard.find("f2")->second.getPiece().setType('p');
  chessBoard.find("f2")->second.getPiece().setWhite(true);

  chessBoard.find("e2")->second.getPiece().setType('r');
  chessBoard.find("e2")->second.getPiece().setWhite(true);
*/
}
