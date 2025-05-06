#include "ClientSit.h"

void ClientSit::handleEvent(ParamBundle& paramBundle, const std::string& clientName, const std::string& current_time, uint64_t tableNumber) const {
    auto it = paramBundle.mapUserToTable.find(clientName);
    
    if(it == paramBundle.mapUserToTable.end()) {
        paramBundle.outputInterface->outputLine(current_time + " 13 ClientUnknown");
        return;
    }
    
    if(it->second.second == tableNumber) { // пересели на то же самое место
        paramBundle.outputInterface->outputLine(current_time + " 13 PlaceIsBusy");
        return;
    }
    
    if(std::get<2>(paramBundle.tablesInfo[tableNumber]) == TableStatus::OCCUPIED) {
        paramBundle.outputInterface->outputLine(current_time + " 13 PlaceIsBusy");
        return;
    }
    
    if(it->second.second != std::numeric_limits<uint64_t>::max()) { // если клиент уже сидел
        u_int64_t currentTimeNumber = convert24HtoMinutes(current_time);
        u_int64_t totalSpendTime = currentTimeNumber - it->second.first;

        std::get<0>(paramBundle.tablesInfo[it->second.second]) += totalSpendTime;
        std::get<1>(paramBundle.tablesInfo[it->second.second]) += (totalSpendTime / 60 + 1) * paramBundle.hourCost;
        std::get<2>(paramBundle.tablesInfo[it->second.second]) = TableStatus::EMPTY;

        paramBundle.leftoverTables += 1;
    }

    it->second = {
        convert24HtoMinutes(current_time),
        tableNumber
    };

    std::get<2>(paramBundle.tablesInfo[tableNumber]) = TableStatus::OCCUPIED;
    paramBundle.leftoverTables -= 1;
}
