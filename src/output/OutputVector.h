#pragma once

#include <vector>

#include "OutputInterface.h"

class OutputVector : public OutputInterface {
public:
    void outputLine(std::string&& line) override;
    void outputLine(const std::string& line) override;

    const std::vector<std::string>& getAllLines() {
        return allLines;
    }

    std::vector<std::string> allLines;
};