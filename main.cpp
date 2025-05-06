#include <iostream>

#include "src/Validator.h"
#include "src/RevenueCounter.h"
#include "src/output/OutputConsole.h"
#include "src/output/OutputVector.h"

int main(int argc, char** argv) {
    if(argc != 2) {
        std::cout << "one file need to be provided";
        return -1;
    }

    Validator validator(argv[1]);
    
    auto res = validator.validateFile();
    
    if(res.getStatus() != ValidationStatus::CORRECT) {
        std::cout << res.getErrorString() << std::endl;
        return -1;
    }

    RevenueCounter revenueCounter(argv[1]);
    revenueCounter.calculateRevenue();
    
    return 0;
}