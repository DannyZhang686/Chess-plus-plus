// Overloads 

#include <iostream>
#include <string>
#include "include/main.hpp"
using namespace std;

// Definitions for board colours
// ANSI Recipe is ESC + BACKGROUND + ; + TEXT + m + RESET
#define ESC "\033["; // ANSI escape code
#define LIGHT_BACKGROUND "107"; // Color code for light background (light grey)
#define LIGHT_TEXT "31"; // Light Text (Red)
#define DARK_BACKGROUND "47";  // Dark(er) Grey.
#define DARK_TEXT "34"; // Dark Text (Blue)
#define RESET "\033[m"; // ANSI ending escape code

// Overloading cout for the chessBoard
ostream& operator<< (ostream& os, map<string, Cell>& inputBoard) {
  string coordinate; // String to hold the coordinate we are printing
  string unicodePiece = ""; // The string that will contain the piece and color code
  os << endl; // Push an endl for spacing
  for (int i = 8; i >= 1; i--) { // Print from 8th rank first (top of the board)
    os << i << " "; // Add the numbered ranks
    for (int j = 1; j <= 8; j++) { // Print from a-file first (left of the board)
      if ((i+j) % 2 == 1) { // If it's a white square
    		unicodePiece = ESC; // First bit of the ANSI Recipe
        unicodePiece += LIGHT_BACKGROUND;
        unicodePiece += ";";
      }
      else { // Black squares
        unicodePiece = ESC;
        unicodePiece += DARK_BACKGROUND;
        unicodePiece += ";";
      }
      // Here, we need to build coordinates to find pieces
      string tempFile(1, (char) (j + 96)); // Building a string from char 
      coordinate = (tempFile) + to_string(i); // Adding the number to get a coordinate (c1, e4)
      Piece curPiece = inputBoard.find(coordinate)->second.getPiece(); // Grabbing piece on the coordinate
      char pieceType = curPiece.getType(); // Get some info about the piece like color, type
      bool pieceColour = curPiece.getWhite();
      if (pieceColour == false) { // If it's a black piece
        unicodePiece += DARK_TEXT; // Add the black ANSI code
      }
      else { // White Piece
        unicodePiece += LIGHT_TEXT; // White ANSI code
      }
      unicodePiece += "m"; // "m" is part of the ANSI code
      switch (pieceType) { // Depending on piece type, add the corresponding unicode char
        case 'p': {
          unicodePiece += "♟";
          break;
        }
        case 'n': {
          unicodePiece += "♞";
          break;
        }
        case 'b': {
          unicodePiece += "♝";
          break;
        }
        case 'r': {
          unicodePiece += "♜";
          break;
        }
        case 'q': {
          unicodePiece += "♛";
          break;
        }
        case 'k': {
          unicodePiece += "♚";
          break;
        }
        default: { // If it's an empty square, add a space instead
          unicodePiece += " ";
          break;
        }
      }
      unicodePiece += " "; // Spacing purposes
      unicodePiece += RESET; // Finish off the ANSI code with RESET 
      os << unicodePiece; // Add that piece to the output
    }
    os << endl; // New line after every rank 
  }
  os << "  A B C D E F G H" << endl; // Output letter files
  return os; // Return the entire output
}

// Overloading cout for a vector of strings (for the move list)
// The vector itself is built in game.cpp at the end of each processTurn.
ostream& operator<<(ostream& os, vector<string>& moves) {
  for (int i = 0; i < moves.size(); i++) { // Moving through the vector of moves
    if (i % 2 == 0) { // White move
      os << i / 2 + 1 << ". "; // Move number (i / 2 + 1 due to std::vector 0-indexing)
      // Print a correct number of spaces to bring the total length of the move number up to 5 (chess games shouldn't exceed 999 moves, in which case three-digit moves have length 5 due to the period and space after the number)
      if (i / 2 + 1 < 10) {
        os << "  ";
      }
      else if (i / 2 + 1 < 100) {
        os << " ";
      }
      // else, no need to print any additional spaces
      os << moves[i];

      int effectiveStrLength = 2; // The actual string length, where a piece character is one character instead of four
      // Start at 2 because of a weird bug where a piece
      // character is sometimes only three characters in a
      // string instead of the usual four; so, since there
      // is a maximum of two non-ASCII piece characters in
      // a single move (0 in castling, 2 in promotions, and
      // and 1 in all other moves), starting at 2 will
      // never add an additional piece due to floor
      // division but will provide the fourth "part" of the
      // piece(s) in case 1 or 2 piece characters are
      // part of the move.
      for (int j = 0; j < moves[i].length(); j++) {
        if ((int) (moves[i][j]) < 10) {
          effectiveStrLength ++;
          // Add only 1 because it's not a normally used ASCII character, so it's part of a piece (small numbers and negative numbers)
        }
        else {
          effectiveStrLength += 4;
          // Add 4 because it's a regular ASCII character
        }
      }
      effectiveStrLength /= 4;
      for (int j = 11; j > effectiveStrLength; j--) {
        // Print some number of spaces to bring the total length of the move to a consistent 11
        // 11 is the string length needed for the longest possible move (in characters), a pawn promotion with check
        os << " ";
      }
      if (i == moves.size() - 1) { // After the last move, print an endl
        os << endl;
      }
    }
    else { // Black move
      os << " " << moves[i] << endl; // Move
    }
  }
  return os; 
}