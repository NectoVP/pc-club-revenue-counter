#include "ClientLeft.h"

void ClientLeft::handleEvent(ParamBundle& paramBundle, std::string clientName, std::string_view current_time, uint64_t tableNumber) const {
    auto it = paramBundle.mapUserToTable.find(clientName);

    if(it == paramBundle.mapUserToTable.end()) {
        std::cout << current_time << " 13 ClientUnknown";
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

        std::cout << current_time << " 12 " << newClientName << ' ' << it->second.second << '\n';
    }

    paramBundle.mapUserToTable.erase(clientName);
}