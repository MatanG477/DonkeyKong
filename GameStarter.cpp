#include "GameStarter.h"  // Include header for GameStarter class
#include "utils.h"         // Include utility functions
#include <windows.h>       // Include Windows header for system functions like GetTickCount

// Constructor to initialize the game state
GameStarter::GameStarter()
    : paused(false),
    results(), steps(), random_seed(0),
    Back_(false),
    levelComplete(false),
    board(),
    mario(),
    barrel(0, 0, board, mario, false, random_seed),  // Initialize barrel with default values
    barrelArmy(),
    ghostManager(board, mario, false) {
    // Any additional initialization if needed
}


// Reset the game to its initial state
void GameStarter::resetGame() {
    mario.needsReset = false;  // Reset Mario's state for the new game
    board.print();  // Print the board
    mario.draw();   // Redraw Mario on the screen
    barrelArmy.reset();  // Reset the barrel army
    ghostManager.initializeGhostsFromBoard(); // Initialize ghosts from the board
    ghostManager.reset();  // Reset the ghost manager
}

// Handle returning to the main menu
void GameStarter::handleBackToMenu() {
    Back_ = false;  // Reset the "Back to Menu" flag
    gameConfig.showMainMenu(); // Show the main menu
    gameConfig.menuInput(mario, board, barrel, barrelArmy, ghostManager); // Handle menu input
    mario.Back = false;  // Reset Mario's back state
}
