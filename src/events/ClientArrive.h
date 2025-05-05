#pragma once

#include "EventHandler.h"

class ClientArrive : public EventHandler {
public:
    void handleEvent(ParamBundle& paramBundle, std::string clientName, std::string_view current_time, uint64_t tableNumber = 0) const override;
    ~ClientArrive() override {}
};