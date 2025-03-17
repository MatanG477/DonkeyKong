#include "AutoGame.h"
#include "Results.h"
#include <iostream>

AutoGame::AutoGame() : GameStarter(), screenFiles(), isSilent(false), original_buf(nullptr) {}

void AutoGame::reportResultError(const std::string& message, const std::string& filename, size_t iteration) {
    system("cls");
    std::cerr << "Screen " << filename << " - " << message << '\n';
    std::cerr << "Iteration: " << iteration << '\n';
    std::cerr << "Press any key to continue to next screens (if any)" << std::endl;
    _getch();
}

bool AutoGame::initialize(bool isSilent) {
    this->isSilent = isSilent;
	mario.setIsSilent(isSilent);
    if (isSilent) {
        original_buf = std::cout.rdbuf();
        std::cout.rdbuf(nullptr);
    }
    SetConsoleOutputCP(CP_UTF8);
    ShowConsoleCursor(false);
    system("cls");
    gameConfig.setIsLoad(true);
    gameConfig.getAllBoardFileNames(screenFiles);
    std::sort(screenFiles.begin(), screenFiles.end());

    std::vector<std::string> validScreens;
    for (const auto& screen : screenFiles) {
        std::string prefix = screen.substr(0, screen.find_last_of('.'));
        if (std::filesystem::exists(prefix + ".result") && std::filesystem::exists(prefix + ".steps")) {
            validScreens.push_back(screen);
        }
        else {
            std::cout << "No result and steps files for screen: " << screen << std::endl;
        }
    }

    if (validScreens.empty()) {
        std::cout << "No valid screens found. Cannot start the game." << std::endl;
        return false;
    }

    screenFiles = validScreens;
    gameConfig.loadScreen(screenFiles[0], board, mario, barrel, barrelArmy, ghostManager, 3);
    gameConfig.setScreenLoaded(true);
    ghostManager = GhostManager(board, mario, board.getIsGameColored());
    ghostManager.initializeGhostsFromBoard();
    

    
    run();
    gameConfig.setRunning(false);
    return true;
}

//// Run the game loop
void AutoGame::run() {
    size_t currentScreenIndex = 0;
    int currentLives = 3; // Initialize lives
    bool unmatching_result_found = false;
    int currScore = 0;
    int currLives = 3;
    size_t iteration; // we need iteration to be outside the loop

    while (gameConfig.getCurrentScreenIndex() < screenFiles.size()) {
        
        if (!gameConfig.isScreenLoaded()) {
            gameConfig.loadScreen(screenFiles[gameConfig.getCurrentScreenIndex()], board, mario, barrel, barrelArmy, ghostManager, currentLives);
            board.print();
            mario.draw();
        }
        else {
            gameConfig.setScreenLoaded(false);
            board.print();
            mario.draw();
        }
       
        std::string filename = screenFiles[gameConfig.getCurrentScreenIndex()];
        std::string filename_prefix = filename.substr(0, filename.find_last_of('.'));
        std::string stepsFilename = filename_prefix + ".steps";
        std::string resultsFilename = filename_prefix + ".result";

        steps = Steps::loadSteps(stepsFilename);
        random_seed = steps.getRandomSeed();
        IsColored = steps.getIsColored();
        mario.setIsColored(IsColored);
        board.setIsGameColored(IsColored); // Ensure the board is also updated
        ghostManager.setColored(IsColored);
        results = Results::loadResults(resultsFilename);
        srand(random_seed);
        barrelArmy.setRandomSeed(random_seed);
        ghostManager.setRandomSeed(random_seed);

        iteration = 0;
        while (gameConfig.isRunning()) {
            if (!isSilent) {
                Sleep(FRAME_DELAY);
            }
            iteration++;

            Back_ = mario.ToMenu();
            levelComplete = mario.updateLevel(); // Check if the level is complete

            if (results.isFinishedBy(iteration)) {
                reportResultError("Results file reached finish while game hadn't!", filename, iteration);
                unmatching_result_found = true;
                currentScreenIndex++;
                gameConfig.setCurrentScreenIndex(currentScreenIndex);
                break;
            }

            if (steps.isNextStepOnIteration(iteration)) {
                mario.keyPressed(steps.popStep());
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
                gameConfig.setCurrentScreenIndex(currentScreenIndex);
                if (currentScreenIndex < screenFiles.size()) {
                    mario.showWinNote(); // Show win note
                    gameConfig.setScreenLoaded(false);
                    break;  // Break to load the next screen
                }
                else { // game finished
                    currScore = board.getScore();
                    mario.showGameFinishNote(); // Show game finish note
                    currentLives = 3;
                    currLives = 3;
                    board.setLives(3);
                    currentScreenIndex++;
                    gameConfig.setCurrentScreenIndex(currentScreenIndex);
                    break;
                }
            }
            if (currLives != board.getLives()) { // Lives changed
                currLives = board.getLives();
                if (results.popResult() != std::tuple{ iteration, Results::hit, currScore }) {
                    reportResultError("Results file doesn't match hit event!", filename, iteration);
                    unmatching_result_found = true;
                    currentScreenIndex++;
                    gameConfig.setCurrentScreenIndex(currentScreenIndex);
                    break;
                }
                currScore = board.getScore();
                if (currLives == 0) { // died
                    mario.showLoseNote(); // Show lose note
                    currScore = board.getScore();
                    if (results.popResult() != std::tuple{ iteration, Results::finished, currScore }) {
                        reportResultError("Results file doesn't match finished event!", filename, iteration);
                        unmatching_result_found = true;
                        currentScreenIndex++;
                        gameConfig.setCurrentScreenIndex(currentScreenIndex);
                        break;
                    }
                }
            }
            if (Back_) {
                currentLives = 3;
                currLives = 3;
                board.setLives(3);
                currentScreenIndex++;
                gameConfig.setCurrentScreenIndex(currentScreenIndex);
                gameConfig.setScreenLoaded(false);
                break;
            }
            gameConfig.updateGame(mario); // Update the game
            barrelArmy.update(mario); // Update the barrel army
            ghostManager.update(); // Update the ghost manager
            if (currScore != board.getScore()) { // score changed
                currScore = board.getScore();
                if (results.popResult() != std::tuple{ iteration, Results::score, currScore }) {
                    reportResultError("Results file doesn't match score event!", filename, iteration);
                    unmatching_result_found = true;
                    currentScreenIndex++;
                    gameConfig.setCurrentScreenIndex(currentScreenIndex);
                    break;
                }
            }

            if (unmatching_result_found && !results.isEmpty()) {
                reportResultError("Results file has additional events after finish event!", filename, iteration);
                unmatching_result_found = true;
                break;
            }
            

        }
    }
	system("cls");
    if (isSilent) {
        std::cout.rdbuf(original_buf);
        std::cout << (unmatching_result_found ? "Test failed." : "Test passed.") << std::endl;
        std::cout.flush();
    }
}

bool AutoGame::GetSilent() {
    return isSilent;
}

void AutoGame::SetSilent(bool silent) {
    isSilent = silent;
}


