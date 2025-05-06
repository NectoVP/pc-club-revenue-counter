#include <gtest/gtest.h>
#include <fstream>

#include "../src/RevenueCounter.h"
#include "../src/output/OutputVector.h"

class RevenueCounterTests : public testing::Test {
public:
    RevenueCounterTests() {}
    
    RevenueCounter createRevenueCounter(const std::vector<std::string>& lines, const std::shared_ptr<OutputVector>& outputVector) {
        std::ofstream file("test_file.txt");

        for(const auto& line : lines) {
            file << line << '\n';
        }

        return RevenueCounter("test_file.txt", outputVector);
    }
};

TEST_F(RevenueCounterTests, basic) {
    std::vector<std::string> test_lines = {
        "3",
        "09:00 19:00",
        "10",
        "08:48 1 client1",
        "09:41 1 client1",
        "09:48 1 client2",
        "09:52 3 client1",
        "09:54 2 client1 1",
        "10:25 2 client2 2",
        "10:58 1 client3",
        "10:59 2 client3 3",
        "11:30 1 client4",
        "11:35 2 client4 2",
        "11:45 3 client4",
        "12:33 4 client1",
        "12:43 4 client2",
        "15:52 4 client4"
    };

    std::vector<std::string> correct_lines = {
        "09:00",
        "08:48 1 client1",
        "08:48 13 NotOpenYet",
        "09:41 1 client1",
        "09:48 1 client2",
        "09:52 3 client1",
        "09:52 13 ICanWaitNoLonger!",
        "09:54 2 client1 1",
        "10:25 2 client2 2",
        "10:58 1 client3",
        "10:59 2 client3 3",
        "11:30 1 client4",
        "11:35 2 client4 2",
        "11:35 13 PlaceIsBusy",
        "11:45 3 client4",
        "12:33 4 client1",
        "12:33 12 client4 1",
        "12:43 4 client2",
        "15:52 4 client4",
        "19:00 11 client3",
        "19:00",
        "1 70 05:58",
        "2 30 02:18",
        "3 90 08:01"
    };

    auto output = std::make_shared<OutputVector>();

    auto counter = createRevenueCounter(test_lines, output);
    counter.calculateRevenue();

    ASSERT_EQ(output->allLines.size(), 24);
    ASSERT_EQ(output->allLines, correct_lines);
}