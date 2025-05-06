#include "RevenueCounter.h"

#include "output/OutputConsole.h"

RevenueCounter::RevenueCounter(const std::string& filePath, const std::shared_ptr<OutputInterface>& outputInterface) 
    : filePath(filePath)
    , paramBundle(outputInterface) {
    
    events.emplace(1, std::make_unique<ClientArrive>());
    events.emplace(2, std::make_unique<ClientSit>());
    events.emplace(3, std::make_unique<ClientWait>());
    events.emplace(4, std::make_unique<ClientLeft>());
    events.emplace(5, std::make_unique<ClientEndOfDay>());
}

RevenueCounter::RevenueCounter(const std::string& filePath) : RevenueCounter(filePath, std::make_unique<OutputConsole>()) {
}

void RevenueCounter::calculateRevenue() {
    std::ifstream file(filePath);

    if(!file.is_open()) {
        paramBundle.outputInterface->outputLine("File was not opened!");
        return;
    }

    init(file);

    paramBundle.outputInterface->outputLine(startTimeStr);

    parseEvents(file);

    std::vector<std::string> leftoverClients;
    for (const auto& pair : paramBundle.mapUserToTable)
        leftoverClients.push_back(pair.first);
    std::sort(leftoverClients.begin(), leftoverClients.end());
    
    for (const auto& key : leftoverClients)
        events[5]->handleEvent(paramBundle, key, endTimeStr, 0);
    
    paramBundle.outputInterface->outputLine(endTimeStr);

    for(int i = 1; i <= tableCount; ++i)
        paramBundle.outputInterface->outputLine(
            std::to_string(i)
            + " " 
            + std::to_string(std::get<1>(paramBundle.tablesInfo[i]))
            + " "
            + convertMinutesTo24H(std::get<0>(paramBundle.tablesInfo[i])));

    file.close();
}

void RevenueCounter::init(std::ifstream& file) {
    std::string line;

    std::getline(file, line);
    tableCount = std::stoull(line);
    paramBundle.tablesInfo.resize(tableCount + 1);
    paramBundle.leftoverTables = tableCount;

    std::getline(file, line);
    startTimeStr = line.substr(0, 5);
    endTimeStr = line.substr(6);
    startTime = convert24HtoMinutes(startTimeStr);
    endTime = convert24HtoMinutes(endTimeStr);

    std::getline(file, line);    
    paramBundle.hourCost = std::stoull(line);
}


void RevenueCounter::parseEvents(std::ifstream& file) {
    std::string line;                                                                                                       
    while(std::getline(file, line)) {
        if(line.empty())
            return;
        paramBundle.outputInterface->outputLine(line);
        
        auto words = getWords(line);
        u_int64_t currentTime = convert24HtoMinutes(words[0]);
        if(currentTime < startTime || currentTime > endTime) {
            paramBundle.outputInterface->outputLine(words[0] + " 13 NotOpenYet");
            continue;
        }
        
        u_int64_t tableNumber = words.size() == 4 ? std::stoull(words[3]) : 0;
        events[std::stoull(words[1])]->handleEvent(paramBundle, words[2], words[0], tableNumber);
    }
}