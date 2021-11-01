// C++ Chess Project
// Justin, Danny, Evan
// May 24, 2021

// Main file, handles the menus and overall structure of the program
#include <iostream>
#include <stdlib.h>
#include <string>
#include <ctype.h>
#include <vector>

#include "include/main.hpp"
using namespace std;

// Initial menu for choosing options, returns true/false to exit loop in main
bool startMenu() { 
  string menuChoice; // What the user chooses
  int stoiMenuChoice; // Integer version of user choice
  bool validMenu = false; // Whether or not the input was valid
  cout << "Welcome to Danny, Evan, and Justin's C++ Chess Game!\n1. Start\n2. Rules\n3. Quit\n";
  while (validMenu == false) { // Input validation for first menu
    cin >> menuChoice;
    validMenu = true; // Assuming true each time through
    try { 
      stoiMenuChoice = stoi(menuChoice); // Try to convert a string to an int
    } catch (...) { // Catch any error
        cout << "Invalid choice, try again." << endl; 
        validMenu = false; // Setting false to restart loop if invalid
    }
  }
  switch(stoiMenuChoice) { // Switch between 1,2,3
    case 1: { // Starting program: 
      cout << "Starting..." << endl;
      return true; // Main only continues if we return true
    }
    case 2: { // Rules:
      cout << "Rules: \nIf you don't know how to play chess, read up on the rules of chess here:\nhttps://en.wikipedia.org/wiki/Rules_of_chess\nTo move a piece, type in the square it is on, and the square you wish to move it to. For example, to play 1. e4, enter \"e2 e4\". \n\nSpecial rules:\nTo castle, enter the initial and final squares of the king if you were to castle normally (e1 g1).\nEn Passant works similarly. Enter the initial and final squares of the capturing pawn.\n " << endl; 
      break;
    }
    case 3: { // Quit Program
      cout << "Quitting..." << endl;
      exit(0);
    }    
    default: { // Wasn't a valid choice
      cout << "Invalid choice, try again." << endl;
      break;
    }
  }
  return false; // Return false if player didn't choose continue (re-displays this menu)
}

// Final menu to play again or quit
int endMenu() {
  bool playAgainValid = false; // Input validation loop for playAgain
  string playAgainChoice; // Taking input as string to avoid crashes
  int stoiPlayAgainChoice; // Converting to int
  cout << "\n1. Play Again\n2. Main Menu\n3. Quit\n" << endl;
    while (playAgainValid == false) { // Loop while user hasn't entered a valid choice
      playAgainValid = true; // Assume true (if valid, then the loop won't restart)
      cin >> playAgainChoice;
      try {
        stoiPlayAgainChoice = stoi(playAgainChoice); 
      } catch (...) { // If input can't be converted, it's invalid
        cout << "Invalid input." << endl;
        playAgainValid = false; // Set to false and restart loop
      }
    }
    switch(stoiPlayAgainChoice) {
      case 1: { // Restart (same names, keep scores)
        cout << "Restarting..." << endl;
        return 1;
      }
      case 2: { // Main menu (re-initialize names, reset scores)
        return 2;
      }
      case 3: { // Quit (end program)
        cout << "Quitting..." << endl;
        exit(0);
      }
      default: { // None of the above
        cout << "Invalid input." << endl;
        break;
      }
    }
  return 0; // Returning 0 by default (doesn't progress through the loop)
}

// Main function
int main() {
  bool playAgain = true; // Loop the entire program
  bool loopStartMenu = false; // Loop the start menu
  int loopEndMenu = 0; // Loop the end menu
  int turn; // Outcome of processTurn
  bool gameOver = false; // Loop the game (each turn)
  double player1Score = 0; // Scores
  double player2Score = 0;
  Game game; // The chess game itself
 
  while (playAgain == true) { // Allows user to play another game
    // 0 is default (run first time), 2 means user chose "main menu" 
    if (loopEndMenu == 0 || loopEndMenu == 2) {
      do { // Show first menu 
        loopStartMenu = startMenu();
        player1Score = 0; // Reset scores 
        player2Score = 0;
      } while (loopStartMenu == false);
      game.setNames(); // Set the names of both players
    } 
    initialize(); // Initialize the board (initialize.cpp)
    game.clearMoves(); // Clear move list
    while (gameOver == false) { // Main game loop 
      cout << "\x1B[2J\x1B[H"; // Clear console
      cout << "C++ Chess Game\n\n" << chessBoard << endl; // Print board
      // White's turn
      vector<string> tempVector = game.getMoves(); // Get the move list
      cout << tempVector << endl; // Print moves
      turn = game.processTurn1(); // Play a turn and store in turn
      switch(turn) { // 0 = normal, 1 = resign, 2 = draw
        case 0: { // Continue
          break;
        }
        case 1: { // White resigns
          cout << "Game over: " << game.getPlayer(2).getName() << " wins by resignation." << endl;
          player2Score++; // Add to score
          gameOver = true; // Leave loop
          break;
        }
        case 2: { // Draw
          cout << "Game over: drawn by agreement." << endl;
          player1Score += 0.5; // Draws add 0.5 to each player
          player2Score += 0.5;
          gameOver = true;
          break;
        }
      }
      ePCountDecrease(); // After every ply decrease en passant counter
      pawnPromotion(game); // Check if any pawns need to be promoted
      Game::mateCheck(false); // Check if black can still play

      if (Game::getBlackInCheckmate()) { // If black is in checkmate
        // Add checkmate symbol
        game.editMove(game.getMoves()[game.getMoves().size() - 1] + "#", game.getMoves().size() - 1);
        gameOver = true; // game is over
        cout << "\x1B[2J\x1B[H"; // Clear console
        cout << "C++ Chess Game\n\n" << chessBoard << endl;
        tempVector = game.getMoves();
        cout << tempVector << endl << "\nGame over: " << game.getPlayer(1).getName() << " wins by checkmate." << endl;
        player1Score++; // Add to P1's score
      }
      else if (Game::getBlackInStalemate()) { // If black is in stalemate
        gameOver = true;
        cout << "\x1B[2J\x1B[H"; // Clear console
        cout << "C++ Chess Game\n\n" << chessBoard << endl << tempVector << endl << "\nGame over: Drawn by stalemate." << endl;
        player1Score += 0.5; // Add scores
        player2Score += 0.5;
      }
      else if (Game::getBlackInCheck()) {        
        // Add check symbol
        game.editMove(game.getMoves()[game.getMoves().size() - 1] + "+", game.getMoves().size() - 1);
      } 
      if (gameOver == true) { // If game is over
        break; // Get out of loop
      }
      cout << "\x1B[2J\x1B[H"; // Clear console
      cout << "C++ Chess Game\n\n" << chessBoard << endl; // Print board position
      // Black's turn
      tempVector = game.getMoves();
      cout << tempVector << endl;
      turn = game.processTurn2();
      switch(turn) { // 0 = continue, 1 = resign, 2 = draw
        case 0: { // Continue
          break;
        }
        case 1: { // Resign
          cout << "Game over: " << game.getPlayer(1).getName() << " wins by resignation." << endl;
          player1Score++;
          gameOver = true;
          break;
        }
        case 2: { // Draw
          cout << "Game over: Drawn by agreement." << endl;
          player1Score += 0.5;
          player2Score += 0.5;
          gameOver = true;
          break;
        }
      }
      ePCountDecrease(); // Same logic for White
      pawnPromotion(game); 
      Game::mateCheck(true); 
      if (Game::getWhiteInCheckmate()) {
        // Add checkmate symbol
        game.editMove(game.getMoves()[game.getMoves().size() - 1] + "#", game.getMoves().size() - 1);
        gameOver = true; // game is over
        cout << "\x1B[2J\x1B[H"; // Clear console
        cout << "C++ Chess Game\n\n" << chessBoard << endl;
        tempVector = game.getMoves();
        cout << tempVector << endl << "\nGame over: " << game.getPlayer(2).getName() << " wins by checkmate." << endl;
        player2Score++; // Add to score
      } 
      else if (Game::getWhiteInStalemate()) {
        gameOver = true;
        tempVector = game.getMoves();
        cout << "\x1B[2J\x1B[H"; // Clear console
        cout << "C++ Chess Game\n\n" << chessBoard << endl << tempVector << endl << "\nGame over: Drawn by stalemate." << endl;
        player1Score += 0.5;
        player2Score += 0.5;
      }
      else if (Game::getWhiteInCheck()) {
        // Add check symbol
        game.editMove(game.getMoves()[game.getMoves().size() - 1] + "+", game.getMoves().size() - 1);
      }
      if (gameOver == true) { 
        break; // Get out of loop
      }
    }
    gameOver = false; // Resetting in case players want to play again
    cout << "Current match score: " << game.getPlayer(1).getName() << " " << player1Score << " - " << player2Score << " " << game.getPlayer(2).getName() << endl;
    do { // Show end menu 
      loopEndMenu = endMenu();
    } while (loopEndMenu == 0);
    if (loopEndMenu == 1) { // 1 means play again
      game.swapNames(); // flip sides on rematch by changing names
      swap(player1Score, player2Score); // flip scores to match names
    }
  }
  return 0;
}