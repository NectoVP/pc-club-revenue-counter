#pragma once

#include <fstream>
#include <iostream>
#include <unordered_set>

#include "ValidationResultHandler.h"
#include "Utility.h"

class Validator {
public:
    explicit Validator(const std::string& filePath) noexcept;

    ValidationResultHandler validateFile() const;

private:
    ValidationResultHandler validateHeader(std::ifstream& file) const;
    ValidationResultHandler validateBody(std::ifstream& file) const;
    
    ValidationResultHandler validateUnsignLongLong(std::string_view str) const;
    ValidationResultHandler validateTime(std::string_view str) const;
    ValidationResultHandler validate24HFormat(std::string_view str) const;
    ValidationResultHandler validate60MFormat(std::string_view str) const;
    ValidationResultHandler validateClientName(std::string_view str) const;

    std::unordered_set<u_int64_t> possible_events = {1, 2, 3, 4};
    mutable u_int64_t tableCount;
    std::string filePath;
};