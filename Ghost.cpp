#include "Ghost.h"
#include "utils.h"
#include <random>
#include <cmath>
#include "SuperGhost.h"

constexpr int MAX_X = 80;
constexpr int MAX_Y = 25;

Ghost::Ghost(int startX, int startY, bool isSuper, Board& board, Mario& mario, bool colored, long seed)
    : Enemy(startX, startY, board, mario, colored), random_seed(seed), IsSuper(isSuper) {

    dir.setX((rand() % 2 == 0) ? -1 : 1);
    dir.setY(0);
}

bool Ghost::checkCollision() const {
    int marioX = pMario->getX();
    int marioY = pMario->getY();
    return (std::abs(p.getX() - marioX) < 2 && std::abs(p.getY() - marioY) < 2);
}

void Ghost::changeDirection() {
    dir.setX(dir.getX() * -1);
    previousChar = pBoard->getChar(p.getX(), p.getY());
}

bool Ghost::checkCollisionWithGhost(const Ghost& other) const {
    return (std::abs(p.getX() - other.p.getX()) < 2 && std::abs(p.getY() - other.p.getY()) < 2);
}

bool Ghost::checkVulnerable(int marioX, int marioY) const {
    return ((p.getX() == marioX + pMario->getDirHammerX() && p.getY() == marioY + pMario->getdirY()) ||
        (p.getX() == marioX + 2 * (pMario->getDirHammerX()) && p.getY() == marioY + pMario->getdirY()));
}

void Ghost::moveApart(Ghost& other) {
    // Move the ghosts apart
    if (p.getX() < other.p.getX()) {
        moveLeft();
        other.moveRight();
    }
    else {
        moveRight();
        other.moveLeft();
    }
}

void Ghost::moveLeft() {
    if (p.getX() > 0 && pBoard->getChar(p.getX() - 1, p.getY()) == ' ') {
        erase();
        p.setX(p.getX() - 1);
        draw();
    }
}

void Ghost::moveRight() {
    if (p.getX() < MAX_X - 1 && pBoard->getChar(p.getX() + 1, p.getY()) == ' ') {
        erase();
        p.setX(p.getX() + 1);
        draw();
    }
}
