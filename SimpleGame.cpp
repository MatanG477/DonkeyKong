#include "SimpleGame.h"
#include <iostream>


// Initialize the game components
bool SimpleGame::initialize(bool isSilent) {
    SetConsoleOutputCP(CP_UTF8); // Set console output to UTF-8
    ShowConsoleCursor(false); // Hide the console cursor
    gameConfig.showMainMenu(); // Show the main menu
    gameConfig.menuInput(mario, board, barrel, barrelArmy, ghostManager); // Handle menu input
    ghostManager = GhostManager(board, mario, board.getIsGameColored()); // Initialize ghost manager
    ghostManager.initializeGhostsFromBoard(); // Initialize ghosts from the board
	run(); // Run the game
    return true;
}

// Run the game loop
void SimpleGame::run() {
    std::vector<std::string>& screenFiles = gameConfig.getScreenFiles(); // Get screen files
    size_t currentScreenIndex = gameConfig.getCurrentScreenIndex(); // Get current screen index
    int currentLives = 3; // Initialize lives

    while (gameConfig.isRunning() && gameConfig.getCurrentScreenIndex() < screenFiles.size()) {
        // Load the current screen
        if (!gameConfig.isScreenLoaded()) {
            gameConfig.loadScreen(screenFiles[gameConfig.getCurrentScreenIndex()], board, mario, barrel, barrelArmy, ghostManager, currentLives);
            system("cls");
            board.print();
            mario.draw();
        }
        else {
            gameConfig.setScreenLoaded(false);
            system("cls");
            board.print();
            mario.draw();
        }

        while (gameConfig.isRunning()) {
            Sleep(FRAME_DELAY);  // Replace real-time tracking with a fixed delay for each frame
            char key;
            Back_ = mario.ToMenu(); // Check if Mario wants to go back to the menu
            gameConfig.handleInput(mario, paused, &key); // Handle game input
            Back_ = mario.ToMenu(); // Check if Mario wants to go back to the menu
            levelComplete = mario.updateLevel(); // Check if the level is complete
            gameConfig.handleInput(mario, paused, &key); // Handle game input

            if (mario.needsReset) {
                resetGame(); // Reset the game if needed
                continue;
            }

            if (levelComplete) {
                currentLives = board.getLives(); // Get the current lives
                mario.LevelComplete = false;
                levelComplete = false;
                currentScreenIndex++;
                gameConfig.setCurrentScreenIndex(currentScreenIndex); // Set the current screen index
                if (currentScreenIndex < screenFiles.size()) {
                    mario.showWinNote(); // Show win note
                    gameConfig.setScreenLoaded(false);
                    break;  // Break to load the next screen
                }
                else {
                    mario.showGameFinishNote(); // Show game finish note
                    currentLives = 3;
                    screenFiles.clear();
                    handleBackToMenu(); // Handle back to menu
                    currentScreenIndex = gameConfig.getCurrentScreenIndex(); // Get the current screen index
                }
            }
            if (board.getLives() == 0) {
                mario.showLoseNote();
            }

            if (Back_) {
                currentLives = 3;
                screenFiles.clear();
                handleBackToMenu(); // Handle back to menu
                break;
            }

            if (paused) {
                gameConfig.showPauseMenu(); // Show pause menu
            }
            else {
                gameConfig.hidePauseMenu(board); // Hide pause menu
                gameConfig.updateGame(mario); // Update the game
                barrelArmy.update(mario); // Update the barrel army
                ghostManager.update(); // Update the ghost manager
            }


        }
    }
}

void SimpleGame::SetSilent(bool silent) {
    isSilent = silent;
}