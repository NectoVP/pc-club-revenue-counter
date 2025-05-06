#pragma once

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <deque>
#include <algorithm>

#include "Utility.h"

#include "events/ClientArrive.h"
#include "events/ClientSit.h"
#include "events/ClientWait.h"
#include "events/ClientLeft.h"
#include "events/ClientEndOfDay.h"

class RevenueCounter {
public:
    explicit RevenueCounter(const std::string& filePath, const std::shared_ptr<OutputInterface>& outputInterface);
    explicit RevenueCounter(const std::string& filePath);

    void calculateRevenue();

private:
    void init(std::ifstream& file);
    void parseEvents(std::ifstream& file);

    std::unordered_map<uint64_t, std::unique_ptr<EventHandler>> events;

    u_int64_t tableCount;
    
    std::string startTimeStr;
    std::string endTimeStr;

    size_t startTime;
    size_t endTime;

    std::string filePath;

    ParamBundle paramBundle;
};