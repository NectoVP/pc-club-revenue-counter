#include "Validator.h"

Validator::Validator(const std::string& filePath) noexcept : filePath(filePath) {    }

ValidationResultHandler Validator::validateFile() const {
    std::ifstream file(filePath);
    if(!file.is_open())
        return ValidationResultHandler(ValidationStatus::FAILED_TO_OPEN);

    auto result = validateHeader(file);
    if(result.getStatus() != ValidationStatus::CORRECT) {
        file.close();
        return result;
    }
    
    result = validateBody(file);
    if(result.getStatus() != ValidationStatus::CORRECT) {
        file.close();
        return result;
    }
    
    file.close();
    return ValidationResultHandler(ValidationStatus::CORRECT);
}

ValidationResultHandler Validator::validateHeader(std::ifstream& file) const {
    std::string line;

    try {
        std::getline(file, line);
        auto result = validateUnsignLongLong(line);
        if(result.getStatus() != ValidationStatus::CORRECT)
            return result;
        tableCount = std::stoull(line);
    } catch(std::exception exception) {
        return ValidationResultHandler(ValidationStatus::INVALID_FORMAT, line);    
    }
     
    try {
        std::getline(file, line);
        if(line.size() != 11 || line.find(' ') == std::string::npos)
            return ValidationResultHandler(ValidationStatus::INVALID_FORMAT, line);
        if(validateTime(line.substr(0, 5)).getStatus() == ValidationStatus::INVALID_FORMAT
            || validateTime(line.substr(6)).getStatus() == ValidationStatus::INVALID_FORMAT)
            return ValidationResultHandler(ValidationStatus::INVALID_FORMAT, line);        
    } catch(std::exception exception) {
        return ValidationResultHandler(ValidationStatus::INVALID_FORMAT, line);    
    }

    try {
        std::getline(file, line);
        auto result = validateUnsignLongLong(line);
        if(result.getStatus() != ValidationStatus::CORRECT)
            return result;
    } catch(std::exception exception) {
        return ValidationResultHandler(ValidationStatus::INVALID_FORMAT, line);    
    }

    return ValidationResultHandler(ValidationStatus::CORRECT);
}

ValidationResultHandler Validator::validateUnsignLongLong(std::string_view str) const {
    try {
        if(str.empty())
            return ValidationResultHandler(ValidationStatus::INVALID_FORMAT, str);
    
        for(int i = 0; i < str.size(); ++i) {
            if(!isdigit(str[i]))
                return ValidationResultHandler(ValidationStatus::INVALID_FORMAT, str);
        }

        if(std::stoull(std::string(str)) == 0)
           return ValidationResultHandler(ValidationStatus::INVALID_FORMAT, str);
    }
    catch(std::exception exception) {
        return ValidationResultHandler(ValidationStatus::INVALID_FORMAT, str);    
    }

    return ValidationResultHandler(ValidationStatus::CORRECT);
}

ValidationResultHandler Validator::validateTime(std::string_view str) const {
    if(str.size() != 5)
        return ValidationResultHandler(ValidationStatus::INVALID_FORMAT);

    if(str.find(':') == std::string::npos)
        return ValidationResultHandler(ValidationStatus::INVALID_FORMAT);

    if(validate24HFormat(str.substr(0, 2)).getStatus() == ValidationStatus::INVALID_FORMAT 
        || validate60MFormat(str.substr(3)).getStatus() == ValidationStatus::INVALID_FORMAT)    
        return ValidationResultHandler(ValidationStatus::INVALID_FORMAT);
    
    return ValidationResultHandler(ValidationStatus::CORRECT);
}

ValidationResultHandler Validator::validate24HFormat(std::string_view str) const {
    if(!std::isdigit(str[0]) || !std::isdigit(str[1]))
        return ValidationResultHandler(ValidationStatus::INVALID_FORMAT);
    
    if(std::stoi(std::string(str)) >= 24)
        return ValidationResultHandler(ValidationStatus::INVALID_FORMAT);
    
    return ValidationResultHandler(ValidationStatus::CORRECT);
}

ValidationResultHandler Validator::validate60MFormat(std::string_view str) const {
    if(!std::isdigit(str[0]) || !std::isdigit(str[1]))
        return ValidationResultHandler(ValidationStatus::INVALID_FORMAT);
    
    if(std::stoi(std::string(str)) >= 60)
        return ValidationResultHandler(ValidationStatus::INVALID_FORMAT);
    
    return ValidationResultHandler(ValidationStatus::CORRECT);
}

ValidationResultHandler Validator::validateClientName(std::string_view str) const {
    for(int i = 0; i < str.size(); ++i) {
        if(std::isdigit(str[i]) || std::islower(str[i]) || str[i] == '-' || str[i] == '_')
            continue;
        else
            return ValidationResultHandler(ValidationStatus::INVALID_FORMAT, str);
    }
    return ValidationResultHandler(ValidationStatus::CORRECT);
}

ValidationResultHandler Validator::validateBody(std::ifstream& file) const {
    std::string line;
    
    bool seenEmptyLine = false;
    while(std::getline(file, line)) {
        if(line.empty()) {
            seenEmptyLine = true;
            continue;
        }

        if(seenEmptyLine)
            return ValidationResultHandler(ValidationStatus::INVALID_FORMAT, line);

        auto words = getWords(line);
        
        if(words.size() < 3)
            return ValidationResultHandler(ValidationStatus::INVALID_FORMAT, line);
        
        if(validateTime(words[0]).getStatus() == ValidationStatus::INVALID_FORMAT)
            return ValidationResultHandler(ValidationStatus::INVALID_FORMAT, line);
        if(validateUnsignLongLong(words[1]).getStatus() == ValidationStatus::INVALID_FORMAT)
            return ValidationResultHandler(ValidationStatus::INVALID_FORMAT, line);
        if(validateClientName(words[2]).getStatus() == ValidationStatus::INVALID_FORMAT)
            return ValidationResultHandler(ValidationStatus::INVALID_FORMAT, line);
        
        if(possible_events.find(std::stoull(std::string(words[1]))) == possible_events.end())
            return ValidationResultHandler(ValidationStatus::INVALID_FORMAT, line);

        if(words.size() == 3) {
            continue;
        }
        if(words.size() == 4 && validateUnsignLongLong(words[3]).getStatus() == ValidationStatus::CORRECT) {
            if(std::stoul(std::string(words[3])) > tableCount)
                return ValidationResultHandler(ValidationStatus::INVALID_FORMAT, line);
            continue;
        }
        
        return ValidationResultHandler(ValidationStatus::INVALID_FORMAT, line);
    }
    return ValidationResultHandler(ValidationStatus::CORRECT);
}