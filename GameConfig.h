#pragma once
#include "Mario.h"
#include "Board.h"
#include "Barrel.h"
#include "BarrelArmy.h"
#include "Ghost.h"
#include "GhostManager.h"
#include <windows.h>
#include <vector>
#include <string>
#include <filesystem>
#include <conio.h>
#include "algorithm"

class GameConfig {
private:
    bool ScreenLoaded = false; // Flag to indicate if the screen is loaded
    bool isGameRunning; // Flag to indicate if the game is running
    bool isLoad = false;
    size_t currentScreenIndex = 0; // Index of the current screen
    std::vector<std::string> screenFiles; // Vector to store screen file names

public:
    GameConfig(); // Constructor to initialize the game configuration
    bool isGameColored; // Flag to indicate if the game is colored
    // Check if the screen is loaded
    bool isScreenLoaded() const { return ScreenLoaded; }
    // Set the screen loaded flag
    void setScreenLoaded(bool value) { ScreenLoaded = value; }
    void setIsLoad(bool value) { isLoad = value; }
    // Set the current screen index
    void setCurrentScreenIndex(size_t index) { currentScreenIndex = index; }
    // Get the screen files
    std::vector<std::string>& getScreenFiles();
    // Get the current screen index
    size_t getCurrentScreenIndex() const { return currentScreenIndex; }
    // Handle menu input
    void menuInput(Mario& mario, Board& board, Barrel& barrel, BarrelArmy& barrelArmy, GhostManager& ghostManager);
    // Handle game input
    bool handleInput(Mario& mario, bool& paused, char* Key);
    bool IsMoveKey(char key);
    // Update the game state
    void updateGame(Mario& mario);
    // Get all board file names
    void getAllBoardFileNames(std::vector<std::string>& vec_to_fill);
    // Load the screen from a file
    void loadScreen(const std::string& filename, Board& board, Mario& mario, Barrel& barrel, BarrelArmy& barrelArmy, GhostManager& ghostManager, int currentLives);
    // Show the pause menu
    void showPauseMenu();
    // Hide the pause menu
    void hidePauseMenu(Board& board);
    // Show the main menu
    void showMainMenu();
    // Show the instructions
    void showInstructions(Board& board, Mario& mario, Barrel& barrel, BarrelArmy& barrelArmy, GhostManager& ghostManager);
    // Show the screen options
    void showScreenOptions(Board& board, Mario& mario, Barrel& barrel, BarrelArmy& barrelArmy, GhostManager& ghostManager);
    // Check if the game is running
    bool isRunning() const { return isGameRunning; }
    // Set the running state of the game
    void setRunning(bool running) { isGameRunning = running; }
};