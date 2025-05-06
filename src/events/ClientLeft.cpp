#include "ClientLeft.h"

void ClientLeft::handleEvent(ParamBundle& paramBundle, const std::string& clientName, const std::string& current_time, uint64_t tableNumber) const {
    auto it = paramBundle.mapUserToTable.find(clientName);

    if(it == paramBundle.mapUserToTable.end()) {
        paramBundle.outputInterface->outputLine(current_time + " 13 ClientUnknown");
        return;
    }

    if(it->second.second == std::numeric_limits<uint64_t>::max()) { // если клиент не сидел    
        paramBundle.mapUserToTable.erase(clientName);
        return;
    }

    updateTableAfterLeaving(paramBundle, current_time, it->second.second, it->second.first);

    if(paramBundle.clientDeque.size() > 0) {
        std::string newClientName = paramBundle.clientDeque.front();
        paramBundle.mapUserToTable[newClientName] = {convert24HtoMinutes(current_time),it->second.second};
    
        std::get<2>(paramBundle.tablesInfo[it->second.second]) = TableStatus::OCCUPIED;

        paramBundle.leftoverTables -= 1;
        paramBundle.clientDeque.pop_front();

        paramBundle.outputInterface->outputLine(current_time + " 12 " + newClientName + " " + std::to_string(it->second.second));
    }

    paramBundle.mapUserToTable.erase(clientName);
}