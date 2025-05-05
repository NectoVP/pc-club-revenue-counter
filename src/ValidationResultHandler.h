#pragma once

#include <string>

enum ValidationStatus {
    FAILED_TO_OPEN,
    INVALID_FORMAT,
    CORRECT
};


class ValidationResultHandler {
public:
    explicit ValidationResultHandler(ValidationStatus status, std::string errorString = "") noexcept;
    explicit ValidationResultHandler(ValidationStatus status, std::string_view errorString) noexcept;

    ValidationStatus getStatus() const noexcept {
        return status;
    }

    std::string getErrorString() const noexcept {
        return errorString;
    }

private:
    ValidationStatus status;
    std::string errorString;
};