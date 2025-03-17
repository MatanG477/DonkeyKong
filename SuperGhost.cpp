#include "SuperGhost.h"

constexpr int MAX_X = 80; // Maximum width of the board
constexpr int MAX_Y = 25; // Maximum height of the board
constexpr char LADDER_CHAR = 'H';
constexpr char CEILING_CHAR1 = '>';
constexpr char CEILING_CHAR2 = '=';
constexpr char CEILING_CHAR3 = '<';


void SuperGhost::draw() const {
    gotoxy(p.getX(), p.getY());
    if (isColored) {
        std::cout << "\033[36mX\033[0m"; // Draw colored ghost
    }
    else {
        std::cout << 'X'; // Draw uncolored ghost
    }
    std::cout.flush();
}

// Function to move the ghost
void SuperGhost::move() {
    isMoving++; // Increment move counter
    if ((isMoving % 5 == 0 || isMoving % 7 == 0)) {
        erase(); // Erase the ghost from its current position
        if (rand() % 100 < 5) {
            changeDirection(); // Occasionally change direction
        }

        int newX = p.getX() + dir.getX(); // Calculate new X position
        int newY = p.getY() + dir.getY(); // Calculate new Y position
        // Check for out-of-bounds or falling conditions
        if (newX <= 0 || newX >= MAX_X - 1 || pBoard->getChar(newX, p.getY() + 1) == ' ') {
            changeDirection(); // Change direction if needed
            newX = p.getX() + dir.getX(); // Recalculate new X position
        }
        // Handle climbing logic
        if (isLadder() && (rand() % 100 < 50)) {
            newX = p.getX();
            isClimb = true; // Set climbing state to true
            dir = { 0, 0 };
        }
        if (isClimb && !isCeiling()) {
            dir.setX(0);
            dir.setY(-1); // Set direction to climb up
        }
        if (isClimb && isCeiling()) {
            newY = p.getY() - 2; // Adjust position if hitting ceiling while climbing
            dir.setY(0);
            dir.setX((rand() % 2 == 0) ? -1 : 1); // Randomly set initial direction
            isClimb = false; // Reset climbing state
        }

        char nextChar = pBoard->getChar(newX, p.getY()); // Get the character at the new position
        if (nextChar != '<' && nextChar != '>' && nextChar != '=' && nextChar != 'Q' && nextChar != 'x' && nextChar != 'X') {
            if (previousChar != 'x' || previousChar != 'X') {
                gotoxy(p.getX(), p.getY());
                std::cout << previousChar; // Restore the previous character
            }

            // Update previousChar before moving
            previousChar = pBoard->getChar(newX, newY);
            p.setX(newX); // Update X position
            p.setY(newY); // Update Y position
        }
        else {
            changeDirection(); // Change direction if the next character is not walkable
        }

        draw(); // Draw the ghost at its new position
    }

    if (isMoving > 11) {
        isMoving = 1; // Reset move counter after a certain threshold
    }
}

bool SuperGhost::isLadder() const {
    return pBoard->getChar(p.getX(), p.getY() + dir.getY()) == LADDER_CHAR;
}

bool SuperGhost::isCeiling() const {
    char ch = pBoard->getChar(p.getX(), p.getY() - 1);
    return (ch == CEILING_CHAR1 || ch == CEILING_CHAR2 || ch == CEILING_CHAR3);
}

