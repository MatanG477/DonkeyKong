#pragma once
#include "Enemy.h"

class SuperGhost;

class Ghost : public Enemy {
protected:
    long random_seed;
    Point dir;
    bool IsSuper;
public:
    Ghost(int startX, int startY, bool isSuper, Board& board, Mario& mario, bool colored, long seed);
    void setRandomSeed(long seed) override {
        random_seed = seed;
        srand(random_seed);
    }

    virtual void move() = 0;
    virtual void draw() const = 0;
    bool checkCollision() const override;
    bool checkVulnerable(int marioX, int marioY) const override;
    void setBoard(Board& board) override { pBoard = &board; }
    void setMario(Mario& mario) override { pMario = &mario; }
    void changeDirection();
    bool checkCollisionWithGhost(const Ghost& other) const;
    void moveApart(Ghost& other);
    // New methods
    void moveLeft();
    void moveRight();
};
