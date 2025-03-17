#pragma once
#include <vector>
#include <memory>
#include "Enemy.h"
#include "Board.h"
#include "Mario.h"
#include "Point.h"

class GhostManager {
private:
    std::vector<std::unique_ptr<Enemy>> ghosts;
    Board* pBoard;
    Mario* pMario;
    bool isColored;
    bool IsSilent;
    long random_seed;

    void handleGhostCollision(int index);
    void resolveCollision(Enemy& ghost1, Enemy& ghost2);

public:
    GhostManager(Board& board, Mario& mario, bool colored);
    void initializeGhostsFromBoard();
    bool removeGhostAt(int x, int y, Point pointMario);
    void update();
    void reset();
    void setColored(bool colored);
    void setRandomSeed(long seed) {
        random_seed = seed;
        for (auto& ghost : ghosts) {
            if (ghost) {
                ghost->setRandomSeed(seed);
            }
        }
    }
    
    
};