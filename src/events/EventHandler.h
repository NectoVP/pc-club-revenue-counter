#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <deque>
#include <limits>

#include "../output/OutputInterface.h"

enum TableStatus {
    EMPTY,
    OCCUPIED,
};

struct ParamBundle {
    ParamBundle(std::unique_ptr<OutputInterface>&& outputInterface) : outputInterface(std::move(outputInterface)) {}

    //key - client, value - pair(user arrival time, table number)
    std::unordered_map<std::string, std::pair<u_int64_t, u_int64_t>> mapUserToTable;

    //index - table number, value - tuple(total time, revenue, status)
    std::vector<std::tuple<u_int64_t, u_int64_t, TableStatus>> tablesInfo;

    std::deque<std::string> clientDeque;

    std::unique_ptr<OutputInterface> outputInterface;

    u_int64_t leftoverTables = 0;
    u_int64_t hourCost = 0;
};

class EventHandler {
public:
    virtual void handleEvent(ParamBundle& paramBundle, const std::string& clientName, const std::string& current_time, uint64_t tableNumber = 0) const = 0;
    virtual ~EventHandler() = default;
};

size_t convert24HtoMinutes(const std::string& str);

std::string convertMinutesTo24H(u_int64_t minutes);