#include "ClientWait.h"

void ClientWait::handleEvent(ParamBundle& paramBundle, const std::string& clientName, const std::string& current_time, uint64_t tableNumber) const {
    auto it = paramBundle.mapUserToTable.find(clientName);
    
    if(it == paramBundle.mapUserToTable.end()) {
        paramBundle.outputInterface->outputLine(current_time + " 13 ClientUnknown");
        return;
    }
    
    if(paramBundle.leftoverTables > 0) {
        paramBundle.outputInterface->outputLine(current_time + " 13 ICanWaitNoLonger!");
        return;
    }

    if(paramBundle.clientDeque.size() > paramBundle.tablesInfo.size() - 1) {
        paramBundle.mapUserToTable.erase(clientName);
        paramBundle.outputInterface->outputLine(current_time + " 11 " + clientName);
        return;
    }

    paramBundle.clientDeque.emplace_back(std::move(clientName));
}