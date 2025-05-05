#include "ClientWait.h"

void ClientWait::handleEvent(ParamBundle& paramBundle, std::string clientName, std::string_view current_time, uint64_t tableNumber) const {
        
    if(paramBundle.leftoverTables > 0) {
        std::cout << current_time << " 13 ICanWaitNoLonger!\n";
        return;
    }

    if(paramBundle.clientDeque.size() > paramBundle.tablesInfo.size() - 1) {
        paramBundle.mapUserToTable.erase(clientName);
        std::cout << current_time << " 11 " << clientName << '\n';
        return;
    }

    paramBundle.clientDeque.emplace_back(std::move(clientName));
}