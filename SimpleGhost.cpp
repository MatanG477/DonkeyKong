#include "SimpleGhost.h"
#include "SuperGhost.h"
constexpr int MAX_X = 80; // Maximum width of the board
constexpr int MAX_Y = 25; // Maximum height of the board

void SimpleGhost::draw() const {
    gotoxy(p.getX(), p.getY());
    if (isColored) {
        std::cout << "\033[34mx\033[0m"; // Draw colored ghost
    }
    else {
        std::cout << 'x'; // Draw uncolored ghost
    }
    std::cout.flush();
}

// Function to move the ghost
void SimpleGhost::move() {
    isMoving++; // Increment move counter
    if ((isMoving % 5 == 0 || isMoving % 7 == 0)) {
        erase(); // Erase the ghost from its current position
        if (static_cast<float>(rand()) / RAND_MAX < 0.05) {
            changeDirection(); // Occasionally change direction
        }

        int newX = p.getX() + dir.getX(); // Calculate new X position

        // Check for out-of-bounds or falling conditions
        if (newX <= 0 || newX >= MAX_X - 1 || pBoard->getChar(newX, p.getY() + 1) == ' ') {
            changeDirection(); // Change direction if needed
            newX = p.getX() + dir.getX(); // Recalculate new X position
        }

        char nextChar = pBoard->getChar(newX, p.getY()); // Get the character at the new position
        if (nextChar != '<' && nextChar != '>' && nextChar != '=' && nextChar != 'Q' && nextChar != 'x' && nextChar != 'X') {
            if (previousChar != 'x' || previousChar != 'X') {
                gotoxy(p.getX(), p.getY());
                std::cout << previousChar; // Restore the previous character
            }

            // Update previousChar before moving
            previousChar = pBoard->getChar(newX, p.getY());
            p.setX(newX); // Update X position
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

