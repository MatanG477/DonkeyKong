#include "ManualGame.h"
#include "Results.h"
#include <iostream>
#include <filesystem>

ManualGame::ManualGame() : GameStarter() {}

void ManualGame::deleteExistingFiles() {
    for (const auto& entry : std::filesystem::directory_iterator(".")) {
        std::string path = entry.path().string();
        if (path.ends_with(".result") || path.ends_with(".steps")) {
            std::filesystem::remove(path);
        }
    }
}

bool ManualGame::initialize(bool isSilent) {
    SetConsoleOutputCP(CP_UTF8);
    ShowConsoleCursor(false);
    gameConfig.showMainMenu();
    gameConfig.menuInput(mario, board, barrel, barrelArmy, ghostManager);
    ghostManager = GhostManager(board, mario, board.getIsGameColored());
    ghostManager.initializeGhostsFromBoard();
    run();
    return true; // Always return true for ManualGame
}

// Run the game loop
void ManualGame::run() {
    deleteExistingFiles();
    std::vector<std::string>& screenFiles = gameConfig.getScreenFiles(); // Get screen files
    size_t currentScreenIndex = gameConfig.getCurrentScreenIndex(); // Get current screen index
    int currentLives = 3; // Initialize lives
    int currScore = 0;
    int currLives = 3;
    size_t iteration = 0; // we need iteration to be outside the loop
    while (gameConfig.isRunning() && gameConfig.getCurrentScreenIndex() < screenFiles.size()) {
        currScore = board.getScore();

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

        std::string filename = screenFiles[gameConfig.getCurrentScreenIndex()];
        std::string filename_prefix = filename.substr(0, filename.find_last_of('.'));
        std::string stepsFilename = filename_prefix + ".steps";
        std::string resultsFilename = filename_prefix + ".result";

        // Open the steps file for writing (create if it doesn't exist, truncate if it does)
        std::ofstream stepsFile(stepsFilename, std::ios::out | std::ios::trunc);
        if (!stepsFile.is_open()) {
            std::cout << "Failed to open or create steps file: " << stepsFilename << std::endl;
            return;
        }

        // Open the results file for writing (create if it doesn't exist, truncate if it does)
        std::ofstream resultsFile(resultsFilename, std::ios::out | std::ios::trunc);
        if (!resultsFile.is_open()) {
            std::cout << "Failed to open or create results file: " << resultsFilename << std::endl;
            return;
        }
        results.ClearResults(); // Clear the results
        steps.ClearSteps(); // Clear the steps
        random_seed = static_cast<long>(std::chrono::system_clock::now().time_since_epoch().count());
        steps.setRandomSeed(random_seed);
        steps.setIsColored(gameConfig.isGameColored);
        srand(random_seed);
        barrelArmy.setRandomSeed(random_seed);
        ghostManager.setRandomSeed(random_seed);

        iteration = 0;
        while (gameConfig.isRunning()) {
            currScore = board.getScore();
            Sleep(FRAME_DELAY);  // Replace real-time tracking with a fixed delay for each frame
            char key;
            iteration++;

            Back_ = mario.ToMenu(); // Check if Mario wants to go back to the menu
            levelComplete = mario.updateLevel(); // Check if the level is complete

            if (gameConfig.handleInput(mario, paused, &key)) { // If there's new input
                steps.addStep(iteration, key);
            }

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
                    results.addResult(iteration, Results::finished, board.getScore());
                    steps.saveSteps(stepsFilename); // Save the steps to the steps file
                    results.saveResults(resultsFilename); // Save the results to the results file

                    mario.showWinNote(); // Show win note
                    gameConfig.setScreenLoaded(false);
                    break;  // Break to load the next screen
                }
                else { // Game finished
                    results.addResult(iteration, Results::finished, board.getScore());
                    steps.saveSteps(stepsFilename); // Save the steps to the steps file
                    results.saveResults(resultsFilename); // Save the results to the results file
                    mario.showGameFinishNote(); // Show game finish note
                    currentLives = 3;
                    screenFiles.clear();
                    handleBackToMenu(); // Handle back to menu
                    currentScreenIndex = gameConfig.getCurrentScreenIndex(); // Get the current screen index
                    break;
                }
            }

            if (currLives != board.getLives()) { // Lives changed
                currLives = board.getLives();
                results.addResult(iteration, Results::hit, board.getScore());

                if (currLives == 0) { // Died
                    mario.showLoseNote();
                    results.addResult(iteration, Results::finished, board.getScore());
                }
            }

            if (Back_) {
                currentLives = 3;
                steps.saveSteps(stepsFilename); // Save the steps to the steps file
                results.saveResults(resultsFilename); // Save the results to the results file
                screenFiles.clear();
                handleBackToMenu(); // Handle back to menu
                break;
            }

            if (paused) {
                gameConfig.showPauseMenu(); // Show pause menu
            }
            else {
                gameConfig.hidePauseMenu(board); // Hide pause menu
                gameConfig.updateGame(mario);   // Update the game state
                barrelArmy.update(mario);      // Update barrels
                ghostManager.update();         // Update ghosts
                if (currScore != board.getScore()) { // Score changed
                    currScore = board.getScore();
                    results.addResult(iteration, Results::score, currScore);
                }
            }
        }

        steps.saveSteps(stepsFilename);   // Save recorded steps at end of level/game.
        results.saveResults(resultsFilename);   // Save recorded results at end of level/game.
    }
}

void ManualGame::SetSilent(bool silent) {
	isSilent = silent;
}

