// Enemy.h
#pragma once
#include "Board.h"
#include "Mario.h"
#include "Point.h"

class Enemy {
protected:
    long random_seed = 0;
    int isMoving = 1; // Flag to indicate if the enemy is moving
    Point p; // Position of the enemy
    Board* pBoard; // Pointer to the game board
    Mario* pMario; // Pointer to the Mario object
    char previousChar; // Character previously at the enemy's position
    bool isColored; // Flag to determine if the enemy is colored

public:
    // Constructor to initialize the enemy
    Enemy(int startX, int startY, Board& board, Mario& mario, bool colored);
    virtual ~Enemy() = default; // Virtual destructor
    // Pure virtual method to move the enemy
    virtual void move() = 0;
    // Pure virtual method to draw the enemy
    virtual void draw() const = 0;
    // Virtual method to erase the enemy
    virtual void erase() const;
    // Pure virtual method to check collision with Mario
    virtual bool checkCollision() const = 0;
    // Pure virtual method to check if the enemy is vulnerable to Mario
    virtual bool checkVulnerable(int marioX, int marioY) const = 0;

    // Getter for the X position of the enemy
    int getX() const { return p.getX(); }
    // Getter for the Y position of the enemy
    int getY() const { return p.getY(); }
    // Set the colored flag
    void setColored(bool colored) { isColored = colored; }
    // Pure virtual method to set the game board
    virtual void setBoard(Board& board) = 0;
    // Pure virtual method to set the Mario object
    virtual void setMario(Mario& mario) = 0;
    virtual void setRandomSeed(long seed) = 0;
};
