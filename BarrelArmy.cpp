#include "BarrelArmy.h"
#include "Barrel.h"
#include <iostream>

// Update the state of the BarrelArmy
void BarrelArmy::update(Mario& mario) {
    if (!pMario) {
        pMario = &mario;
    }

    for (int i = 0; i < activeBarrels; ++i) {
        auto* barrel = static_cast<Barrel*>(barrels[i].get());
        barrel->erase();
        barrel->move();
        barrel->draw();
        barrel->FollowFloorDirection();

        if (barrel->checkCollision()) {
            handleBarrelCollision(i, mario);
            --i;
        }
        else if (barrel->isExploded()) {
            if (barrel->getCountFall() > 7) {
                handleBarrelExplosion(i, mario);
            }
            else {
                removeBarrel(i);
            }
            --i;
        }
    }

    if (activeBarrels < MAX_BARRELS && shouldDropNewBarrel()) {
        dropNewBarrel();
    }
}

// Handle collision between a barrel and Mario
void BarrelArmy::handleBarrelCollision(int index, Mario& mario) {
    auto* barrel = static_cast<Barrel*>(barrels[index].get());
    barrel->barrelExplode();
    mario.handleBarrel(barrel->getX(), barrel->getY());
    removeBarrel(index);
}

// Handle barrel explosion
void BarrelArmy::handleBarrelExplosion(int index, Mario& mario) {
    auto* barrel = static_cast<Barrel*>(barrels[index].get());
    if (abs(barrel->getX() - mario.getX()) <= 2 && abs(barrel->getY() - mario.getY()) <= 2) {
        mario.handleBarrel(barrel->getX(), barrel->getY());
    }
    removeBarrel(index);
}

// Remove a barrel from the army
void BarrelArmy::removeBarrel(int index) {
    if (index >= 0 && index < activeBarrels) {
        barrels[index] = std::move(barrels[activeBarrels - 1]);
        --activeBarrels;
    }
}

// Determine if a new barrel should be dropped
bool BarrelArmy::shouldDropNewBarrel() {
    barrelDrop++;
    return (barrelDrop % 13 == 0);
}

// Drop a new barrel
void BarrelArmy::dropNewBarrel() {
    if (activeBarrels < MAX_BARRELS) {
        auto newBarrel = std::make_unique<Barrel>(0, 0, *pBoard, *pMario, pMario->getIsColored(), random_seed);
        auto* barrel = newBarrel.get();
        if (barrel->getDirectionRoll() % 2 == 1) {
            barrel->reset(barrel->getP1().getX(), barrel->getP1().getY());
        }
        else {
            barrel->reset(barrel->getP2().getX(), barrel->getP2().getY());
        }
        barrel->draw();
        barrels[activeBarrels] = std::move(newBarrel);
        ++activeBarrels;
    }
}

// Set the game board for the BarrelArmy
void BarrelArmy::setBoard(Board& board) {
    pBoard = &board;
    for (auto& barrel : barrels) {
        if (barrel) {
            barrel->setBoard(board);
        }
    }
}

// Reset the BarrelArmy
void BarrelArmy::reset() {
    activeBarrels = 0;
}

// Remove a barrel at the specified position
bool BarrelArmy::removeBarrelAt(int x, int y, Point pointMario) {
    for (int i = 0; i < activeBarrels; ++i) {
        auto* barrel = static_cast<Barrel*>(barrels[i].get());
        if (barrel->getX() == x && barrel->getY() == y) {
            if (barrel->checkVulnerable(pointMario.getX(), pointMario.getY())) {
                gotoxy(x, y);
                char ch = pBoard->getChar(x, y);
                std::cout << ch;
                removeBarrel(i);
                return true;
            }
        }
    }
    return false;
}

// Set the Mario object for the BarrelArmy
void BarrelArmy::setMario(Mario& mario) {
    pMario = &mario;
    for (auto& barrel : barrels) {
        if (barrel) {
            barrel->setMario(mario);
        }
    }
}
