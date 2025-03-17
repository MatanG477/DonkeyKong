#pragma once
#include "GameStarter.h"
#include <string>
#include <vector>
class AutoGame : public GameStarter {
    bool isSilent;
    std::vector<std::string> screenFiles;
    std::streambuf* original_buf;

public:
    AutoGame();
    void reportResultError(const std::string& message, const std::string& filename, size_t iteration);
    virtual bool initialize(bool isSilent) override;
    virtual void run() override;
    bool GetSilent();
    void SetSilent(bool silent);
    
};