#include "Barrel.h"
#include <thread>
#include <chrono>

constexpr int MAX_X = 80;
constexpr int MAX_Y = 25;

constexpr char FLOOR_CHAR = ' ';
constexpr char LADDER_CHAR = 'H';
constexpr char CEILING_CHAR1 = '>';
constexpr char CEILING_CHAR2 = '=';
constexpr char CEILING_CHAR3 = '<';

// Constructor to initialize the barrel
Barrel::Barrel(int startX, int startY, Board& board, Mario& mario, bool colored, long seed)
    : Enemy(startX, startY, board, mario, colored), random_seed(seed) {

    directionRoll = (rand() % 2) + 1;
    DonkeySTARTX = board.getDonkeyStartPoint().getX();
    DonkeySTARTY = board.getDonkeyStartPoint().getY();
    p1.setX(DonkeySTARTX - 1);
    p1.setY(DonkeySTARTY);
    p2.setX(DonkeySTARTX + 1);
    p2.setY(DonkeySTARTY);
}

// Function to determine the direction the barrel should move based on the floor type below it
void Barrel::FollowFloorDirection() {
    // Get the character at the position directly below the barrel
    char floorType = pBoard->getChar(p.getX(), p.getY() + 1);

    // Determine movement direction based on the floor type ('<', '>', or '=')
    if (floorType == CEILING_CHAR3) {
        dir = { -1, 0 }; // Move left
        prevDir = dir;   // Store previous direction for later use
    }
    else if (floorType == CEILING_CHAR1) {
        dir = { 1, 0 }; // Move right
        prevDir = dir;   // Store previous direction for later use
    }
    else if (floorType == CEILING_CHAR2) {
        dir = prevDir;   // Continue in the same direction as before
    }
}

// Function to move the barrel on the board
void Barrel::move() {
    isMoving++; // Increment move counter
    // Check if it's time to move based on prime number conditions
    if ((isMoving % 5 == 0 || isMoving % 7 == 0)) {
        erase(); // Erase the barrel from its current position

        // Calculate new X and Y positions based on current direction
        int newX = p.getX() + dir.x;
        int newY = p.getY() + dir.y;

        // Check if the barrel is falling (empty space below it)
        if (pBoard->getChar(p.getX(), p.getY() + 1) == ' ') {
            dir = { 0, 1 }; // Set direction to move downward
            countFall++; // Increment fall counter
        }
        else {
            // If the barrel has been falling for a while
            if (countFall > 0) {
                // Check if the barrel has been falling for too long
                if (countFall > 7) {
                    barrelExplode(); // Explode the barrel if it's been falling too long
                    return; // Exit early after explosion
                }
                countFall = 0; // Reset fall count

                // Check the floor type below the barrel to adjust movement
                char floorType = pBoard->getChar(p.getX(), p.getY() + 1);
                if (floorType == CEILING_CHAR3) {
                    dir = { -1, 0 }; // Move left on '<' floor
                }
                else if (floorType == CEILING_CHAR1) {
                    dir = { 1, 0 }; // Move right on '>' floor
                }
                else if (floorType == CEILING_CHAR2) {
                    dir = prevDir; // Continue in previous direction on '=' floor
                }
            }
        }

        // Recalculate new positions based on updated direction
        newX = p.getX() + dir.x;
        newY = p.getY() + dir.y;

        // Check for out-of-bounds conditions and remove the barrel if it goes out of bounds
        if (newX < 0 || newX >= MAX_X || newY < 0 || newY >= MAX_Y || pBoard->getChar(newX, newY) == 'Q') {
            exploded = true; // Mark the barrel as exploded to indicate it should be removed
            // Reset barrel to specific position based on direction roll without explosion
            if (directionRoll % 2 == 1)
                reset(p1.getX(), p1.getY()); // Reset to first position
            else
                reset(p2.getX(), p2.getY()); // Reset to second position
            return; // Exit early after resetting
        }

        // Update barrel's position if it's within the board boundaries
        if (newX >= 0 && newX < MAX_X && newY >= 0 && newY < MAX_Y) {
            p.setX(newX); // Update X position
            p.setY(newY); // Update Y position
        }

        draw(); // Draw the barrel at its new position
    }

    // Reset the moving counter after a certain threshold
    if (isMoving > 11)
        isMoving = 1;
}

// Function to draw the barrel on the board
void Barrel::draw() const {
    gotoxy(p.getX(), p.getY());
    if (exploded) {
        std::cout << pBoard->getChar(p.getX(), p.getY());
    }
    else {
        char uncolored = ch;
        std::string colored = "\033[38;2;139;69;19m" + std::string(1, ch) + "\033[0m";
        if (pMario && pMario->getIsColored())
            std::cout << colored;
        else
            std::cout << uncolored;
    }
}

// Function to erase the barrel from the board
void Barrel::erase() const {
    gotoxy(p.getX(), p.getY());
    char boardChar = pBoard->getChar(p.getX(), p.getY());
    // Erase the barrel's current position, showing the board character
    if (pMario->getIsColored()) {
        std::string coloredBoardChar = "\033[0m" + std::string(1, boardChar) + "\033[0m"; // taken from gpt
        std::cout << coloredBoardChar;
    }
    else {
        std::cout << boardChar;
    }
}

// Function to reset the barrel's position
void Barrel::reset(int newX, int newY) {
    gotoxy(p.getX(), p.getY());
    std::cout << ' ';
    p.setX(newX);
    p.setY(newY);
    dir = { 0, 0 }; // Reset direction
    exploded = false; // Reset exploded state

    // Set initial direction based on directionRoll
    if (directionRoll == 1) {
        dir = { -1, 0 }; // Move left
    }
    else {
        dir = { 1, 0 }; // Move right
    }
    prevDir = dir; // Store the initial direction as the previous direction
}

// Function to set the starting position of the barrel
void Barrel::setStartPosition(Point p) {
    this->p = p;
    // You might want to reset other properties here as well, such as:
    exploded = false;
    countFall = 0;
}

// Function to check collision with Mario
bool Barrel::checkCollision() const {
    int marioX = pMario->getX();
    int marioY = pMario->getY();
    return ((p.getX() == marioX) && (p.getY() == marioY)) ||
        (p.getX() == marioX + pMario->getdirX() && p.getY() == marioY + pMario->getdirY()) ||
        (p.getX() + dir.x == marioX && p.getY() + dir.y == marioY);
}

// Function to check if the barrel is vulnerable to Mario's hammer
bool Barrel::checkVulnerable(int marioX, int marioY) const {
    return ((p.getX() == marioX + pMario->getDirHammerX() && p.getY() == marioY + pMario->getdirY()) ||
        (p.getX() == marioX + 2 * (pMario->getDirHammerX()) && p.getY() == marioY + pMario->getdirY()));
}

// Function to handle the barrel's explosion when it reaches a dangerous state
void Barrel::barrelExplode() {
    exploded = true; // Set the exploded flag
    int x = p.getX(); // Get the barrel's X position
    int y = p.getY(); // Get the barrel's Y position
    // Define the explosion frames
    const char* explosionFrames[] = {
        " x ",
        " X ",
        "   "
    };

    // Initialize the originalChars array
    char originalChars[3][3] = { {' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '} };

    // Save the original characters from the board
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int posX = x + j - 1;
            int posY = y + i - 1;
            if (posX >= 0 && posX < MAX_X && posY >= 0 && posY < MAX_Y) {
                originalChars[i][j] = pBoard->getChar(posX, posY);
            }
        }
    }

    // Show the explosion animation //Taken from gpt
    for (int frame = 0; frame < 2; ++frame) {
        gotoxy(x, y); // Move cursor to the barrel's position 
        std::cout << explosionFrames[frame]; // Print the explosion frame
        //std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Wait for 100 milliseconds
    }

    // Clear the explosion
    gotoxy(x, y);
    std::cout << explosionFrames[2]; // Clear the explosion frame

    // Restore the original characters
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            int posX = x + j - 1;
            int posY = y + i - 1;
            if (posX >= 0 && posX < MAX_X && posY >= 0 && posY < MAX_Y) {
                gotoxy(posX, posY);
                std::cout << originalChars[i][j];
            }
        }
    }
}
