#include "RevenueCounter.h"

RevenueCounter::RevenueCounter(const std::string& filePath) : filePath(filePath) {
    events.emplace(1, std::make_unique<ClientArrive>());
    events.emplace(2, std::make_unique<ClientSit>());
    events.emplace(3, std::make_unique<ClientWait>());
    events.emplace(4, std::make_unique<ClientLeft>());
}

void RevenueCounter::calculateRevenue() {
    std::ifstream file(filePath);

    if(!file.is_open()) {
        std::cout << "File was not opened!\n";
        return;
    }

    init(file);

    std::cout << startTimeStr << std::endl;

    parseEvents(file);

    
    std::vector<std::string> leftoverClients;
    for (const auto& pair : paramBundle.mapUserToTable)
        leftoverClients.push_back(pair.first);
    std::sort(leftoverClients.begin(), leftoverClients.end());
    
    for (const auto& key : leftoverClients)
        events[4]->handleEvent(paramBundle, key, endTimeStr, 0);

    std::cout << endTimeStr << std::endl;

    for(int i = 1; i <= tableCount; ++i)
        std::cout << i << ' ' << std::get<1>(paramBundle.tablesInfo[i]) << ' ' << convertMinutesTo24H(std::get<0>(paramBundle.tablesInfo[i])) << std::endl;

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
        std::cout << line << std::endl;
        
        auto words = getWords(line);
        u_int64_t currentTime = convert24HtoMinutes(words[0]);
        if(currentTime < startTime || currentTime > endTime) {
            std::cout << words[0] << " 13 NotOpenYet\n";
            continue;
        }
        u_int64_t tableNumber = words.size() == 4 ? std::stoull(std::string(words[3])) : 0;
        events[std::stoull(std::string(words[1]))]->handleEvent(paramBundle, std::string(words[2]), words[0], tableNumber);
    }
}