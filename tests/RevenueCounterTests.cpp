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

    ASSERT_EQ(output->allLines, correct_lines);
}

TEST_F(RevenueCounterTests, NotOpenYet) {
    std::vector<std::string> test_lines = {
        "3",
        "10:00 22:00",
        "1",
        "08:48 1 client1",
        "09:41 1 client2",
    };

    std::vector<std::string> correct_lines = {
        "10:00",
        "08:48 1 client1",
        "08:48 13 NotOpenYet",
        "09:41 1 client2",
        "09:41 13 NotOpenYet",
        "22:00",
        "1 0 00:00",
        "2 0 00:00",
        "3 0 00:00"
    };

    auto output = std::make_shared<OutputVector>();

    auto counter = createRevenueCounter(test_lines, output);
    counter.calculateRevenue();

    ASSERT_EQ(output->allLines, correct_lines);
}


TEST_F(RevenueCounterTests, FullDaySit) {
    std::vector<std::string> test_lines = {
        "3",
        "10:00 22:00",
        "1",
        "10:00 1 c1",
        "10:00 1 c2",
        "10:00 2 c1 1",
        "10:00 2 c2 2",
    };

    std::vector<std::string> correct_lines = {
        "10:00",
        "10:00 1 c1",
        "10:00 1 c2",
        "10:00 2 c1 1",
        "10:00 2 c2 2",
        "22:00 11 c1",
        "22:00 11 c2",
        "22:00",
        "1 12 12:00",
        "2 12 12:00",
        "3 0 00:00"
    };

    auto output = std::make_shared<OutputVector>();

    auto counter = createRevenueCounter(test_lines, output);
    counter.calculateRevenue();

    ASSERT_EQ(output->allLines, correct_lines);
}

TEST_F(RevenueCounterTests, OneHourSit) {
    std::vector<std::string> test_lines = {
        "3",
        "10:00 22:00",
        "1",
        "10:00 1 c1",
        "10:00 1 c2",
        "10:00 2 c1 1",
        "10:00 2 c2 2",
        "11:00 4 c1",
        "11:00 4 c2",
    };

    std::vector<std::string> correct_lines = {
        "10:00",
        "10:00 1 c1",
        "10:00 1 c2",
        "10:00 2 c1 1",
        "10:00 2 c2 2",
        "11:00 4 c1",
        "11:00 4 c2",
        "22:00",
        "1 1 01:00",
        "2 1 01:00",
        "3 0 00:00"
    };

    auto output = std::make_shared<OutputVector>();

    auto counter = createRevenueCounter(test_lines, output);
    counter.calculateRevenue();

    ASSERT_EQ(output->allLines, correct_lines);
}

TEST_F(RevenueCounterTests, ManualTableSwitchImmedately) {
    std::vector<std::string> test_lines = {
        "3",
        "10:00 22:00",
        "1",
        "10:00 1 c1",
        "10:00 1 c2",
        "10:00 2 c1 1",
        "10:00 2 c2 2",
        "11:00 4 c1",
        "11:00 2 c2 1"
    };

    std::vector<std::string> correct_lines = {
        "10:00",
        "10:00 1 c1",
        "10:00 1 c2",
        "10:00 2 c1 1",
        "10:00 2 c2 2",
        "11:00 4 c1",
        "11:00 2 c2 1",
        "22:00 11 c2",
        "22:00",
        "1 12 12:00",
        "2 1 01:00",
        "3 0 00:00"
    };

    auto output = std::make_shared<OutputVector>();

    auto counter = createRevenueCounter(test_lines, output);
    counter.calculateRevenue();

    ASSERT_EQ(output->allLines, correct_lines);
}

TEST_F(RevenueCounterTests, YouShallNotPass) {
    std::vector<std::string> test_lines = {
        "3",
        "10:00 22:00",
        "1",
        "10:00 1 c1",
        "10:00 1 c1",
        "11:00 4 c1",
        "10:00 1 c1",
    };

    std::vector<std::string> correct_lines = {
        "10:00",
        "10:00 1 c1",
        "10:00 1 c1",
        "10:00 13 YouShallNotPass",
        "11:00 4 c1",
        "10:00 1 c1",
        "22:00 11 c1",
        "22:00",
        "1 0 00:00",
        "2 0 00:00",
        "3 0 00:00"
    };

    auto output = std::make_shared<OutputVector>();

    auto counter = createRevenueCounter(test_lines, output);
    counter.calculateRevenue();

    ASSERT_EQ(output->allLines, correct_lines);
}

TEST_F(RevenueCounterTests, SitSameTablePlaceIsBusy) {
    std::vector<std::string> test_lines = {
        "3",
        "10:00 22:00",
        "1",
        "10:00 1 c1",
        "10:00 2 c1 1",
        "11:00 2 c1 1",
    };

    std::vector<std::string> correct_lines = {
        "10:00",
        "10:00 1 c1",
        "10:00 2 c1 1",
        "11:00 2 c1 1",
        "11:00 13 PlaceIsBusy",
        "22:00 11 c1",
        "22:00",
        "1 12 12:00",
        "2 0 00:00",
        "3 0 00:00"
    };

    auto output = std::make_shared<OutputVector>();

    auto counter = createRevenueCounter(test_lines, output);
    counter.calculateRevenue();

    ASSERT_EQ(output->allLines, correct_lines);
}

TEST_F(RevenueCounterTests, SitDifferentTablePlaceIsBusy) {
    std::vector<std::string> test_lines = {
        "3",
        "10:00 22:00",
        "1",
        "10:00 1 c1",
        "10:00 1 c2",
        "10:00 2 c1 1",
        "10:00 2 c2 2",
        "11:00 2 c1 2",
    };

    std::vector<std::string> correct_lines = {
        "10:00",
        "10:00 1 c1",
        "10:00 1 c2",
        "10:00 2 c1 1",
        "10:00 2 c2 2",
        "11:00 2 c1 2",
        "11:00 13 PlaceIsBusy",
        "22:00 11 c1",
        "22:00 11 c2",
        "22:00",
        "1 12 12:00",
        "2 12 12:00",
        "3 0 00:00"
    };

    auto output = std::make_shared<OutputVector>();

    auto counter = createRevenueCounter(test_lines, output);
    counter.calculateRevenue();

    ASSERT_EQ(output->allLines, correct_lines);
}

TEST_F(RevenueCounterTests, ClientUnknownSit) {
    std::vector<std::string> test_lines = {
        "3",
        "10:00 22:00",
        "1",
        "10:00 2 c1 1",
    };

    std::vector<std::string> correct_lines = {
        "10:00",
        "10:00 2 c1 1",
        "10:00 13 ClientUnknown",
        "22:00",
        "1 0 00:00",
        "2 0 00:00",
        "3 0 00:00"
    };

    auto output = std::make_shared<OutputVector>();

    auto counter = createRevenueCounter(test_lines, output);
    counter.calculateRevenue();

    ASSERT_EQ(output->allLines, correct_lines);
}

TEST_F(RevenueCounterTests, ClientUnknownWait) {
    std::vector<std::string> test_lines = {
        "3",
        "10:00 22:00",
        "1",
        "10:00 3 c1",
    };

    std::vector<std::string> correct_lines = {
        "10:00",
        "10:00 3 c1",
        "10:00 13 ClientUnknown",
        "22:00",
        "1 0 00:00",
        "2 0 00:00",
        "3 0 00:00"
    };

    auto output = std::make_shared<OutputVector>();

    auto counter = createRevenueCounter(test_lines, output);
    counter.calculateRevenue();

    ASSERT_EQ(output->allLines, correct_lines);
}

TEST_F(RevenueCounterTests, ClientUnknownLeave) {
    std::vector<std::string> test_lines = {
        "3",
        "10:00 22:00",
        "1",
        "10:00 4 c1",
    };

    std::vector<std::string> correct_lines = {
        "10:00",
        "10:00 4 c1",
        "10:00 13 ClientUnknown",
        "22:00",
        "1 0 00:00",
        "2 0 00:00",
        "3 0 00:00"
    };

    auto output = std::make_shared<OutputVector>();

    auto counter = createRevenueCounter(test_lines, output);
    counter.calculateRevenue();

    ASSERT_EQ(output->allLines, correct_lines);
}

TEST_F(RevenueCounterTests, ICanWaitNoLonger) {
    std::vector<std::string> test_lines = {
        "3",
        "10:00 22:00",
        "1",
        "10:00 1 c1",
        "10:00 3 c1",
    };

    std::vector<std::string> correct_lines = {
        "10:00",
        "10:00 1 c1",
        "10:00 3 c1",
        "10:00 13 ICanWaitNoLonger!",
        "22:00 11 c1",
        "22:00",
        "1 0 00:00",
        "2 0 00:00",
        "3 0 00:00"
    };

    auto output = std::make_shared<OutputVector>();

    auto counter = createRevenueCounter(test_lines, output);
    counter.calculateRevenue();

    ASSERT_EQ(output->allLines, correct_lines);
}

TEST_F(RevenueCounterTests, ClientLeftBecauseOfQueue) {
    std::vector<std::string> test_lines = {
        "1",
        "10:00 22:00",
        "1",
        "10:00 1 c1",
        "10:00 2 c1 1",
        "10:00 1 c2",
        "10:00 1 c3",
        "10:00 1 c4",
        "10:00 3 c2",
        "10:00 3 c3",
        "10:00 3 c4",
    };

    std::vector<std::string> correct_lines = {
        "10:00",
        "10:00 1 c1",
        "10:00 2 c1 1",
        "10:00 1 c2",
        "10:00 1 c3",
        "10:00 1 c4",
        "10:00 3 c2",
        "10:00 3 c3",
        "10:00 3 c4",
        "10:00 11 c4",
        "22:00 11 c1",
        "22:00 11 c2",
        "22:00 11 c3",
        "22:00",
        "1 12 12:00",
    };

    auto output = std::make_shared<OutputVector>();

    auto counter = createRevenueCounter(test_lines, output);
    counter.calculateRevenue();

    ASSERT_EQ(output->allLines, correct_lines);
}

TEST_F(RevenueCounterTests, TakingClientsFromQueue) {
    std::vector<std::string> test_lines = {
        "1",
        "10:00 22:00",
        "1",
        "10:00 1 c1",
        "10:00 2 c1 1",
        "10:00 1 c2",
        "10:00 3 c2",
        "10:00 1 c3",
        "10:00 3 c3",
        "10:30 4 c1",
        "15:30 4 c2",
    };

    std::vector<std::string> correct_lines = {
        "10:00",
        "10:00 1 c1",
        "10:00 2 c1 1",
        "10:00 1 c2",
        "10:00 3 c2",
        "10:00 1 c3",
        "10:00 3 c3",
        "10:30 4 c1",
        "10:30 12 c2 1",
        "15:30 4 c2",
        "15:30 12 c3 1",
        "22:00 11 c3",
        "22:00",
        "1 13 12:00",
    };

    auto output = std::make_shared<OutputVector>();

    auto counter = createRevenueCounter(test_lines, output);
    counter.calculateRevenue();

    ASSERT_EQ(output->allLines, correct_lines);
}