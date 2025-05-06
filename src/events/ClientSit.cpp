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
    
    if(it->second.second != std::numeric_limits<uint64_t>::max()) // если клиент уже сидел
        updateTableAfterLeaving(paramBundle, current_time, it->second.second, it->second.first);

    it->second = {
        convert24HtoMinutes(current_time),
        tableNumber
    };

    std::get<2>(paramBundle.tablesInfo[tableNumber]) = TableStatus::OCCUPIED;
    paramBundle.leftoverTables -= 1;
}
