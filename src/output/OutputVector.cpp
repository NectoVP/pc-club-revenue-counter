#include "OutputVector.h"

void OutputVector::outputLine(std::string&& line) {
    allLines.emplace_back(std::move(line));
}

void OutputVector::outputLine(const std::string& line) {
    allLines.push_back(line);
}