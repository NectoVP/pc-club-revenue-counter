#pragma once

#include <string>

class OutputInterface {
public:
    virtual void outputLine(std::string&& line) = 0;
    virtual void outputLine(const std::string& line) = 0;
};