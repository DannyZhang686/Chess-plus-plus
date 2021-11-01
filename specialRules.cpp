// Special Chess rules (en passant, castling, promotion)

#include <iostream>
#include "include/main.hpp"
using namespace std;

// Castling
bool castlingCheck(string initialSquare, string finalSquare, Cell initialCell, Cell finalCell) {
  // This string array represents the squares that cannot
  // be attacked if a king is to castle
  // [0][n] is white kingside
  // [1][n] is white queenside
  // [2][n] is black kingside
  // [3][n] is black queenside
  string kingSquares[4][2] = {{"f1", "g1"}, {"d1", "c1"}, {"f8", "g8"}, {"d8", "c8"}};
  Piece king, rook;

  // For white castling, a white king HAS to be on e1 
  if ((initialSquare == "e1") && (initialCell.getPiece().getWhite()) && (initialCell.getPiece().getType() == 'k')) {
    // If the white king castles kingside, it ends up on g1
    if (finalSquare == "g1") { 
      king = initialCell.getPiece();
      // kingside castling uses the rook on h1
      rook = chessBoard.find("h1")->second.getPiece();
      // Ensure that the rook is also white
      if (!rook.getWhite()) {
        return false;
      }
      // Ensure that the piece on h1 is actually a rook
      if (rook.getType() != 'r') {
        return false;
      }
      // Make sure all squares between king and rook are empty
      if ((chessBoard.find("f1")->second.getPiece().getType() != 'x') || (chessBoard.find("g1")->second.getPiece().getType() != 'x')) {
        return false;
      }
      // Can't castle if king or rook have moved
      if ((king.getHasMoved()) || (rook.getHasMoved())) {
        return false;
      } 
      // Can't castle if in check
      if (Game::getWhiteInCheck()) {
        return false;
      }
      for (int i = 0; i < 2; i++) { // Moves king over square by square, checking if there is check
        // Bring king to kingSquares[0][i] (white kingside castle)
        chessBoard.find(kingSquares[0][i])->second.setPiece(initialCell.getPiece()); // Move piece to new cell
        Piece blankPiece = Piece(); // Replace moved piece with empty square
        // Clear the square where the king used to be to avoid having multiple kings on the board
        if (i == 0) { 
          chessBoard.find("e1")->second.setPiece(blankPiece);
        }
        else {
          chessBoard.find("f1")->second.setPiece(blankPiece);
        }
        Game::updateState(); // Update check
        if (Game::getWhiteInCheck()) { // Square is attacked, so can't castle
          chessBoard.find("e1")->second.setPiece(chessBoard.find(kingSquares[0][i])->second.getPiece());
          Piece blankPiece = Piece(); // Replace king with empty square
          chessBoard.find(kingSquares[0][i])->second.setPiece(blankPiece);
          Game::updateState(); // Re-update state, king no longer in check
          return false;
        }
      }
      // King is now on g1 and castling is valid since we made it through the loop
      // Must move rook from h1 to f1
      chessBoard.find("f1")->second.setPiece(chessBoard.find("h1")->second.getPiece());
      Piece blankPiece = Piece(); // Replace moved piece with empty square
      chessBoard.find("h1")->second.setPiece(blankPiece);
      return true;
    }
    else if (finalSquare == "c1") { // White queenside castle
      king = initialCell.getPiece();
      rook = chessBoard.find("a1")->second.getPiece();
      // Ensure that the rook is white
      if (!rook.getWhite()) {
        return false;
      }
      // Ensure that the piece on a1 is a rook
      if (rook.getType() != 'r') {
        return false;
        }
      // Make sure all squares between king and rook are empty
      if ((chessBoard.find("b1")->second.getPiece().getType() != 'x') || (chessBoard.find("c1")->second.getPiece().getType() != 'x') || (chessBoard.find("d1")->second.getPiece().getType() != 'x')) {
        return false;
      }
      // Can't castle if king or rook have moved
      if ((king.getHasMoved()) || (rook.getHasMoved())) {
        return false;
      } 
      if (Game::getWhiteInCheck()) {
        return false;
      }
      for (int i = 0; i < 2; i++) { // Moves king over square by square, checking if check
        // Bring king to kingSquares[1][i] (white queenside castle)
        chessBoard.find(kingSquares[1][i])->second.setPiece(initialCell.getPiece()); // Move piece to new cell
        Piece blankPiece = Piece(); // Replace moved piece with empty square
        if (i == 0) {
          chessBoard.find("e1")->second.setPiece(blankPiece);
        }
        else {
          chessBoard.find("d1")->second.setPiece(blankPiece);
        }
        Game::updateState(); // Update state and see if check
        if (Game::getWhiteInCheck()) {
          // Square is attacked, so can't castle
          chessBoard.find("e1")->second.setPiece(chessBoard.find(kingSquares[1][i])->second.getPiece());
          Piece blankPiece = Piece(); // Replace king with empty square
          chessBoard.find(kingSquares[1][i])->second.setPiece(blankPiece);
          Game::updateState(); // Re-update state, king no longer in check
          return false;
        }
      }
      // King is now on c1 and castling is valid since we made it through the loop
      // Must move rook from a1 to d1
      chessBoard.find("d1")->second.setPiece(chessBoard.find("a1")->second.getPiece());
      Piece blankPiece = Piece(); // replace moved piece with empty square
      chessBoard.find("a1")->second.setPiece(blankPiece);
      return true;
    }
  }
  // black castling (king on e8), same logic as before
  else if ((initialSquare == "e8") && (!initialCell.getPiece().getWhite()) && (initialCell.getPiece().getType() == 'k')) {
    if (finalSquare == "g8") { // Kingside castling
      king = initialCell.getPiece();
      rook = chessBoard.find("h8")->second.getPiece();
      // Ensure that the rook is black (king starting on e8)
      if (rook.getWhite()) {
        return false;
      }
      // Ensure that h8 is a rook
      if (rook.getType() != 'r') {
        return false;
      }
      // Make sure all squares between king and rook are empty
      if ((chessBoard.find("f8")->second.getPiece().getType() != 'x') || (chessBoard.find("g8")->second.getPiece().getType() != 'x')) {
        return false;
      }
      // Can't castle if king or rook have moved
      if ((king.getHasMoved()) || (rook.getHasMoved())) {
        return false;
      } 
      if (Game::getBlackInCheck()) {
        return false;
      }
      for (int i = 0; i < 2; i++) { // Moves king over square by square, checking if check
        // Bring king to kingSquares[2][i] (black kingside castle)
        chessBoard.find(kingSquares[2][i])->second.setPiece(initialCell.getPiece()); // Move piece to new cell
        Piece blankPiece = Piece(); // Replace moved piece with empty square
        if (i == 0) {
          chessBoard.find("e8")->second.setPiece(blankPiece);
        }
        else {
          chessBoard.find("f8")->second.setPiece(blankPiece);
        }
        Game::updateState();
        if (Game::getBlackInCheck()) {
          // Square is attacked, so can't castle
          chessBoard.find("e8")->second.setPiece(chessBoard.find(kingSquares[2][i])->second.getPiece());
          Piece blankPiece = Piece(); // Replace king with empty square
          chessBoard.find(kingSquares[2][i])->second.setPiece(blankPiece);
          Game::updateState(); // Re-update state, king no longer in check
          return false;
        }
      }
      // King is now on g8 and castling is valid since we made it through the loop
      // Must move rook from h8 to f8
      chessBoard.find("f8")->second.setPiece(chessBoard.find("h8")->second.getPiece());
      Piece blankPiece = Piece(); // Replace moved piece with empty square
      chessBoard.find("h8")->second.setPiece(blankPiece);
      return true;
    } 
    else if (finalSquare == "c8") { // Queenside castle
      king = initialCell.getPiece();
      rook = chessBoard.find("a8")->second.getPiece();
      // Ensure that the rook is black (king starting on e8)
      if (rook.getWhite()) {
        return false;
      } 
      // Castling with a rook
      if (rook.getType() != 'r') {
        return false;
      }
      // Make sure all squares between king and rook are empty
      if ((chessBoard.find("b8")->second.getPiece().getType() != 'x') || (chessBoard.find("c8")->second.getPiece().getType() != 'x') || (chessBoard.find("d8")->second.getPiece().getType() != 'x')) {
        return false;
      }
      // Can't castle if king or rook have moved
      if ((king.getHasMoved()) || (rook.getHasMoved())) {
        return false;
      } 
      if (Game::getBlackInCheck()) {
        return false;
      }
      for (int i = 0; i < 2; i++) { // moves king over square by square, checking if check
        chessBoard.find(kingSquares[3][i])->second.setPiece(initialCell.getPiece()); // Move piece to new cell
        Piece blankPiece = Piece(); // replace moved piece with empty square          
        if (i == 0) {
          chessBoard.find("e8")->second.setPiece(blankPiece);
        }
        else {
          chessBoard.find("d8")->second.setPiece(blankPiece);
        }
        Game::updateState(); // check check
        if (Game::getBlackInCheck()) {
          // Square is attacked, so can't castle
          chessBoard.find("e8")->second.setPiece(chessBoard.find(kingSquares[3][i])->second.getPiece());
          Piece blankPiece = Piece(); // replace king with empty square
          chessBoard.find(kingSquares[3][i])->second.setPiece(blankPiece);
          Game::updateState(); // re-update state, king no longer in check
          return false;
        }
      }
      // King is now on c8 and castling is valid since we made it through the loop
      // Must move rook from a8 to d8
      chessBoard.find("d8")->second.setPiece(chessBoard.find("a8")->second.getPiece());
      Piece blankPiece = Piece(); // replace moved piece with empty square
      chessBoard.find("a8")->second.setPiece(blankPiece);
      return true;
    }
  }
  return false;
}

void ePCountDecrease() {
  // Decrease the EP count of all pawns, down to a minimum of 0
  for (char i = 'a'; i <= 'h'; i++) {
    for (int j = 1; j <= 8; j++) {
      string tempSquare(1, i);
      tempSquare += to_string(j);

      // For pieces, the following line of code keeps the
      // number at 0; for pawns, it decreases the EP count
      // by 1 to a minimum of 0.
      chessBoard.find(tempSquare)->second.getPiece().setEPCountDown(max(chessBoard.find(tempSquare)->second.getPiece().getEPCountDown() - 1, 0));
    }
  }
}

int enPassantCheck(string finalSquare, Cell initialCell, int finalRank) {
  // Check for en passant
  if (initialCell.getPiece().getWhite() == true) {
    // White pawn trying to do en passant
    if (finalRank == 6) {
      string enPassantSquare = finalSquare;
      enPassantSquare[1] -= 1; // Check the piece one rank down
      Piece tempPiece = chessBoard.find(enPassantSquare)->second.getPiece();
      if ((tempPiece.getType() == 'p') && (tempPiece.getWhite() == false)) {
        // Must be an opposite-coloured pawn
        if (tempPiece.getEPCountDown() > 0) {
          // The en passant move is valid
          return 4;
        }
      }
    }
  }
  else {
    // Black pawn trying to do en passant
    if (finalRank == 3) {
      string enPassantSquare = finalSquare;
      enPassantSquare[1] += 1; // Check the piece one rank up
      Piece tempPiece = chessBoard.find(enPassantSquare)->second.getPiece();
      if ((tempPiece.getType() == 'p') && (tempPiece.getWhite() == true)) {
        // Must be an opposite-coloured pawn
        if (tempPiece.getEPCountDown() > 0) {
          // The en passant move is valid
          return 4;
        }
      }
    }
  }
  return 0; // Invalid by default
}

// Pawn promotion
// Pawns promote when they cross the entire board
void pawnPromotion(Game& game) { // Passing reference game so we can edit it (move list)
  for (char i = 'a'; i <= 'h'; i++) { // loop through the files
    Piece p;
    string input;
    bool validPromotion = false;
    char choice;
    // First rank will be the letter from the for loop + 1 (e.g. a1, b1)
    string firstRank(1, i);
    firstRank += to_string(1);
    // See if there is a pawn on the first rank 
    p = chessBoard.find(firstRank)->second.getPiece();
    
    if (p.getType() == 'p') { // If there is,
      if (p.getWhite() == false) { // and it is a black pawn, 
        while (validPromotion == false) { // black can promote
          validPromotion = true;
          cout << "What would you like to promote your pawn to?\n(n = knight, b = bishop, r = rook, q = queen): ";
          cin >> input; // As a string
          if (input.length() != 1) {
            cout << "Invalid input length." << endl;
            validPromotion = false;
            continue;
          }
          choice = input[0]; // Go to char
          switch(choice) { // Setting to their choice
            case 'n': {
              chessBoard.find(firstRank)->second.getPiece().setType('n');
              // Using editMove, we append the notation for promotion (this is why we pass game by reference)
              game.editMove(game.getMoves()[game.getMoves().size() - 1] + "=♞ ", game.getMoves().size() - 1);
              break;
            }
            case 'b': {
              chessBoard.find(firstRank)->second.getPiece().setType('b');
              game.editMove(game.getMoves()[game.getMoves().size() - 1] + "=♝ ", game.getMoves().size() - 1);
              break;
            }
            case 'r': {
              chessBoard.find(firstRank)->second.getPiece().setType('r');
              game.editMove(game.getMoves()[game.getMoves().size() - 1] + "=♜ ", game.getMoves().size() - 1);
              break;
            }
            case 'q': {
              chessBoard.find(firstRank)->second.getPiece().setType('q');
              game.editMove(game.getMoves()[game.getMoves().size() - 1] + "=♛ ", game.getMoves().size() - 1);
              break;
            }
            default: {
              cout << "Invalid promotion choice." << endl;
              validPromotion = false;
              break;
            }
          }
        }  
      }
    }
    // Same thing for white pawns, except they're on the 8th rank
    string eighthRank(1, i);
    eighthRank += to_string(8);
    p = chessBoard.find(eighthRank)->second.getPiece();
    if (p.getType() == 'p') {
      if (p.getWhite() == true) {
        while (validPromotion == false) {
          validPromotion = true;
          cout << "What would you like to promote your pawn to?\n(n = knight, b = bishop, r = rook, q = queen): ";
          cin >> input;
          if (input.length() != 1) {
            cout << "Invalid input length." << endl;
            validPromotion = false;
            continue;
          }
          choice = input[0];
          switch(choice) {
            case 'n': {
              chessBoard.find(eighthRank)->second.getPiece().setType('n');
              game.editMove(game.getMoves()[game.getMoves().size() - 1] + "=♞ ", game.getMoves().size() - 1);
              break;
            }
            case 'b': {
              chessBoard.find(eighthRank)->second.getPiece().setType('b');
              game.editMove(game.getMoves()[game.getMoves().size() - 1] + "=♝ ", game.getMoves().size() - 1);
              break;
            }
            case 'r': {
              chessBoard.find(eighthRank)->second.getPiece().setType('r');
              game.editMove(game.getMoves()[game.getMoves().size() - 1] + "=♜ ", game.getMoves().size() - 1);
              break;
            }
            case 'q': {
              chessBoard.find(eighthRank)->second.getPiece().setType('q');
              game.editMove(game.getMoves()[game.getMoves().size() - 1] + "=♛ ", game.getMoves().size() - 1);
              break;
            }
            default: {
              cout << "Invalid promotion choice." << endl;
              validPromotion = false;
              break;
            }
          }
        } 
      }
    }
  }
}