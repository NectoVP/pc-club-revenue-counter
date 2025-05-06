#include "ClientLeft.h"

void ClientLeft::handleEvent(ParamBundle& paramBundle, const std::string& clientName, const std::string& current_time, uint64_t tableNumber) const {
    auto it = paramBundle.mapUserToTable.find(clientName);

    if(it == paramBundle.mapUserToTable.end()) {
        paramBundle.outputInterface->outputLine(current_time + " 13 ClientUnknown");
        return;
    }

    u_int64_t currentTimeNumber = convert24HtoMinutes(current_time);
    u_int64_t totalSpendTime = currentTimeNumber - it->second.first;

    std::get<0>(paramBundle.tablesInfo[it->second.second]) += totalSpendTime;
    std::get<1>(paramBundle.tablesInfo[it->second.second]) += (totalSpendTime / 60 + 1) * paramBundle.hourCost;
    std::get<2>(paramBundle.tablesInfo[it->second.second]) = TableStatus::EMPTY;
    paramBundle.leftoverTables += 1;

    if(paramBundle.clientDeque.size() > 0) {
        std::string newClientName = paramBundle.clientDeque.front();
        paramBundle.mapUserToTable[newClientName] = {currentTimeNumber,it->second.second};
    
        std::get<2>(paramBundle.tablesInfo[it->second.second]) = TableStatus::OCCUPIED;

        paramBundle.leftoverTables -= 1;
        paramBundle.clientDeque.pop_front();

        paramBundle.outputInterface->outputLine(current_time + " 12 " + newClientName + " " + std::to_string(it->second.second));
    }

    paramBundle.mapUserToTable.erase(clientName);
}