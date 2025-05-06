#include <iostream>

#include "src/Validator.h"
#include "src/RevenueCounter.h"
//#include "src/output/OutputConsole.h"

int main(int argc, char** argv) {
    //if(argc != 2) {
    //    std::cout << "one file need to be provided";
    //    return -1;
    //}

    //Validator validator(argv[1]);
    
    Validator validator("../file.txt");
    auto res = validator.validateFile();
    //std::cout << res.getStatus() << "\t" << res.getErrorString() << std::endl;

    RevenueCounter revenueCounter("../file.txt");
    if(res.getStatus() == ValidationStatus::CORRECT)
        revenueCounter.calculateRevenue();

    return 0;
}