#include "GameStarter.h"
#include "ManualGame.h"
#include "AutoGame.h"
#include "SimpleGame.h"
#include <string>
#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {
    try {
        bool isLoad = argc > 1 && std::string(argv[1]) == "-load";
        bool isSilent = isLoad && argc > 2 && std::string(argv[2]) == "-silent";
        GameStarter* game = nullptr;
        if (isLoad) {
            game = new AutoGame();
            static_cast<AutoGame*>(game)->SetSilent(isSilent);
        }
        else if (argc == 1) {
            game = new SimpleGame();
            static_cast<SimpleGame*>(game)->SetSilent(isSilent);
        }
        else {
            game = new ManualGame();
            static_cast<ManualGame*>(game)->SetSilent(isSilent);
        }

        if (!game->initialize(isSilent)) {
            throw std::runtime_error("Failed to initialize game.");
        }

        delete game;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
