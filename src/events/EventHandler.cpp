#include "EventHandler.h"

int atoiStringView(std::string_view str) {
    int val = 0;
    for(int i = 0; i < str.size(); ++i)
        val = val*10 + (str[i] - 48);
    return val;
}

size_t convert24HtoMinutes(const std::string& str) {
    return std::stoi(str.substr(0, 2)) * 60 + std::stoi(str.substr(3));
}

size_t convert24HtoMinutes(std::string_view str) {
    return atoiStringView(str.substr(0, 2)) * 60 + atoiStringView(str.substr(3));
}

std::string convertMinutesTo24H(u_int64_t minutes) {
    std::string hours = std::to_string(minutes / 60);
    if(hours.size() == 1)
        hours = "0" + hours;
    
    std::string min = std::to_string(minutes % 60);
    if(min.size() == 1)
        min = "0" + min;

    return hours + ":" + min; 
};