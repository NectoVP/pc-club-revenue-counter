#include "ClientArrive.h"

void ClientArrive::handleEvent(ParamBundle& paramBundle, const std::string& clientName, const std::string& current_time, uint64_t tableNumber) const {
        
    if(paramBundle.mapUserToTable.find(clientName) != paramBundle.mapUserToTable.end()) {
        paramBundle.outputInterface->outputLine(current_time + " 13 YouShallNotPass");
        return;
    }
    
    paramBundle.mapUserToTable.insert({
        std::move(clientName), 
        {
            convert24HtoMinutes(current_time),
            std::numeric_limits<uint64_t>::max()
        }
    });
}