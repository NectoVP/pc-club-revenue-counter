#pragma once

#include <iostream>

#include "OutputInterface.h"

class OutputConsole : public OutputInterface {
public:
    void outputLine(std::string&& line) override;
    void outputLine(const std::string& line) override;
};