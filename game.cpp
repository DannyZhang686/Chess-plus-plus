// General Game functions (turns, players)

#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <locale> 
#include "include/main.hpp"
using namespace std;

//////////////////////
//                  //
//   class Player   //
//                  //
//////////////////////

// Constructors
Player::Player(string n, bool w) {
  name = n;
  white = w;
}
Player::Player() {
  name = "Kasparov";
  white = true;
}
// Accessors
string Player::getName() {
  return name;
}
bool Player::getWhite() {
  return white;
} 
// Mutators
void Player::setName(string n) {
  name = n;
}
void Player::setWhite(bool w) {
  white = w;
}

/////////////////////
//                 //
//   class Board   //
//                 //
/////////////////////

// Constructor
Board::Board() {
  Piece tempPiece; // temporary piece for initialization of cell
  vector<Cell> newRow = {}; // making rows
  board.push_back(newRow); // 0 index
  for (int i = 1; i <= 8; i++) { // 1 to 8 to make ranks easier
    board.push_back(newRow); // add the rank
    for (int j = 1; j <= 8; j++) { // loop through files a-h
      Cell newCell(j, i, tempPiece); // make a piece with the current file/rank
      board[i].push_back(newCell); // add to board
    }
  }
}

// Accessor
Cell Board::getCell(int rank, int file) {
  return board[file][rank]; // -1 to keep 1-8 in the rest of the code
}

//////////////////////
//                  //
//    class Game    //
//                  //
//////////////////////

// Static variables initialization
bool Game::whiteInCheck = false;
bool Game::blackInCheck = false;
bool Game::whiteInCheckmate = false;
bool Game::blackInCheckmate = false;
bool Game::whiteInStalemate = false;
bool Game::blackInStalemate = false;

Game::Game() {} // There are no variables to initialize :)
// Accessors
Player Game::getPlayer(int input) {
  if (input == 1) {
    return Player1;
  } 
  else {
    return Player2;
  }
}
Board Game::getBoard() {
  return board;
}
vector<string> Game::getMoves() {
  return moves;
}
bool Game::getWhiteInCheck() {
  return whiteInCheck;
}
bool Game::getBlackInCheck() {
  return blackInCheck;
}
bool Game::getWhiteInCheckmate() {
  return whiteInCheckmate;
}
bool Game::getBlackInCheckmate() {
  return blackInCheckmate;
}
bool Game::getWhiteInStalemate() {
  return whiteInStalemate;
}
bool Game::getBlackInStalemate() {
  return blackInStalemate;
}

// Mutators
void Game::setNames() {
  string tempName;
  cout << "Enter White's name: ";
  cin >> tempName;
  Player1.setName(tempName.substr(0, 20)); // Take the first 20 characters of the name
  cout << "\nEnter Black's name: ";
  cin >> tempName;
  if (tempName == Player1.getName()) { // if same name add (1)
    tempName = tempName.substr(0, 20) + " (1)";
  }
  cout << endl;
  Player2.setName(tempName);
}
void Game::swapNames() { // Name swap is called when players switch colors for a rematch
  string tempName = Player1.getName();
  Player1.setName(Player2.getName());
  Player2.setName(tempName);
}
void Game::setMoves(string input) {
  moves.push_back(input);
}
void Game::editMove(string input, int index) {
  moves[index] = input;
}
void Game::clearMoves() { // Empty move list
  moves.clear();
}
void Game::setWhiteInCheck(bool input) {
  whiteInCheck = input;
}
void Game::setBlackInCheck(bool input) {
  blackInCheck = input;
}
void Game::setWhiteInCheckmate(bool input) {
  whiteInCheckmate = input;
}
void Game::setBlackInCheckmate(bool input) {
  blackInCheckmate = input;
}
void Game::setWhiteInStalemate(bool input) {
  whiteInStalemate = input;
}
void Game::setBlackInStalemate(bool input) {
  blackInStalemate = input;
}

///////////////////////////
//                       //
//    Turn processors    //
//                       //
///////////////////////////

// White's turn
int Game::processTurn1() { 
  string p1Initial, p1Final; // Squares the user enters
  string outputPiece; // String which holds the chess piece character
  string tempMove; // Temporary string to be pushed into the move list
  int validInput = 0; // Whether the input is a legal chess move
  int validInputPawn = 0; // Whether the input is a valid pawn move
  bool skipInput = false; // Skip second input if player enters castling (O-O)

   updateState(); // Update the game state, including checkmates, stalemates, etc.
  
  while (validInput == 0) { // Loop while input is invalid
    skipInput = false;
    validInput = 1; // Set to true, and turn false if something goes wrong
    cout << getPlayer(1).getName() << "'s turn. Enter the square on which the piece you wish to move is located: ";
    cin >> p1Initial;
    if (p1Initial == "resign" || p1Initial == "Resign") return 1; // Return values for switch case in main
    // Special notation for castling
    if ((p1Initial == "O-O") || (p1Initial == "0-0") || (p1Initial == "o-o")) {
      p1Initial = "e1"; // Set initial / final squares
      p1Final = "g1";
      skipInput = true; // Skip that next input
    }
    if ((p1Initial == "O-O-O") || (p1Initial == "0-0-0") || (p1Initial == "o-o-o")) {
      p1Initial = "e1"; // Set initial / final squares
      p1Final = "c1";
      skipInput = true; // Skip that next input
    }
    if ((p1Initial == "draw") || (p1Initial == "Draw")) {
      string input;
      cout << getPlayer(1).getName() << " has requested a draw. Accept? Y/n: ";
      cin >> input;
      if ((input.substr(0, 1) == "Y") || (input.substr(0, 1) == "y")) {
        return 2;
      } 
      else {
        cout << "Draw declined." << endl;
        validInput = 0;
        continue;
      }
    }
    if (skipInput == false) {
      cout <<"\nEnter the square where you want to move this piece: ";
      cin >> p1Final;
    }
    if ((p1Initial.length() != 2) || (p1Final.length() != 2)) {
      cout << "Invalid input length. Please try again.\n";
      validInput = 0;
      continue;
    }
    for (int i = 0; i < p1Initial.length(); i++) { // Breaking down the string into a character and an integer
      p1Initial[i] = tolower(p1Initial[i]);
      p1Final[i] = tolower(p1Final[i]);
    }
    int curFile = (int) (p1Initial[0] - 96); // ASCII codes
    try {
      Cell selectedCell = chessBoard.find(p1Initial)->second;
      Piece curPiece = selectedCell.getPiece();
      if (curPiece.getWhite() == false) { // White isn't allowed to play black's pieces
        validInput = 0;
      }
      else {
        switch (curPiece.getType()) { // If it's a valid move, call the appropriate validMove method
          case 'p': {
            Pawn curPawn = convertToPawn(curPiece); // Convert the piece to a pawn
            validInputPawn = curPawn.validMove(p1Initial, p1Final); // Find whether the pawn move was valid
            if (validInputPawn == 0) { // Invalid pawn move
              validInput = 0;
            }
            outputPiece = "♟";
            break;
          }
          case 'n': {
            Knight curKnight = convertToKnight(curPiece);
            validInput = curKnight.validMove(p1Initial, p1Final);
            outputPiece = "♞";
            break;  
          }
          case 'b': {
            Bishop curBishop = convertToBishop(curPiece);
            validInput = curBishop.validMove(p1Initial, p1Final);
            outputPiece = "♝";
            break;
          }
          case 'r': {
            Rook curRook = convertToRook(curPiece);
            validInput = curRook.validMove(p1Initial, p1Final);
            outputPiece = "♜";
            break;
          }  
          case 'q': {
            Queen curQueen = convertToQueen(curPiece);
            validInput = curQueen.validMove(p1Initial, p1Final);
            outputPiece = "♛";
            break;
          }
          case 'k': {
            King curKing = convertToKing(curPiece);
            validInput = curKing.validMove(p1Initial, p1Final);
            outputPiece = "♚";
            break;
          }
          default: { // Player didn't move any of the previous pieces
            cout << "You didn't move a piece.\n"; // Invalid by default
            validInput = 0;
            continue;
          }
        }
      }
      if (validInput != 0 || validInputPawn != 0) {
        // Moving the piece // Create new cell
        // Storing these values in case we need to restore them
        bool moveFlag = chessBoard.find(p1Initial)->second.getPiece().getHasMoved(); 
        int enPassantFlag = chessBoard.find(p1Initial)->second.getPiece().getEPCountDown();
        if (validInputPawn == 4) { // If en passant
          p1Final[1] -= 1; // We take the piece on the square below where our pawn ends up
        }
        Piece tempPiece = chessBoard.find(p1Final)->second.getPiece(); // Store piece in case we need to restore
        if (validInputPawn == 4) { // Piece is obtained, we can reset the coordinate
          p1Final[1] += 1;
        }
        chessBoard.find(p1Final)->second.setPiece(selectedCell.getPiece()); // Move piece to new cell
        chessBoard.find(p1Final)->second.getPiece().setHasMoved(true); // Moving a piece sets hasMoved to true
        if (validInputPawn == 2) { // Moving pawn 2 squares, available for en passant
          chessBoard.find(p1Final)->second.getPiece().setEPCountDown(2);
        }
        Piece blankPiece = Piece(); // Replace moved piece with empty square
        if (validInputPawn == 4) { // If it was en passant, empty square below pawn
          p1Final[1] -= 1;
          chessBoard.find(p1Final)->second.setPiece(blankPiece);
          p1Final[1] += 1;
        }
        chessBoard.find(p1Initial)->second.setPiece(blankPiece); // Also empty where white's pawn used to be
        updateState(); // Update game state
        if (getWhiteInCheck() == true) { // Illegal move, as it put white in check
          // We already moved the piece so that we could updateState, so put it back
          cout << "Illegal move, because it puts white in check." << endl;
          chessBoard.find(p1Initial)->second.setPiece(chessBoard.find(p1Final)->second.getPiece()); // Put the piece back to where it was
          chessBoard.find(p1Initial)->second.getPiece().setHasMoved(moveFlag); // Set everything back to what it was
          chessBoard.find(p1Initial)->second.getPiece().setEPCountDown(enPassantFlag); // Reset the en passant flag to what it was before
          if (validInputPawn == 4) {
            // Put back the original piece, which is on a different square for en passant
            p1Final[1] -= 1;
            chessBoard.find(p1Final)->second.setPiece(tempPiece);
            p1Final[1] += 1;
            chessBoard.find(p1Final)->second.setPiece(blankPiece);
          }
          else {
            chessBoard.find(p1Final)->second.setPiece(tempPiece); // Put the piece back
          }
          validInput = 0;
          updateState(); // Re-update after putting piece back
        }
      }
      else {
        cout << "Invalid input.\n";
        validInput = 0;
        continue;
      }
    } catch (...) {
        cout << "Unexpected error.\n";
        validInput = 0;
    }
  }
  // Special notation for Castling
  if (outputPiece == "♚" && p1Initial == "e1") {
    if (p1Final == "g1") { // If kingside castling
      tempMove = "O-O";
    }
    else if (p1Final == "c1") {
      tempMove = "O-O-O";
    }
    else {
      tempMove = outputPiece + " " + p1Initial + "-" + p1Final;
    }
  } 
  // Otherwise, if there is no castling and no capture, add the move to the list
  else if ((validInput != 2) && (validInputPawn < 3)) { 
    tempMove = outputPiece + " " + p1Initial + "-" + p1Final;
  }
  // If we make it here, there is a capture which has special notation
  else { 
    tempMove = outputPiece + " " + p1Initial + "x" + p1Final;
  }
  setMoves(tempMove);
  return 0; // Move on as usual
}

// This is the code for the black pieces, which is identical in most ways to that for the white pieces; it is therefore commented in less detail.
int Game::processTurn2() { 
  string p2Initial, p2Final;
  string outputPiece;
  string tempMove;
  int validInput = 0;
  int validInputPawn = 0;
  bool skipInput;
  updateState();
  while (validInput == 0) { // Loop while input is invalid
    skipInput = false;
    validInput = 1; // Set to true, and turn false if something goes wrong
    cout << getPlayer(2).getName() << "'s turn. Enter the square on which the piece you wish to move is located: ";
    cin >> p2Initial;
    if ((p2Initial == "resign") || (p2Initial == "Resign")) return 1;
    if ((p2Initial == "O-O") || (p2Initial == "0-0") || (p2Initial == "o-o")) {
      p2Initial = "e8"; // Set initial / final squares
      p2Final = "g8";
      skipInput = true; // skip that next input
    }
    if ((p2Initial == "O-O-O") || (p2Initial == "0-0-0") || (p2Initial == "o-o-o")) {
      p2Initial = "e8"; // Set initial / final squares
      p2Final = "c8";
      skipInput = true; // skip that next input
    }
    if ((p2Initial == "draw") || (p2Initial == "Draw")) {
      string input;
      cout << getPlayer(2).getName() << " has requested a draw. Accept? Y/n: ";
      cin >> input;
      if ((input.substr(0, 1) == "Y") || (input.substr(0, 1) == "y")) {
        return 2;
      } 
      else {
        cout << "Draw declined." << endl;
        validInput = 0;
        continue;
      }
    }
    if (skipInput == false) {
      cout << "\nEnter the square where you want to move this piece: ";
      cin >> p2Final;
    }
    
    if ((p2Initial.length() != 2) || (p2Final.length() != 2)) {
      cout << "Invalid input length. Please try again.\n";
      validInput = 0;
      continue;
    }
    for (int i = 0; i < p2Initial.length(); i++) {
      p2Initial[i] = tolower(p2Initial[i]);
      p2Final[i] = tolower(p2Final[i]);
    }
    try {
      Cell selectedCell = chessBoard.find(p2Initial)->second;
      Piece curPiece = selectedCell.getPiece(); // Get the piece that 
      if (curPiece.getWhite() == true) { // Black playing white's pieces
        validInput = 0;
      }
      else {
        switch (curPiece.getType()) {
          case 'p': {
            Pawn curPawn = convertToPawn(curPiece);
            validInputPawn = curPawn.validMove(p2Initial, p2Final);
            if (validInputPawn == 0) { // If invalid pawn move
              validInput = 0;
            }
            outputPiece = "♟";
            break;
          }
          case 'n': {
            Knight curKnight = convertToKnight(curPiece);
            validInput = curKnight.validMove(p2Initial, p2Final);
            outputPiece = "♞";
            break;  
          }
          case 'b': {
            Bishop curBishop = convertToBishop(curPiece);
            validInput = curBishop.validMove(p2Initial, p2Final);
            outputPiece = "♝";
            break;
          }
          case 'r': {
            Rook curRook = convertToRook(curPiece);
            validInput = curRook.validMove(p2Initial, p2Final);
            outputPiece = "♜";
            break;
          }  
          case 'q': {
            Queen curQueen = convertToQueen(curPiece);
            validInput = curQueen.validMove(p2Initial, p2Final);
            outputPiece = "♛";
            break;
          }
          case 'k': {
            King curKing = convertToKing(curPiece);
            validInput = curKing.validMove(p2Initial, p2Final);
            outputPiece = "♚";
            break; 
          }
          default: { // Player didn't move any of the previous pieces
            cout << "Invalid Move.\n"; // Invalid by default
            validInput = 0;
          }
        }
      }
      if (validInput != 0 || validInputPawn != 0) {
        // Moving the piece // Create new cell
        bool moveFlag = chessBoard.find(p2Initial)->second.getPiece().getHasMoved();
        int enPassantFlag = chessBoard.find(p2Initial)->second.getPiece().getEPCountDown();
        if (validInputPawn == 4) {
          p2Final[1] += 1;
        }
        Piece tempPiece = chessBoard.find(p2Final)->second.getPiece(); // Store the piece in case the move is illegal
        if (validInputPawn == 4) {
          p2Final[1] -= 1;
        }
        chessBoard.find(p2Final)->second.setPiece(selectedCell.getPiece()); // Move piece to new cell
        chessBoard.find(p2Final)->second.getPiece().setHasMoved(true);
        if (validInputPawn == 2) {
          chessBoard.find(p2Final)->second.getPiece().setEPCountDown(2);
        } 
        Piece blankPiece = Piece(); // replace moved piece with empty square
        if (validInputPawn == 4) {
          p2Final[1] += 1;
          chessBoard.find(p2Final)->second.setPiece(blankPiece);
          p2Final[1] -= 1;
        }
        chessBoard.find(p2Initial)->second.setPiece(blankPiece);
        updateState();
        //selectedCell.setPiece(blankPiece); // Delete the piece from initial cell
        if (getBlackInCheck() == true) { // Illegal move, as it put black in check
          cout << "Illegal move, puts black in check." << endl;

          chessBoard.find(p2Initial)->second.setPiece(chessBoard.find(p2Final)->second.getPiece()); // put piece back
          chessBoard.find(p2Initial)->second.getPiece().setHasMoved(moveFlag); 
          chessBoard.find(p2Initial)->second.getPiece().setEPCountDown(enPassantFlag);
          if (validInputPawn == 4) {
            // Put back the original piece
            p2Final[1] += 1;
            chessBoard.find(p2Final)->second.setPiece(tempPiece);
            p2Final[1] -= 1;
            chessBoard.find(p2Final)->second.setPiece(blankPiece);
          }
          else {
            chessBoard.find(p2Final)->second.setPiece(tempPiece);
          }
          validInput = 0;
          updateState(); // re-update
        }
      }
      else {
        cout << "Invalid move, please try again.\n";
        validInput = 0;
        continue;
      }
    }
    catch (...) {
      cout << "Invalid move, please try again.\n";
      validInput = 0;
    }
  }
  
  // Special notation for Castling
  if (outputPiece == "♚" && p2Initial == "e8") {
    if (p2Final == "g8") { // If kingside castling
      tempMove = "O-O";
    }
    else if (p2Final == "c8") {
      tempMove = "O-O-O";
    }
    else {
      tempMove = outputPiece + " " + p2Initial + "-" + p2Final;
    }
  } 
  // Otherwise, if there is no castling and no capture, add the move to the list
  else if ((validInput != 2) && (validInputPawn < 3)) { 
    tempMove = outputPiece + " " + p2Initial + "-" + p2Final;
  }
  // If we make it here, there is a capture which has special notation
  else { 
    tempMove = outputPiece + " " + p2Initial + "x" + p2Final;
  }
  setMoves(tempMove);
  return 0;
}
// Refresh state of kings and game
// Updates static variables which indicate check, checkmate, and stalemate
void Game::updateState() { 
  string blackKingSquare, whiteKingSquare; // Where the kings are
  bool whiteFlag, blackFlag; // Flags to reset if there is no longer check
  // Find the kings
  for (char i = 'a'; i <= 'h'; i++) { // Looping through all squares
    for (int j = 1; j <= 8; j++) {
      string tempSquare(1, i);
      tempSquare += to_string(j);
      Piece curPiece = chessBoard.find(tempSquare)->second.getPiece();
      if (curPiece.getType() == 'k') { // Once we find a king, store the square
        if (curPiece.getWhite() == true) {
          whiteKingSquare = tempSquare;
        }
        else {
          blackKingSquare = tempSquare;
        }
      }
    }
  }
  // This loop checks for every piece and determines if 
  // it is attacking the opposite king
  for (char i = 'a'; i <= 'h'; i++) {
    for (int j = 1; j <= 8; j++) {
      string tempSquare(1, i);
      tempSquare += to_string(j);
      Piece curPiece = chessBoard.find(tempSquare)->second.getPiece();
      // Get piece color, should look for king of opposite color
      bool curPieceColor = curPiece.getWhite(); 
      switch(curPiece.getType()) {
        case 'p': { 
          // Pawns are special, as checks have to be a capture (not just a valid move)
          // For example, a king can safely sit in front of an enemy pawn
          Pawn tempPawn = convertToPawn(curPiece);
          if (curPieceColor == true) { // White pawns
            if (tempPawn.validMove(tempSquare, blackKingSquare) == 3) { // If the pawn can CAPTURE (not just move)
              setBlackInCheck(true); // Update static variable
              blackFlag = true;
            }
          }
          else { // Black pawns, same logic
            if (tempPawn.validMove(tempSquare, whiteKingSquare) == 3) {
              setWhiteInCheck(true);
              whiteFlag = true;
            }
          }
          break;
        }
        // For every other piece (excluding the kings), 
        // a check is if the position of the enemy king is just a valid move
        case 'n': { 
          Knight tempKnight = convertToKnight(curPiece);
          if (curPieceColor == true) { // White
            if (tempKnight.validMove(tempSquare, blackKingSquare) != 0) {
              setBlackInCheck(true);
              blackFlag = true;
            }
          }
          else { // Black, same logic
            if (tempKnight.validMove(tempSquare, whiteKingSquare) != 0) {
              setWhiteInCheck(true);
              whiteFlag = true;
            }
          }
          break;
        }
        case 'b': {
          Bishop tempBishop = convertToBishop(curPiece);
          if (curPieceColor == true) {
            if (tempBishop.validMove(tempSquare, blackKingSquare) != 0) {
              setBlackInCheck(true);
              blackFlag = true;
            }
          } 
          else {
            if (tempBishop.validMove(tempSquare, whiteKingSquare) != 0) {
              setWhiteInCheck(true);
              whiteFlag = true;
            }
          }
          break;
        }
        case 'r': {
          Rook tempRook = convertToRook(curPiece);
          if (curPieceColor == true) {
            if (tempRook.validMove(tempSquare, blackKingSquare) != 0) {
              setBlackInCheck(true);
              blackFlag = true;
            }
          } 
          else {
            if (tempRook.validMove(tempSquare, whiteKingSquare) != 0) {
              setWhiteInCheck(true);
              whiteFlag = true;
            }
          }
          break;
        }  
        case 'q': {
          Queen tempQueen = convertToQueen(curPiece);
          if (curPieceColor == true) {
            if (tempQueen.validMove(tempSquare, blackKingSquare) != 0) {
              setBlackInCheck(true);
              blackFlag = true;
            }
          } 
          else {
            if (tempQueen.validMove(tempSquare, whiteKingSquare) != 0) {
              setWhiteInCheck(true);
              whiteFlag = true;
            }
          }
          break;
        }
        default: { // No checks
          break;
        }
      }
    }
  }
  // If a flag wasn't triggered, then there was no check
  // so, set the appropriate player's check variable to false
  if (!whiteFlag) setWhiteInCheck(false); // reset flags
  if (!blackFlag) setBlackInCheck(false);
}

// Stalemate and checkmate
// Checks if a given side has any valid moves, and if they are in check
  // no moves + check = checkmate, no moves + no check = stalemate
// Takes only one color as input, because white can't be in checkmate after white's move
// This reduces the number of pieces we check
void Game::mateCheck(bool colour) { 
  bool hasValidMoves = false; // Tracks if the player has any valid moves at all
  // This first set of loops finds all of the pieces
  for (char i = 'a'; i <= 'h'; i++) {
    for (int j = 1; j <= 8; j++) {
      string tempSquare1(1, i);
      tempSquare1 += to_string(j);
      Piece tempPiece = chessBoard.find(tempSquare1)->second.getPiece();
      // Ignoring empty squares and only checking a certain player's pieces
      if (tempPiece.getWhite() == colour && tempPiece.getType() != 'x') {
        // This second loop goes through all other squares and finds
        // If the pieces from the first loop can move there
        for (char m = 'a'; m <= 'h'; m++) {
          for (int n = 1; n <= 8; n++) {
            string tempSquare2(1, m);
            tempSquare2 += to_string(n);
            Piece curPiece = tempPiece; // Copy the piece to avoid reference issues
            // The switch looks for a valid move and breaks once it finds one
            switch(curPiece.getType()) {
              case 'p': {
                Pawn curPawn = convertToPawn(curPiece);
                if (curPawn.validMove(tempSquare1, tempSquare2) != 0) {
                  hasValidMoves = true; // setting this flag to true
                }
                break;
              }
              case 'n': {
                Knight curKnight = convertToKnight(curPiece);
                if (curKnight.validMove(tempSquare1, tempSquare2) != 0) {
                  hasValidMoves = true;
                }
                break;
              }
              case 'b': {
                Bishop curBishop = convertToBishop(curPiece);
                if (curBishop.validMove(tempSquare1, tempSquare2) != 0) {
                  hasValidMoves = true;
                }
                break;
              }
              case 'r': {
                Rook curRook = convertToRook(curPiece);
                if (curRook.validMove(tempSquare1, tempSquare2) != 0) {
                  hasValidMoves = true;
                }
                break;
              }  
              case 'q': {
                Queen curQueen = convertToQueen(curPiece);
                if (curQueen.validMove(tempSquare1, tempSquare2) != 0) {
                  hasValidMoves = true;
                }
                break;
              }
              case 'k': {
                King curKing = convertToKing(curPiece);
                if (curKing.validMove(tempSquare1, tempSquare2) != 0) {
                  hasValidMoves = true;
                }
                break;
              }
              default: { // Didn't match to a piece
                break;
              }
            }
            // If the player has any valid moves, 
            // check if the "valid" move doesn't result in check
            // This section tries the move and sees if it results in check
            // If it does, we undo the move and get another
            if (hasValidMoves) { 
              updateState(); // update checks/mates
              Cell selectedCell = chessBoard.find(tempSquare1)->second; // Gets the cell of the first piece
              // Stores values in case they need to be reset
              bool moveFlag = chessBoard.find(tempSquare1)->second.getPiece().getHasMoved();
              int enPassantFlag = chessBoard.find(tempSquare1)->second.getPiece().getEPCountDown();
              Piece tempPiece = chessBoard.find(tempSquare2)->second.getPiece(); // Store the piece in case the move is illegal 
              chessBoard.find(tempSquare2)->second.setPiece(selectedCell.getPiece()); // Move piece to new cell
              chessBoard.find(tempSquare2)->second.getPiece().setHasMoved(true);
              Piece blankPiece = Piece(); // Replace moved piece with empty square
              chessBoard.find(tempSquare1)->second.setPiece(blankPiece);
              updateState();
              if ((colour == false && getBlackInCheck() == true) || (colour == true && getWhiteInCheck() == true)) { // Illegal move, as it puts you in check
                hasValidMoves = false;
              }
              // Since only testing, ALWAYS put the move back
              chessBoard.find(tempSquare1)->second.setPiece(chessBoard.find(tempSquare2)->second.getPiece()); // Put piece back
              chessBoard.find(tempSquare1)->second.getPiece().setHasMoved(moveFlag); 
              chessBoard.find(tempSquare1)->second.getPiece().setEPCountDown(enPassantFlag);
              chessBoard.find(tempSquare2)->second.setPiece(tempPiece); // Put back the original piece
              updateState(); // Re-update
            }
            // Break out as soon as we find one valid move
            if (hasValidMoves) break;
          }
          if (hasValidMoves) break;
        }
        if (hasValidMoves) break;
      }
      if (hasValidMoves) break;
    }
    if (hasValidMoves) break;
  }
  // No valid moves, determine if checkmate or stalemate
  if (!hasValidMoves) { 
    if (colour == true) {
      if (whiteInCheck == true) {
        setWhiteInCheckmate(true);
      } 
      else {
        setWhiteInStalemate(true);
      }
    }
    else if (colour == false) {
      if (blackInCheck == true) {
        setBlackInCheckmate(true);
      } 
      else {
        setBlackInStalemate(true);
      }
    }
  }
}