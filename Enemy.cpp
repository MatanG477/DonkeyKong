#include "Enemy.h"
#include "utils.h"

// Constructor to initialize the enemy
Enemy::Enemy(int startX, int startY, Board& board, Mario& mario, bool colored)
    : p(startX, startY), pBoard(&board), pMario(&mario), isColored(colored) {
    previousChar = pBoard->getChar(p.getX(), p.getY()); // Store the character at the enemy's initial position
}

// Function to erase the enemy from the board
void Enemy::erase() const {
    gotoxy(p.getX(), p.getY()); // Move cursor to the enemy's position
    std::cout << previousChar; // Restore the previous character at the enemy's position
    std::cout.flush(); // Flush the output buffer
}