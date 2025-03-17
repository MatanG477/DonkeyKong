#pragma once
#include "Board.h"
#include "Mario.h"
#include "Barrel.h"
#include "BarrelArmy.h"
#include "GhostManager.h"
#include "GameConfig.h"
#include "Steps.h"
#include "Results.h"


class GameStarter {
protected:
    Board board; // Game board
    long random_seed;
    Steps steps;
    Results results;
    Mario mario; // Mario character
    GameConfig gameConfig;
    Barrel barrel; // Single barrel
    BarrelArmy barrelArmy; // Group of barrels
    GhostManager ghostManager;
    bool paused; // Game paused state
    bool Back_; // Flag to return to menu
    bool levelComplete;
    bool IsColored = false;
    static constexpr int FRAME_RATE = 20; // Frames per second
    static constexpr int FRAME_DELAY = 1000 / FRAME_RATE; // Delay between frames in milliseconds

    GameStarter(const GameStarter&) = delete;

    // Reset the game to its initial state
    void resetGame();
    // Handle returning to the main menu
    void handleBackToMenu();

public:
    // Constructor to initialize the game starter
    GameStarter();
    // Initialize the game components
    virtual bool initialize(bool isSilent) = 0;
    // Run the game loop
    virtual void run() = 0;
    // Set the running state of the game

};