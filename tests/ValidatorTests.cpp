#include <gtest/gtest.h>
#include <fstream>

#include "../src/Validator.h"

class ValidatorTests : public testing::Test {
public:
    ValidatorTests() {}
    
    Validator createValidator(const std::vector<std::string>& lines) {
        std::ofstream file("test_file.txt");

        for(const auto& line : lines) {
            file << line << '\n';
        }

        return Validator("test_file.txt");
    }
};

TEST_F(ValidatorTests, Basic) {
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
    auto validator = createValidator(test_lines);

    auto result = validator.validateFile();

    ASSERT_EQ(result.getStatus(), ValidationStatus::CORRECT);
}

TEST_F(ValidatorTests, BadTableNumber1) {
    std::vector<std::string> test_lines = {
        "0",
        "09:00 19:00",
        "10",
        "08:48 1 client1",
    };
    auto validator = createValidator(test_lines);

    auto result = validator.validateFile();

    ASSERT_EQ(result.getStatus(), ValidationStatus::INVALID_FORMAT);
    ASSERT_EQ(result.getErrorString(), "0");
}

TEST_F(ValidatorTests, BadTableNumber2) {
    std::vector<std::string> test_lines = {
        "-3",
        "09:00 19:00",
        "10",
        "08:48 1 client1",
    };
    auto validator = createValidator(test_lines);

    auto result = validator.validateFile();

    ASSERT_EQ(result.getStatus(), ValidationStatus::INVALID_FORMAT);
    ASSERT_EQ(result.getErrorString(), "-3");
}

TEST_F(ValidatorTests, BadTableNumber3) {
    std::vector<std::string> test_lines = {
        "2.2",
        "09:00 19:00",
        "10",
        "08:48 1 client1",
    };
    auto validator = createValidator(test_lines);

    auto result = validator.validateFile();

    ASSERT_EQ(result.getStatus(), ValidationStatus::INVALID_FORMAT);
    ASSERT_EQ(result.getErrorString(), "2.2");
}

TEST_F(ValidatorTests, BadTimeFormat1) {
    std::vector<std::string> test_lines = {
        "3",
        "9:00 19:00",
        "10",
        "08:48 1 client1",
    };
    auto validator = createValidator(test_lines);

    auto result = validator.validateFile();

    ASSERT_EQ(result.getStatus(), ValidationStatus::INVALID_FORMAT);
    ASSERT_EQ(result.getErrorString(), "9:00 19:00");
}

TEST_F(ValidatorTests, BadTimeFormat2) {
    std::vector<std::string> test_lines = {
        "3",
        "09:0019:00",
        "10",
        "08:48 1 client1",
    };
    auto validator = createValidator(test_lines);

    auto result = validator.validateFile();

    ASSERT_EQ(result.getStatus(), ValidationStatus::INVALID_FORMAT);
    ASSERT_EQ(result.getErrorString(), "09:0019:00");
}

TEST_F(ValidatorTests, BadTimeFormat3) {
    std::vector<std::string> test_lines = {
        "3",
        "00:00 24:00",
        "10",
        "08:48 1 client1",
    };
    auto validator = createValidator(test_lines);

    auto result = validator.validateFile();

    ASSERT_EQ(result.getStatus(), ValidationStatus::INVALID_FORMAT);
    ASSERT_EQ(result.getErrorString(), "00:00 24:00");
}

TEST_F(ValidatorTests, BadHourcost1) {
    std::vector<std::string> test_lines = {
        "3",
        "09:00 19:00",
        "0",
        "08:48 1 client1",
    };
    auto validator = createValidator(test_lines);

    auto result = validator.validateFile();

    ASSERT_EQ(result.getStatus(), ValidationStatus::INVALID_FORMAT);
    ASSERT_EQ(result.getErrorString(), "0");
}

TEST_F(ValidatorTests, BadHourcost2) {
    std::vector<std::string> test_lines = {
        "3",
        "09:00 19:00",
        "-10",
        "08:48 1 client1",
    };
    auto validator = createValidator(test_lines);

    auto result = validator.validateFile();

    ASSERT_EQ(result.getStatus(), ValidationStatus::INVALID_FORMAT);
    ASSERT_EQ(result.getErrorString(), "-10");
}

TEST_F(ValidatorTests, BadHourcost3) {
    std::vector<std::string> test_lines = {
        "3",
        "09:00 19:00",
        "1.5",
        "08:48 1 client1",
    };
    auto validator = createValidator(test_lines);

    auto result = validator.validateFile();

    ASSERT_EQ(result.getStatus(), ValidationStatus::INVALID_FORMAT);
    ASSERT_EQ(result.getErrorString(), "1.5");
}

TEST_F(ValidatorTests, BadFileFormat1) {
    std::vector<std::string> test_lines = {
        "09:00 19:00",
        "10",
        "08:48 1 client1",
    };
    auto validator = createValidator(test_lines);

    auto result = validator.validateFile();

    ASSERT_EQ(result.getStatus(), ValidationStatus::INVALID_FORMAT);
    ASSERT_EQ(result.getErrorString(), "09:00 19:00");
}

TEST_F(ValidatorTests, BadFileFormat2) {
    std::vector<std::string> test_lines = {
        "3",
        "10",
        "08:48 1 client1",
    };
    auto validator = createValidator(test_lines);

    auto result = validator.validateFile();

    ASSERT_EQ(result.getStatus(), ValidationStatus::INVALID_FORMAT);
    ASSERT_EQ(result.getErrorString(), "10");
}

TEST_F(ValidatorTests, BadFileFormat3) {
    std::vector<std::string> test_lines = {
        "3",
        "09:00 19:00",
        "08:48 1 client1",
    };
    auto validator = createValidator(test_lines);

    auto result = validator.validateFile();

    ASSERT_EQ(result.getStatus(), ValidationStatus::INVALID_FORMAT);
    ASSERT_EQ(result.getErrorString(), "08:48 1 client1");
}

TEST_F(ValidatorTests, BadFileFormat4) {
    std::vector<std::string> test_lines = {
        "3",
        "09:00 19:00",
        "",
        "10",
        "08:48 1 client1",
    };
    auto validator = createValidator(test_lines);

    auto result = validator.validateFile();

    ASSERT_EQ(result.getStatus(), ValidationStatus::INVALID_FORMAT);
    ASSERT_EQ(result.getErrorString(), "");
}

TEST_F(ValidatorTests, BadFileFormat5) {
    std::vector<std::string> test_lines = {
        "3",
        " ",
        "09:00 19:00",
        "10",
        "08:48 1 client1",
    };
    auto validator = createValidator(test_lines);

    auto result = validator.validateFile();

    ASSERT_EQ(result.getStatus(), ValidationStatus::INVALID_FORMAT);
    ASSERT_EQ(result.getErrorString(), " ");
}

TEST_F(ValidatorTests, BadEventId) {
    std::vector<std::string> test_lines = {
        "3",
        "09:00 19:00",
        "10",
        "08:48 -2 client1",
    };
    auto validator = createValidator(test_lines);

    auto result = validator.validateFile();

    ASSERT_EQ(result.getStatus(), ValidationStatus::INVALID_FORMAT);
    ASSERT_EQ(result.getErrorString(), "08:48 -2 client1");
}

TEST_F(ValidatorTests, BadClientName1) {
    std::vector<std::string> test_lines = {
        "3",
        "09:00 19:00",
        "10",
        "08:48 2 Client1",
    };
    auto validator = createValidator(test_lines);

    auto result = validator.validateFile();

    ASSERT_EQ(result.getStatus(), ValidationStatus::INVALID_FORMAT);
    ASSERT_EQ(result.getErrorString(), "08:48 2 Client1");
}

TEST_F(ValidatorTests, BadClientName2) {
    std::vector<std::string> test_lines = {
        "3",
        "09:00 19:00",
        "10",
        "08:48 2 cli&ent1",
    };
    auto validator = createValidator(test_lines);

    auto result = validator.validateFile();

    ASSERT_EQ(result.getStatus(), ValidationStatus::INVALID_FORMAT);
    ASSERT_EQ(result.getErrorString(), "08:48 2 cli&ent1");
}