#include <gtest/gtest.h>

#include <fstream>
#include <iostream>

#include "Statement.h"

// PATH_TO_JSON : exported path from pre-definition
struct StatementTest : public testing::Test {
    Json::Value invoices;
    Json::Value plays;

    StatementTest() {
        std::string invoicesJson(PATH_TO_JSON);
        invoicesJson.append("/invoices.json");
        invoices = getJson(invoicesJson);

        std::string playsJson(PATH_TO_JSON);
        playsJson.append("/plays.json");
        plays = getJson(playsJson);
    }
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(StatementTest, ParseJSON) {
    std::string result = statement(invoices, plays);
    EXPECT_NO_THROW();
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}