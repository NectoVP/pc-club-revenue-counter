#include "OutputConsole.h"

void OutputConsole::outputLine(std::string&& line) {
    std::cout << line << '\n';
}

void OutputConsole::outputLine(const std::string& line)  {
    std::cout << line << '\n';
}