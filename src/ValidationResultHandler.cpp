#include "ValidationResultHandler.h"

ValidationResultHandler::ValidationResultHandler(ValidationStatus status, std::string errorString) noexcept
    : status(status)
    , errorString(errorString) {

}

ValidationResultHandler::ValidationResultHandler(ValidationStatus status, std::string_view errorString) noexcept
    : status(status)
    , errorString(errorString) {
}