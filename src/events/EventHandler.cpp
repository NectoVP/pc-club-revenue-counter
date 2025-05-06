#include "EventHandler.h"

void updateTableAfterLeaving(ParamBundle& paramBundle, const std::string& currentTime, u_int64_t oldChairNumber, u_int64_t oldTime) {
    u_int64_t currentTimeNumber = convert24HtoMinutes(currentTime);
    u_int64_t totalSpendTime = currentTimeNumber - oldTime;

    std::get<0>(paramBundle.tablesInfo[oldChairNumber]) += totalSpendTime;
    std::get<1>(paramBundle.tablesInfo[oldChairNumber]) += (totalSpendTime / 60 + (totalSpendTime % 60 != 0)) * paramBundle.hourCost;
    std::get<2>(paramBundle.tablesInfo[oldChairNumber]) = TableStatus::EMPTY;

    paramBundle.leftoverTables += 1;
}

size_t convert24HtoMinutes(const std::string& str) {
    return std::stoi(str.substr(0, 2)) * 60 + std::stoi(str.substr(3));
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