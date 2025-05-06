#pragma once

#include "EventHandler.h"

class ClientLeft : public EventHandler {
public:
    void handleEvent(ParamBundle& paramBundle, const std::string& clientName, const std::string& current_time, uint64_t tableNumber = 0) const override;
    ~ClientLeft() override {}
};