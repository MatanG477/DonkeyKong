#pragma once
#include "Barrel.h"
#include <array>
#include <memory>

class BarrelArmy {
private:
    static constexpr int MAX_BARRELS = 5;
    std::array<std::unique_ptr<Enemy>, MAX_BARRELS> barrels;
    int activeBarrels = 0;
    Board* pBoard = nullptr;
    Mario* pMario = nullptr;
    int barrelDrop = 1;
    long random_seed = 0;
	bool IsSilent = false;

    void removeBarrel(int index);
    bool shouldDropNewBarrel();
    void dropNewBarrel();
    void handleBarrelCollision(int index, Mario& mario);
    void handleBarrelExplosion(int index, Mario& mario);

public:
    BarrelArmy() = default;
    void setBoard(Board& board);
    void reset();
    void setMario(Mario& mario);
    bool removeBarrelAt(int x, int y, Point pointMario);
    void update(Mario& mario);
    void setRandomSeed(long seed) {
        random_seed = seed;
        for (auto& barrel : barrels) {
            if (barrel) {
                barrel->setRandomSeed(seed);
            }
        }
    }
    
};