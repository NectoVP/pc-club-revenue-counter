#include "ClientArrive.h"

void ClientArrive::handleEvent(ParamBundle& paramBundle, std::string clientName, std::string_view current_time, uint64_t tableNumber) const {
        
    if(paramBundle.mapUserToTable.find(clientName) != paramBundle.mapUserToTable.end()) {
        std::cout << current_time << " 13 YouShallNotPass\n";
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