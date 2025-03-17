#pragma once
#include "Enemy.h"

class Barrel : public Enemy {
private:
    long random_seed;
    int directionRoll;
    bool exploded = false;
    int countFall = 0;
    struct Direction { int x, y; };
    Direction dir{ 0, 0 };
    Direction prevDir = { 0, 0 };
    Point p1, p2;
    int DonkeySTARTX, DonkeySTARTY;
    char ch = 'O';

public:
    Barrel(int startX, int startY, Board& board, Mario& mario, bool colored, long seed);
    void setStartPosition(Point p);
    void setRandomSeed(long seed) override {
        random_seed = seed;
        srand(random_seed);
    }
    void move() override;
    void draw() const override;
    bool checkCollision() const override;
    bool checkVulnerable(int marioX, int marioY) const override;
    void setBoard(Board& board) override { pBoard = &board; }
    void setMario(Mario& mario) { pMario = &mario; }
    void erase() const;
    void FollowFloorDirection();
    void barrelExplode();
    bool isExploded() const { return exploded; }
    int getCountFall() const { return countFall; }
    void reset(int newX, int newY);
    int getDirectionRoll() const { return directionRoll; }
    Point getP1() const { return p1; }
    Point getP2() const { return p2; }
};