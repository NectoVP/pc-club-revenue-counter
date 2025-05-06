#include "ClientEndOfDay.h"

void ClientEndOfDay::handleEvent(ParamBundle& paramBundle, const std::string& clientName, const std::string& current_time, uint64_t tableNumber) const {
    auto it = paramBundle.mapUserToTable.find(clientName);

    if(it->second.second != std::numeric_limits<uint64_t>::max())
        updateTableAfterLeaving(paramBundle, current_time, it->second.second, it->second.first);

    paramBundle.outputInterface->outputLine(current_time + " 11 " + clientName);
    paramBundle.mapUserToTable.erase(clientName);
}