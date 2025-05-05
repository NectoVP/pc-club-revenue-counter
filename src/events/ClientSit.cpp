#include "ClientSit.h"

void ClientSit::handleEvent(ParamBundle& paramBundle, std::string clientName, std::string_view current_time, uint64_t tableNumber) const {
    auto it = paramBundle.mapUserToTable.find(clientName);
    
    if(it == paramBundle.mapUserToTable.end()) {
        std::cout << current_time << " 13 ClientUnknown\n";
        return;
    }
    
    if(it->second.second == tableNumber) { // пересели на то же самое место
        std::cout << current_time << " 13 PlaceIsBusy\n";
        return;
    }
    
    if(std::get<2>(paramBundle.tablesInfo[tableNumber]) == TableStatus::OCCUPIED) {
        std::cout << current_time << " 13 PlaceIsBusy\n";
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
