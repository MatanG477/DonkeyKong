#include "GameConfig.h"
#include <cstring>
// Define ASCII values for specific keys
constexpr int ESC = 27;
constexpr int ONE = 49;
constexpr int TWO = 50;
constexpr int THREE = 51;
constexpr int FOUR = 52;
constexpr int FIVE = 53;
constexpr int BACKSPACE_KEY = 8;
constexpr int SPACE_KEY = 32;
constexpr int Instructions = 56;
constexpr int Exit = 57;
constexpr int Space = 32;

// Constructor to initialize the game configuration
GameConfig::GameConfig() : isGameRunning(true), isGameColored(false) {}

// Handle menu input
void GameConfig::menuInput(Mario& mario, Board& board, Barrel& barrel, BarrelArmy& barrelArmy, GhostManager& ghostManager) {
    board.setScore(0);  // Reset the score 
    gotoxy(board.getScorePosition().getX(), board.getScorePosition().getY()); // Reset the score on the board
    std::cout << ' '; // Clear the score
    char str[] = " Game made by Adi Tapiero and Matan Gurfinkel ";
    char empty[sizeof(str)] = { ' ' }; // Initialize empty with spaces
    int len = static_cast<int>(strlen(str)); // Cast size_t to int
    bool validKeyPressed = false;
    char key = 0;

    for (int i = 0; i < len; i++) {
        if (_kbhit()) {
            key = _getch();
            if (key == ONE || key == TWO || key == Instructions || key == Exit) {
                validKeyPressed = true;
                break; // Break the loop if a valid key is pressed
            }
        }
        empty[i] = str[i];
        str[i] = ' ';
        empty[i + 1] = '\0';
        gotoxy(15, 17);
        std::cout << empty;
        gotoxy(15, 18);
        std::cout << str;
        str[i] = empty[i];
        empty[i] = ' ';
        Sleep(100);
    }

    if (!validKeyPressed) {
        while (!_kbhit()) {
            Sleep(500);
        }
        key = _getch();
    }

    while (key != ONE && key != Instructions && key != Exit && key != TWO) {
        key = _getch();
    }

    if (key == ONE) {
        system("cls");
        isGameColored = false;
        mario.setIsColored(false);
        board.setIsGameColored(false); // Ensure the board is also updated
        showScreenOptions(board, mario, barrel, barrelArmy, ghostManager);
    }
    else if (key == TWO) {
        system("cls");
        isGameColored = true;
        mario.setIsColored(true);
        board.setIsGameColored(true); // Ensure the board is also updated
        showScreenOptions(board, mario, barrel, barrelArmy, ghostManager);
    }
    else if (key == Instructions) {
        system("cls");
        showInstructions(board, mario, barrel, barrelArmy, ghostManager);
    }
    else if (key == Exit) {
        setRunning(false);
    }
}

// Handle game input
bool GameConfig::handleInput(Mario& mario, bool& paused, char* Key) {
    if (_kbhit()) {
        char key = _getch();
        if (key == ESC) {
            paused = !paused;
            return false;
        }
        if (!paused && IsMoveKey(key)) {
            mario.keyPressed(key);
            *Key = key;
            return true;
        }
    }
    return false;
}
bool GameConfig::IsMoveKey(char key) {
    return key == 'A' || key == 'D' || key == 'W' || key == 'S' || key == 'X' || key == 'P' || key == 'a' || key == 'd' || key == 'w' || key == 's' || key == 'x' || key == 'p';
}
// Update the game state
void GameConfig::updateGame(Mario& mario) {
    mario.erase();
    mario.move();
    mario.draw();
    mario.Win();
}

// Show the pause menu
void GameConfig::showPauseMenu() {
    if (isGameColored) {
        gotoxy(30, 12);
        std::cout << "\033[38;2;202;44;146mGame Paused";
        gotoxy(25, 14);
        std::cout << "Press ESC to resume\033[0m";
    }
    else {
        gotoxy(30, 12);
        std::cout << "Game Paused";
        gotoxy(25, 14);
        std::cout << "Press ESC to resume";
    }
}


// Hide the pause menu
void GameConfig::hidePauseMenu(Board& board) {
    const auto& currentBoard = board.getCurrentBoard(); // Use const reference to avoid unintended copy
    gotoxy(30, 12);
    for (int i = 0; i < 11; i++) // prints the previous board chars to hide the pause menu
    {
        std::cout << currentBoard[12][30 + i];
    }

    gotoxy(25, 14);
    for (int i = 0; i < 19; i++)
    {
        std::cout << currentBoard[14][25 + i];
    }
}

// Show the main menu
void GameConfig::showMainMenu() {
    system("cls");
    gotoxy(10, 1);
    std::cout << "______            _                _   __                  ";
    gotoxy(10, 2);
    std::cout << "|  _  \\          | |              | | / /                  ";
    gotoxy(10, 3);
    std::cout << "| | | |___  _ __ | | _____ _   _  | |/ /  ___  _ __   __ _ ";
    gotoxy(10, 4);
    std::cout << "| | | / _ \\| '_ \\| |/ / _ \\ | | | |    \\ / _ \\| '_ \\ / _` |";
    gotoxy(10, 5);
    std::cout << "| |/ / (_) | | | |   <  __/ |_| | | |\\  \\ (_) | | | | (_| |";
    gotoxy(10, 6);
    std::cout << "|___/ \\___/|_| |_|_|\\_\\___|\\__, | \\_| \\_/\\___/|_| |_|\\__, |";
    gotoxy(10, 7);
    std::cout << "                            __/ |                     __/ |";
    gotoxy(10, 8);
    std::cout << "                           |___/                     |___/ ";
    gotoxy(15, 9);
    std::cout << "================================================";
    gotoxy(15, 10);
    std::cout << "||      START UNCOLORED GAME - (press 1)      ||";
    gotoxy(15, 12);
    std::cout << "||        START COLORED GAME - (press 2)      ||";
    gotoxy(15, 14);
    std::cout << "||   PRESENT INSTRUCTIONS AND KEYS - (press 8)||";
    gotoxy(15, 16);
    std::cout << "||               EXIT - (press 9)             ||";
    gotoxy(15, 20);
    std::cout << "===============================================" << std::flush;
}

// Show the instructions
void GameConfig::showInstructions(Board& board, Mario& mario, Barrel& barrel, BarrelArmy& barrelArmy, GhostManager& ghostManager) {
    system("cls");
    gotoxy(5, 0);
    std::cout << "--------------------------------------------------------------------";
    gotoxy(5, 1);
    std::cout << "    INSTRUCTIONS                               GAME CONTROLS        ";
    gotoxy(5, 2);
    std::cout << "--------------------------------------------------------------------";
    gotoxy(5, 3);
    std::cout << "* Navigate through the game board                _______            ";
    gotoxy(5, 4);
    std::cout << "  to rescue Pauline by reaching her.            |\\     /|           ";
    gotoxy(5, 5);
    std::cout << "                                                | +---+ |           ";
    gotoxy(5, 6);
    std::cout << "* Avoid barrels by jumping                      | | W | |           "; // MOVE UP / JUMP - (press W)
    gotoxy(5, 7);
    std::cout << "  and avoid ghosts by jumping or hammer.        |   UP  |           ";
    gotoxy(5, 8);
    std::cout << "                                                | +---+ |           ";
    gotoxy(5, 9);
    std::cout << "* Use ladders to move between floors.       ____|/_____\\|__ _______ ";
    gotoxy(5, 10);
    std::cout << "  use hammer (P) to hit ghosts.            |\\     /|\\     /|\\     /|"; // MOVE LEFT - (press A)
    gotoxy(5, 11);
    std::cout << "                                           | +---+ | +---+ | +---+ |"; // MOVE RIGHT - (press D)
    gotoxy(5, 12);
    std::cout << "* Gain score by killing ghosts.            | | A | | | S | | | D | |"; // STAY - (press S)
    gotoxy(5, 13);
    std::cout << "                                           |  LEFT | STAY |  RIGHT |";
    gotoxy(5, 14);
    std::cout << "* Falling more than 5 lines will           | +---+ | +---+ | +---+ |";
    gotoxy(5, 15);
    std::cout << "  result in losing a life.                 |/_____\\|/_____\\|/_____\\|";
    gotoxy(5, 16);
    std::cout << "                                                    |\\     /|       ";
    gotoxy(5, 17);
    std::cout << "* A barrel falling more than 8 lines                | +---+ |       ";
    gotoxy(5, 18);
    std::cout << "  will explode and hurt mario if                    | | X | |       "; // MOVE DOWN - (press X)
    gotoxy(5, 19);
    std::cout << "  within 2 charachters range.                       | DOWN  |       ";
    gotoxy(5, 20);
    std::cout << "                                                    | +---+ |       ";
    gotoxy(5, 21);
    std::cout << "* You start with 3 lives.                           |/_____\\|       "; // PAUSE/RESUME GAME - (press ESC)
    gotoxy(5, 22);
    std::cout << "  The game is over once all lives are lost.      PAUSE/RESUME -  ESC";
    gotoxy(5, 23);
    std::cout << "--------------------------------------------------------------------";

    while (!_kbhit()) {
        gotoxy(5, 24);
        std::cout << "                   PRESS SPACE TO RETURN TO MENU                    " << std::flush;
        Sleep(600);
        gotoxy(5, 24);
        std::cout << "                -> PRESS SPACE TO RETURN TO MENU <-                 " << std::flush;
        Sleep(400);
    }
    char key = _getch();
    while (key != Space) {
        key = _getch();
    }
    showMainMenu();
    this->menuInput(mario, board, barrel, barrelArmy, ghostManager);
}

// Show the screen options
void GameConfig::showScreenOptions(Board& board, Mario& mario, Barrel& barrel, BarrelArmy& barrelArmy, GhostManager& ghostManager) {
    system("cls");
    getAllBoardFileNames(screenFiles);
    sort(screenFiles.begin(), screenFiles.end());

    gotoxy(5, 1);
    std::cout << "     ____                              ___        _   _";
    gotoxy(5, 2);
    std::cout << "    / ___|  ___ _ __ ___  ___ _ __    / _ \\ _ __ | |_(_) ___  _ __  ___";
    gotoxy(5, 3);
    std::cout << "    \\___ \\ / __| '__/ _ \\/ _ \\ '_ \\  | | | | '_ \\| __| |/ _ \\| '_ \\/ __|";
    gotoxy(5, 4);
    std::cout << "     ___) | (__| | |  __/  __/ | | | | |_| | |_) | |_| | (_) | | | \\__ \\";
    gotoxy(5, 5);
    std::cout << "    |____/ \\___|_|  \\___|\\___|_| |_|  \\___/| .__/ \\__|_|\\___/|_| |_|___/";
    gotoxy(5, 6);
    std::cout << "                                           |_|";
    gotoxy(5, 8);
    std::cout << "           ( ___ )--------------------------------------( ___ )";

    int page = 0;
    int itemsPerPage = 5;
    int totalPages = static_cast<int>((screenFiles.size() + itemsPerPage - 1) / itemsPerPage);

    while (true) {
        system("cls");
        gotoxy(5, 1);
        std::cout << "     ____                              ___        _   _";
        gotoxy(5, 2);
        std::cout << "    / ___|  ___ _ __ ___  ___ _ __    / _ \\ _ __ | |_(_) ___  _ __  ___";
        gotoxy(5, 3);
        std::cout << "    \\___ \\ / __| '__/ _ \\/ _ \\ '_ \\  | | | | '_ \\| __| |/ _ \\| '_ \\/ __|";
        gotoxy(5, 4);
        std::cout << "     ___) | (__| | |  __/  __/ | | | | |_| | |_) | |_| | (_) | | | \\__ \\";
        gotoxy(5, 5);
        std::cout << "    |____/ \\___|_|  \\___|\\___|_| |_|  \\___/| .__/ \\__|_|\\___/|_| |_|___/";
        gotoxy(5, 6);
        std::cout << "                                           |_|";
        gotoxy(5, 8);
        std::cout << "           ( ___ )--------------------------------------( ___ )";

        for (int i = 0; i < itemsPerPage; ++i) {
            int index = page * itemsPerPage + i;
            if (index < static_cast<int>(screenFiles.size())) {
                gotoxy(17, 10 + i * 2);
                if ((index + 1) < 10)
                    std::cout << "|   |  Screen " << (index + 1) << "  - " << screenFiles[index] << " (Press " << (index + 1) - page * itemsPerPage << ")  |   |";
				else
                    std::cout << "|   |  Screen " << (index + 1) << " - " << screenFiles[index] << " (Press " << (index + 1) - page * itemsPerPage << ")  |   |";
            }
            else {
                gotoxy(17, 10 + i * 2);
                std::cout << "|   |                                         |   |";
            }
        }

        if (page == 0) {
            gotoxy(2, 23);
            std::cout << "                                       page " << page + 1 << "/" << totalPages << "   Press [space] for next page   ";
        }
        else if (page == (static_cast<int>(screenFiles.size()) / itemsPerPage)) {
            gotoxy(2, 23);
            std::cout << "Press [BackSpace] for previous page    page " << page + 1 << "/" << totalPages << "                                ";
        }
        else {
            gotoxy(2, 23);
            std::cout << "Press [BackSpace] for previous page    page " << page + 1 << "/" << totalPages << "   Press [space] for next page   ";
        }

        gotoxy(5, 10 + itemsPerPage * 2);
        std::cout << "           ( ___ )--------------------------------------( ___ )";

        char key = _getch();
        if (key >= '1' && key <= '5') {
            int choice = (key - '1') + page * itemsPerPage;
            if (choice < static_cast<int>(screenFiles.size())) {
                currentScreenIndex = choice;
                loadScreen(screenFiles[currentScreenIndex], board, mario, barrel, barrelArmy, ghostManager, 3);
                this->setScreenLoaded(true);
                system("cls");
                board.print();
                mario.draw();
                return;
            }
        }
        else if (key == SPACE_KEY) {
            if (page < totalPages - 1) {
                page++;
            }
        }
        else if (key == BACKSPACE_KEY) {
            if (page > 0) {
                page--;
            }
        }
    }
}

// Get the screen files
std::vector<std::string>& GameConfig::getScreenFiles() {
    return screenFiles;
}

// Load the screen from a file
void GameConfig::loadScreen(const std::string& filename, Board& board, Mario& mario, Barrel& barrel, BarrelArmy& barrelArmy, GhostManager& ghostManager, int currentLives) {
    std::string mutableFilename = filename; // Create a mutable copy of the filename
    while (!board.reset(mutableFilename.c_str(), currentLives)) {
        std::cout << "Loading next file...\n";
        currentScreenIndex++;
        if (currentScreenIndex >= screenFiles.size()) {
            system("cls");
            gotoxy(5, 12);
            std::cout << "Error: You reached the last board file\n";
            Sleep(2000);
            if (isLoad) {
                setRunning(false); break;
            }
            gotoxy(5, 14);
            std::cout << "Loading the main menu...\n";
            Sleep(3000);
            showMainMenu();
            menuInput(mario, board, barrel, barrelArmy, ghostManager);
        }
        mutableFilename = screenFiles[currentScreenIndex]; // Assign new value to mutableFilename
    }
    mario.setStartPosition(Point(board.getMarioStartPoint().getX(), board.getMarioStartPoint().getY()));
    mario.setBoard(board);
    mario.setBarrelArmy(barrelArmy);
    mario.setGhostManager(ghostManager);
    barrel.setStartPosition(Point(board.getDonkeyStartPoint().getX(), board.getDonkeyStartPoint().getY()));
    barrel.setBoard(board);
    barrel.setMario(mario);
    barrelArmy.setBoard(board);
    barrelArmy.setMario(mario);
    barrelArmy.reset();
    ghostManager.setColored(board.getIsGameColored());
    ghostManager.initializeGhostsFromBoard();
}

// Get all board file names
void GameConfig::getAllBoardFileNames(std::vector<std::string>& vec_to_fill) {
    namespace fs = std::filesystem;
    for (const auto& entry : fs::directory_iterator(fs::current_path())) {
        auto filename = entry.path().filename();
        auto filenameStr = filename.string();
        if (filenameStr.substr(0, 5) == "dkong" && filename.extension() == ".screen") {
            vec_to_fill.push_back(filenameStr);
        }
    }
}
