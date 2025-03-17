#pragma once
#include "GameStarter.h"
#include <filesystem>

class SimpleGame : public GameStarter
{
    SimpleGame(const SimpleGame&) = delete;
    SimpleGame& operator=(const SimpleGame&) = delete;
    bool isSilent = false;

public:
    SimpleGame() :GameStarter() {};
    virtual bool initialize(bool isSilent) override;
    virtual void run() override;
    void SetSilent(bool silent);
};
