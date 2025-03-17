#include "Mario.h"
#include "GameConfig.h"
#include <cmath>
// Define starting positions and game parameters

constexpr int MAX_FALL = 4;
constexpr int FIVE = 5;
constexpr int MAX_X = 80; // Maximum width of the board
constexpr int MAX_Y = 25; // Maximum height of the board
constexpr int NOTE_DELAY = 1000;
constexpr int NOTE_COUNTDOWN = 5;
constexpr char FLOOR_CHAR = ' ';
constexpr char LADDER_CHAR = 'H';
constexpr char CEILING_CHAR1 = '>';
constexpr char CEILING_CHAR2 = '=';
constexpr char CEILING_CHAR3 = '<';

#define marioToStartPoint p.setX(StartPoint.getX()),p.setY(StartPoint.getY()) // Macro for Mario's starting position
#define princessLocation (p.getX()==PrincessPoint.getX() && p.getY()==PrincessPoint.getY()) // Define the princess's location

// Handle key press events to move Mario
void Mario::keyPressed(char key) {
    Direction newDir = { 0, 0 };
    MoveKey moveKey = static_cast<MoveKey>(std::tolower(key));
    switch (moveKey) {
    case MoveKey::UP:
        if (isFloor() && p.getY() >= 3) dir = {0, -1};
        break;
    case MoveKey::LEFT:
        dir = { -1, 0 };
        break;
    case MoveKey::DOWN:
        dir = { 0, 1 };
        break;
    case MoveKey::RIGHT:
        dir = { 1, 0 };
        break;
    case MoveKey::STAY:
        dir = { 0, 0 };
        prevDirHammer = prevDir;
        prevDir = { 0, 0 };
        break;
    case MoveKey::PUNCH:
        if (IsHammer) {
            if (!IsKilling) IsKilling = 1;
            dir = prevDir;
            kill();
        }
        break;

    default:
        if (IsHammer && newDir.x != dir.x) {
            // Direction changed, erase the old hammer position
            erase();
        }

        dir = newDir;
        return; // Invalid key, do nothing
    }
    if (dir.x != prevDir.x || dir.y != prevDir.y) move();
}


// Move Mario based on the current direction
void Mario::move() {
    erase(); // Erase current position
    if (IsKilling) {
        IsKilling++;
        gotoxy(pBoard->getPActivePosition().getX(), pBoard->getPActivePosition().getY());
        std::cout << 'P';
    }
    else {
        gotoxy(pBoard->getPActivePosition().getX(), pBoard->getPActivePosition().getY());
        std::cout << ' ';
    }
    if (IsKilling >= 5) IsKilling = 0;
    int newX = p.getX() + dir.x; // Calculate new X position
    int newY = p.getY() + dir.y; // Calculate new Y position

    // Store previous horizontal direction if moving left or right
    if (dir.x != 0) {
        prevDirHammer = dir;
        prevDir.x = dir.x;
    }

    // Handle climbing logic
    if (isLadder() && dir.x == 0 && dir.y == -1) {
        isClimb = true; // Set climbing state to true
        dir.x = 0; prevDir.x = 0; // Reset horizontal direction
    }
    if (isClimb && isCeiling()) {
        newY = p.getY() - 2; // Adjust position if hitting ceiling while climbing
        dir = { 0, 0 }; // Stop movement in vertical direction
        isClimb = false; // Reset climbing state
    }

    // Handle jumping logic
    if (p.getX() > 75 || p.getX() < 5) {
        prevDir = { 0, 0 }; // Reset horizontal direction at boundaries
    }

    if (jump > 0 && !isLadder()) {
        if (jump <= 2) {
			p.setY(p.getY()-1);
        }
        else if (jump <= 4) {
            p.setY(p.getY() + 1);

        }
		p.setX(p.getX() + prevDir.x); // Apply horizontal movement during jump
        newX= p.getX(); // Update new X position during jump

        jump++;
        if (jump > 4) {
            jump = 0; // Reset jump after completion
            dir.x = prevDir.x; // Restore horizontal direction after jump
            dir.y = 0; // Reset vertical direction to zero after jump
        }
    }
    else {
        if (dir.x == 0 && dir.y == -1) { // Check for 'w' key pressed
            if (!isLadder() && isFloor()) {
                jump = 1; // Start jump sequence when on floor and not on ladder
                dir.y = 0; // Reset vertical direction to prevent downward movement immediately after jumping
                return;
            }
        }
        else if (dir.x == 0 && dir.y == 1) { // Check for 'x' key pressed to move down
            char under = pBoard->getChar(p.getX(), p.getY() + 1);
            if (under == '>' || under == '<' || under == '=') {
                if (pBoard->getChar(p.getX(), p.getY() + 2) == 'H') {
                    newY = p.getY() + 2; // Move down further on specific characters like 'H'
                }
            }
            else if (isLadder()) {
                newY = p.getY() + 1; // Move down one space on a ladder
            }
            else {
                dir = { prevDir.x, 0 }; // Cancel downward movement and maintain horizontal direction on floor 
            }
        }

        // Apply gravity logic 
        if (!isFloor() && !isLadder() && !jump) {
            dir.x = 0;
            newY++;
            countFall++;
        }
        else {
            if (countFall > MAX_FALL) {
                marioDamage();
                newX = StartPoint.getX();
                newY = StartPoint.getY();
            }
            countFall = 0;
        }

        // Update position within bounds checking 
        if (newX >= 0 && newX < MAX_X - 1 && newY >= 0 && newY < MAX_Y) {
			p.setX(newX);
			p.setY(newY);
        }
    }

    // Collision detection with obstacles on the board //taken from amirs exercise
    if ((pBoard->getChar(p.getX(), p.getY()) == '<' || pBoard->getChar(p.getX(), p.getY()) == '>' || pBoard->getChar(p.getX(), p.getY()) == '=') || pBoard->getChar(p.getX(), p.getY()) == 'Q') {
		p.setX(newX - dir.x); // Adjust position to avoid overlap with obstacles
		p.setY(newY - dir.y);
        dir = { 0, 0 }; // Stop movement on collision 
    }
    if (pBoard->getChar(newX, newY) == 'P') {
        pBoard->setChar(newX, newY, ' '); // Remove the Hammer character
        IsHammer = true; // Set the hammer flag to true
    }

    draw(); // Draw Mario at the new position 
}

// Draw Mario at the current position
void Mario::draw(char c) const {
    int dirx = dir.x;
    if (dirx == 0) {
        dirx = prevDirHammer.x;
    }
    if (IsHammer) {
        if (dirx > 0) {
            // Hammer on the right
            gotoxy(p.getX(), p.getY());
            std::cout << (IsColored ? "\033[31m" + std::string(1, c) + "\033[0m" : std::string(1, c));
            gotoxy(p.getX() + 1, p.getY());
            std::cout << 'P';
        }
        else {
            // Hammer on the left
            gotoxy(p.getX() - 1, p.getY());
            std::cout << 'P';
            gotoxy(p.getX(), p.getY());
            std::cout << (IsColored ? "\033[31m" + std::string(1, c) + "\033[0m" : std::string(1, c));
        }
    }
    else { // No hammer
        gotoxy(p.getX(), p.getY());
        std::cout << (IsColored ? "\033[31m" + std::string(1, c) + "\033[0m" : std::string(1, c));
    }
}

// Erase Mario from the current position
void Mario::erase() const {
    if (IsHammer) {
        // Erase both Mario and the hammer
        gotoxy(p.getX() - 1, p.getY());
        std::cout << pBoard->getChar(p.getX() - 1, p.getY());
        gotoxy(p.getX(), p.getY());
        std::cout << pBoard->getChar(p.getX(), p.getY());
        gotoxy(p.getX() + 1, p.getY());
        std::cout << pBoard->getChar(p.getX() + 1, p.getY());
    }
    else {
        // Erase only Mario
        gotoxy(p.getX(), p.getY());
        std::cout << pBoard->getChar(p.getX(), p.getY());
    }
}

// Handle killing action when 'P' or 'p' is pressed
void Mario::kill() {
    int targetX1 = p.getX() + prevDirHammer.x;
    int targetX2 = p.getX() + 2 * prevDirHammer.x;
    if (pBarrelArmy->removeBarrelAt(targetX1, p.getY(), p) || pGhostManager->removeGhostAt(targetX1, p.getY(), p)) {
        pBoard->updateScore(); // Increase score by 1 point
    }
    if (pBarrelArmy->removeBarrelAt(targetX2, p.getY(), p) || pGhostManager->removeGhostAt(targetX2, p.getY(), p)) {
        pBoard->updateScore(); // Increase score by 1 point
    }
    gotoxy(pBoard->getPActivePosition().getX(), pBoard->getPActivePosition().getY());
    std::cout << ' ';
}

// Set the game board for Mario
void Mario::setBoard(Board& board) {
    pBoard = &board;
    StartPoint.setX(pBoard->getMarioStartPoint().getX());
    StartPoint.setY(pBoard->getMarioStartPoint().getY());
    PrincessPoint.setX(pBoard->getPrincessStartPoint().getX());
    PrincessPoint.setY(pBoard->getPrincessStartPoint().getY());
}

// Handle damage to Mario
void Mario::marioDamage() {
    dir = { 0, 0 };
    Back = false;
    needsReset = true;
    IsHammer = false;
    if (pBoard->HowManyHeart() == 1) { // Only one heart remaining -> dead condition check 
        pBoard->setLives(0);
        dir = { 0, 0 };
        Back = true;
        marioToStartPoint;
    }
    else {
        pBoard->removeHeart();
    }
}

// Check if Mario should return to the menu
bool Mario::ToMenu() const {
    return Back; // Return whether Mario is in the back state to go to menu 
}

// Check if the level is complete
bool Mario::updateLevel() const {
    return LevelComplete; // Return whether Mario is in the back state to go to menu 
}

// Handle winning condition
void Mario::Win() {

    if (princessLocation) { // Check if reached the princess's location 
        marioToStartPoint;
        LevelComplete = true;
    }
}

// Handle collision with a barrel
void Mario::handleBarrel(int barrelX, int barrelY) {
    marioDamage();
    marioToStartPoint;
    draw();
}

// Handle collision with a ghost
void Mario::handleGhost(int barrelX, int barrelY) {
    marioDamage();
    marioToStartPoint;
    draw();
}

// Check if Mario is standing on the floor
bool Mario::isFloor() const {
    return !(pBoard->getChar(p.getX(), p.getY() + 1) == FLOOR_CHAR);
}

// Check if Mario is on a ladder
bool Mario::isLadder() const {
    return pBoard->getChar(p.getX(), p.getY() + dir.y) == LADDER_CHAR;
}

// Check if Mario is under a ceiling
bool Mario::isCeiling() const {
    char ch = pBoard->getChar(p.getX(), p.getY() - 1);
    return (ch == CEILING_CHAR1 || ch == CEILING_CHAR2 || ch == CEILING_CHAR3);
}

// Show the win note
void Mario::showWinNote() {
    IsHammer = false;
    const char* WinScreen[MAX_Y] = {
        // 01234567890123456789012345678901234567890123456789012345678901234567890123456789
        "                                                                                ", // 0
        "               __  __ ____   __  __   _       __ ____ _   __   __               ", // 1
        "               \\ \\/ // __ \\ / / / /  | |     / //  _// | / /  / /               ", // 2
        "                \\  // / / // / / /   | | /| / / / / /  |/ /  / /                ", // 3
        "                / // /_/ // /_/ /    | |/ |/ /_/ / / /|  /  /_/                 ", // 4
        "               /_/ \\____/ \\____/     |__/|__//___//_/ |_/  (_)                  ", // 5
        "                                                                                ", // 6
        "                                                                                ", // 7
        "                                                                                ", // 8
        "                                                                                ", // 9
        "                                                                                ", // 10
        "                                                                                ", // 11
        "                                                                                ", // 12
        "                                                                                ", // 13
        "                                                                                ", // 14
        "                                                                                ", // 15
        "                                                                                ", // 16
        "                                                                                ", // 17
        "                                                                                ", // 18
        "                                                                                ", // 19
        "                                                                                ", // 20
        "                                                                                ", // 21
        "                                                                                ", // 22
        "                                                                                ", // 23
        "                                                                                "  // 24
    };
    gotoxy(0, 0);
    for (int i = 0; i < MAX_Y; i++) {
        std::cout << WinScreen[i] << '\n';
    }
    if (this->IsColored)
        printMarioPhotoColored(Point(33, 8));
    else
        printMarioPhoto(Point(33, 8));
    showScoreNote();
    for (int i = NOTE_COUNTDOWN; i >= 0; i--) {
        gotoxy(23, 22);
        std::cout << "Continue to next stage in: " << i << " seconds   " << std::flush;
        Sleep(IsSilent ? 0 : NOTE_DELAY);
    }
    dir = { 0, 0 };
}

// Show the game finish note
void Mario::showGameFinishNote() {
    dir = { 0, 0 };
    IsHammer = false;
    const char* GameFinishedScreen[MAX_Y] = {
        // 01234567890123456789012345678901234567890123456789012345678901234567890123456789
        "                                                                                ", // 0
        "        ____                        _____ _       _     _              _ _      ", // 1
        "       / ___| __ _ _ __ ___   ___  |  ___(_)_ __ (_)___| |__   ___  __| | |     ", // 2
        "      | |  _ / _` | '_ ` _ \\ / _ \\ | |_  | | '_ \\| / __| '_ \\ / _ \\/ _` | |", // 3
        "      | |_| | (_| | | | | | |  __/ |  _| | | | | | \\__ \\ | | |  __/ (_| |_|   ", // 4
        "       \\____|\\__,_|_| |_| |_|\\___| |_|   |_|_| |_|_|___/_| |_|\\___|\\__,_(_)", // 5
        "                                                                                ", // 6
        "                                                                                ", // 7
        "                                                                                ", // 8
        "                                                                                ", // 9
        "                                                                                ", // 10
        "                                                                                ", // 11
        "                                                                                ", // 12
        "                                                                                ", // 13
        "                                                                                ", // 14
        "                                                                                ", // 15
        "                                                                                ", // 16
        "                                                                                ", // 17
        "                                                                                ", // 18
        "                                                                                ", // 19
        "                                                                                ", // 20
        "                                                                                ", // 21
        "                                                                                ", // 22
        "                                                                                ", // 23
        "                                                                                "  // 24
    };
    gotoxy(0, 0);
    for (int i = 0; i < MAX_Y; i++) {
        std::cout << GameFinishedScreen[i] << '\n';
    }
    showScoreNote();
    for (int i = NOTE_COUNTDOWN; i >= 0; i--) {
        gotoxy(23, 22);
        std::cout << "Back to main menu: " << i << " seconds   " << std::flush;
        Sleep(IsSilent ? 0 : NOTE_DELAY);
    }
}

void Mario::showLoseNote() {
    dir = { 0, 0 }; // Reset direction
    const char* LoseScreen[MAX_Y] = {
        // 01234567890123456789012345678901234567890123456789012345678901234567890123456789
        "                                                                                ", // 0
        "              _________    __  _________   ____ __   ____________               ", // 1
        "             / ____/   |  /  |/  / ____/  / __ | |  / / ____/ __ |              ", // 2
        "            / / __/ /| | / /|_/ / __/    / / / / | / / __/ / /_/ /              ", // 3
        "           / /_/ / ___ |/ /  / / /___   / /_/ /| |/ / /___/ _, _/               ", // 4
        "           |____/_/  |_/_/  /_/_____/   |____/ |___/_____/_/ |_|                ", // 5
        "                                                                                ", // 6
        "                                                                                ", // 7
        "                                                                                ", // 8
        "                                                                                ", // 9
        "                                                                                ", // 10
        "                                                                                ", // 11
        "                                                                                ", // 12
        "                                                                                ", // 13
        "                                                                                ", // 14
        "                                                                                ", // 15
        "                                                                                ", // 16
        "                                                                                ", // 17
        "                                                                                ", // 18
        "                                                                                ", // 19
        "                                                                                ", // 20
        "                                                                                ", // 21
        "                                                                                ", // 22
        "                                                                                ", // 23
        "                                                                                "  // 24
    };
    gotoxy(0, 0); // Move cursor to the top-left corner
    for (int i = 0; i < MAX_Y; i++) {
        std::cout << LoseScreen[i] << '\n'; // Print each line of the lose screen
    }
    if (this->IsColored)
        printMonkeyPhotoColored(Point(55, 6)); // Print colored monkey photo
    else
        printMonkeyPhoto(Point(55, 6)); // Print monkey photo
    showScoreNote(); // Show the score note
    for (int i = NOTE_COUNTDOWN; i >= 0; i--) {
        gotoxy(15, 10);
        std::cout << "Back to main menu in ";
        gotoxy(15, 11);
        std::cout << i << " seconds   " << std::flush;
        Sleep(IsSilent ? 0 : NOTE_DELAY);
    }
}

// Print Mario's photo at a specific position
void Mario::printMarioPhoto(Point p) {
    int x = p.getX(); int y = p.getY();
    gotoxy(x, y);
    std::cout << "\033[47;30m" << "\x20\xE2\x94\x80\xE2\x96\x84\xE2\x96\x88\xE2\x96\x88\xE2\x96\x88\xE2\x96\x88\xE2\x96\x84\xE2\x96\x84\x20" << "\033[0m";
    gotoxy(x, y + 1);
    std::cout << "\033[47;30m" << "\x20\xE2\x96\x84\xE2\x96\x80\xE2\x96\x84\xE2\x96\x80\xE2\x96\x90\xE2\x94\x94\xE2\x94\x80\xE2\x94\x90\x20" << "\033[0m";
    gotoxy(x, y + 2);
    std::cout << "\033[47;30m" << "\x20\xE2\x96\x88\xE2\x96\x84\xE2\x96\x90\xE2\x96\x8C\xE2\x96\x84\xE2\x96\x88\xE2\x96\x84\xE2\x94\x98\x20" << "\033[0m";
    gotoxy(x, y + 3);
    std::cout << "\033[47;30m" << "\x20\xE2\x94\x94\xE2\x96\x84\xE2\x96\x84\xE2\x96\x84\xE2\x96\x84\xE2\x96\x84\xE2\x94\x98\x20\x20" << "\033[0m";
    gotoxy(x, y + 4);
    std::cout << "\033[47;30m" << "\x20\xE2\x96\x88\xE2\x96\x88\x20\xE2\x96\x88\x20\xE2\x96\x88\xE2\x96\x88\xE2\x96\x88\x20" << "\033[0m" << std::flush;
}

// Print Mario's photo in color at a specific position
void Mario::printMarioPhotoColored(Point p) {
    int x = p.getX(); int y = p.getY();
    gotoxy(x, y);
    std::cout << "\033[47;30m" << "\033[31m\x20\xE2\x94\x80\xE2\x96\x84\xE2\x96\x88\xE2\x96\x88\xE2\x96\x88\xE2\x96\x88\xE2\x96\x84\xE2\x96\x84\x20\033[0m" << "\033[0m";
    gotoxy(x, y + 1);
    std::cout << "\033[47;30m" << "\x20\xE2\x96\x84\xE2\x96\x80\xE2\x96\x84\xE2\x96\x80\xE2\x96\x90\xE2\x94\x94\xE2\x94\x80\xE2\x94\x90\x20" << "\033[0m";
    gotoxy(x, y + 2);
    std::cout << "\033[47;30m" << "\x20\xE2\x96\x88\xE2\x96\x84\xE2\x96\x90\xE2\x96\x8C\xE2\x96\x84\xE2\x96\x88\xE2\x96\x84\xE2\x94\x98\x20" << "\033[0m";
    gotoxy(x, y + 3);
    std::cout << "\033[47;30m" << "\033[34m\x20\xE2\x94\x94\xE2\x96\x84\xE2\x96\x84\xE2\x96\x84\xE2\x96\x84\xE2\x96\x84\xE2\x94\x98\x20\x20\033[0m" << "\033[0m";
    gotoxy(x, y + 4);
    std::cout << "\033[47;30m" << "\033[34m\x20\xE2\x96\x88\xE2\x96\x88\x20\xE2\x96\x88\x20\xE2\x96\x88\xE2\x96\x88\xE2\x96\x88\x20\033[0m" << "\033[0m" << std::flush;
}

// Print monkey's photo at a specific position
void Mario::printMonkeyPhoto(Point p) {
    const char* Monkey[19] = {
    "            .-\"\"\"-.",
    "          _/-=-.   \\",
    "         (_|a a/   |_",
    "          / \"  \\   ,_)",
    "     _    \\`=' /__/",
    "    / \\_  .;--'  `-.",
    "    \\___)//      ,  \\",
    "     \\ \\/;        \\  \\",
    "      \\_.|         | |",
    "       .-\\ '     _/_/",
    "     .'  _;.    (_  \\",
    "    /  .'   `\\   \\\\_/",
    "   |_ /       |  |\\\\",
    "  /  _)       /  / ||",
    " /  /       _/  /  //",
    " \\_/       ( `-/  ||",
    "           /  /   \\\\ .-.",
    "           \\_/     \\'-'",
    "                    `\"`"
    };

    for (int i = 0; i < 19; i++) {
        gotoxy(p.getX(), p.getY() + i);
        std::cout << Monkey[i];
    }
    std::cout.flush();
}

// Print monkey's photo in color at a specific position
void Mario::printMonkeyPhotoColored(Point p) {
    const char* Monkey[19] = {
   "\033[38;2;139;69;19m            .-\"\"\"-.",
   "          _/-=-.   \\",
   "         (_\033[0m|\033[33ma a\033[0m/   \033[38;2;139;69;19m_|",
   "          \033[0m/ \"  \\\033[38;2;139;69;19m   ,_)",
   "     _    \033[0m\\`=' /\033[38;2;139;69;19m__/",
   "    / \\_  .;--'  `-.",
   "    \\___)//      ,  \\",
   "     \\ \\/;        \\  \\",
   "      \\_.|         | |",
   "       .-\\ '     _/_/",
   "     .'  _;.    (_  \\",
   "    /  .'   `\\   \\\\_/",
   "   |_ /       |  |\\\\",
   "  /  _)       /  / ||",
   " /  /       _/  /  //",
   " \\_/       ( `-/  ||",
   "           /  /   \\\\ .-.",
   "           \\_/     \\'-'",
   "                    `\"`\033[0m"
    };

    for (int i = 0; i < 19; i++) {
        gotoxy(p.getX(), p.getY() + i);
        std::cout << Monkey[i];
    }
    std::cout.flush();
}

// Show the score note
void Mario::showScoreNote() {
    gotoxy(3, 15);
    std::cout << " ####    ####    ####   #####   ######      ";
    gotoxy(3, 16);
    std::cout << "##      ##  ##  ##  ##  ##  ##  ##        ##";
    gotoxy(3, 17);
    std::cout << " ####   ##      ##  ##  #####   ####        ";
    gotoxy(3, 18);
    std::cout << "    ##  ##  ##  ##  ##  ##  ##  ##        ##";
    gotoxy(3, 19);
    std::cout << " ####    ####    ####   ##  ##  ######      ";

    int firstDigit = pBoard->getScore() / 10;
    int secondDigit = pBoard->getScore() % 10;

    if (firstDigit == 0) {
        if (secondDigit == 0)
            printDigit(firstDigit, Point(50, 15));
        else {
            printDigit(secondDigit, Point(50, 15));
        }
    }
    else {
        printDigit(firstDigit, Point(50, 15));
        printDigit(secondDigit, Point(56, 15));
    }
}

// Print a digit at a specific position
void Mario::printDigit(int digit, Point p) {
    const char* Digit0[FIVE] = {
        " #### ",
        "##  ##",
        "##  ##",
        "##  ##",
        " #### "
    };
    const char* Digit1[FIVE] = {
        " ##  ",
        "###  ",
        " ##  ",
        " ##  ",
        "#### "
    };
    const char* Digit2[FIVE] = {
        " #### ",
        "##  ##",
        "   ## ",
        " ##   ",
        "######"
    };
    const char* Digit3[FIVE] = {
        " #### ",
        "    ##",
        "  ##  ",
        "    ##",
        " #### "
    };
    const char* Digit4[FIVE] = {
        "##  ##",
        "##  ##",
        "######",
        "    ##",
        "    ##"
    };
    const char* Digit5[FIVE] = {
        "######",
        "##    ",
        "##### ",
        "    ##",
        "##### "
    };
    const char* Digit6[FIVE] = {
        " #### ",
        "##    ",
        "##### ",
        "##  ##",
        " #### "
    };
    const char* Digit7[FIVE] = {
        "######",
        "    ##",
        "   ## ",
        "  ##  ",
        " ##   "
    };
    const char* Digit8[FIVE] = {
        " #### ",
        "##  ##",
        " #### ",
        "##  ##",
        " #### "
    };
    const char* Digit9[FIVE] = {
        " #### ",
        "##  ##",
        " #### ",
        "    ##",
        " #### "
    };
    const char** Digit;
    switch (digit) {
    case 0:
        Digit = Digit0;
        break;
    case 1:
        Digit = Digit1;
        break;
    case 2:
        Digit = Digit2;
        break;
    case 3:
        Digit = Digit3;
        break;
    case 4:
        Digit = Digit4;
        break;
    case 5:
        Digit = Digit5;
        break;
    case 6:
        Digit = Digit6;
        break;
    case 7:
        Digit = Digit7;
        break;
    case 8:
        Digit = Digit8;
        break;
    case 9:
        Digit = Digit9;
        break;
    default:
        Digit = Digit0;
        break;
    }
    for (int i = 0; i < FIVE; i++) {
        gotoxy(p.getX(), p.getY() + i);
        std::cout << Digit[i];
    }
}