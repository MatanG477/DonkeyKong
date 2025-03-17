#include "GhostManager.h"
#include "Ghost.h"
#include "SimpleGhost.h"
#include "SuperGhost.h"

GhostManager::GhostManager(Board& board, Mario& mario, bool colored)
    : pBoard(&board), pMario(&mario), isColored(colored) {
    initializeGhostsFromBoard();
}

void GhostManager::initializeGhostsFromBoard() {
    ghosts.clear();
    const auto& locations = pBoard->getGhostLocations();
    for (const auto& loc : locations) {
        if (std::get<2>(loc)) {
            ghosts.push_back(std::make_unique<SuperGhost>(
                std::get<0>(loc), std::get<1>(loc), *pBoard, *pMario, isColored, random_seed));
        }
        else {
            ghosts.push_back(std::make_unique<SimpleGhost>(
                std::get<0>(loc), std::get<1>(loc), *pBoard, *pMario, isColored, random_seed));
        }
    }
}

void GhostManager::update() {
    for (int i = 0; i < ghosts.size(); ++i) {
        ghosts[i]->move();
        ghosts[i]->draw();

        // Check collisions with other ghosts
        for (int j = i + 1; j < ghosts.size(); ++j) {
            auto* ghost1 = dynamic_cast<Ghost*>(ghosts[i].get());
            auto* ghost2 = dynamic_cast<Ghost*>(ghosts[j].get());
            if (ghost1 && ghost2 && ghost1->checkCollisionWithGhost(*ghost2)) {
                if (SuperGhost* superGhost1 = dynamic_cast<SuperGhost*>(ghost1)) {
                    superGhost1->SetIsClimb(false);
                }
                if (SuperGhost* superGhost2 = dynamic_cast<SuperGhost*>(ghost2)) {
                    superGhost2->SetIsClimb(false);
                }
                resolveCollision(*ghost1, *ghost2);
            }
        }

        if (ghosts[i]->checkCollision()) {
            handleGhostCollision(i);
        }
    }
}

void GhostManager::handleGhostCollision(int index) {
    pMario->handleGhost(ghosts[index]->getX(), ghosts[index]->getY());
}

void GhostManager::resolveCollision(Enemy& ghost1, Enemy& ghost2) {
    auto* g1 = dynamic_cast<Ghost*>(&ghost1);
    auto* g2 = dynamic_cast<Ghost*>(&ghost2);
    if (g1 && g2) {
        g1->moveApart(*g2);
        g1->changeDirection();
        g2->changeDirection();
    }
}

bool GhostManager::removeGhostAt(int x, int y, Point pointMario) {
    for (auto it = ghosts.begin(); it != ghosts.end(); ++it) {
        if ((*it)->getX() == x && (*it)->getY() == y) {
            if ((*it)->checkVulnerable(pointMario.getX(), pointMario.getY())) {
                gotoxy(x, y);
                char ch = pBoard->getChar(x, y);
                std::cout << ch;
                ghosts.erase(it);
                return true;
            }
        }
    }
    return false;
}

void GhostManager::reset() {
    initializeGhostsFromBoard();
}

void GhostManager::setColored(bool colored) {
    isColored = colored;
    for (auto& ghost : ghosts) {
        ghost->setColored(colored);
    }
}
