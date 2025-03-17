#pragma once
#include "GameStarter.h"

class ManualGame : public GameStarter {
    void deleteExistingFiles();
    bool isSilent;


public:
    ManualGame();
    virtual bool initialize(bool isSilent) override;
    virtual void run() override;
    void SetSilent(bool silent);
};