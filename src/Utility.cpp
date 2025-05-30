#include "Utility.h"

std::vector<std::string> getWords(std::string_view text) {
    std::vector<std::string> words;
    size_t start = text.find_first_not_of(' ');

    while (start != std::string_view::npos) {
        size_t end = text.find_first_of(' ', start + 1);
        if (end == std::string_view::npos)
            end = text.length();
        words.emplace_back(text.substr(start, end - start));
        start = text.find_first_not_of(' ', end + 1);
    }
    return words;
}