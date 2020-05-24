#include <gtest/gtest.h>

#include <fstream>
#include <iostream>

#include "statement.h"

struct StatementTest : public testing::Test {
    Json::Value invoices;
    Json::Value plays;

    StatementTest() {
        std::ifstream ifs;
        std::string jsonPath(PATH_TO_JSON);
        jsonPath += "/invoices.json";
        ifs.open(jsonPath.c_str());
        if (!ifs.is_open()) {
            std::cout << "failed to open invoices.json" << std::endl;
        }

        Json::CharReaderBuilder builder;
        JSONCPP_STRING errs;
        if (!parseFromStream(builder, ifs, &invoices, &errs)) {
            std::cout << errs << std::endl;
        }
        ifs.close();

        jsonPath = PATH_TO_JSON;
        jsonPath += "/invoices.json";
        ifs.open(jsonPath.c_str());
        if (!ifs.is_open()) {
            std::cout << "failed to open plays.json" << std::endl;
        }
        if (!parseFromStream(builder, ifs, &plays, &errs)) {
            std::cout << errs << std::endl;
        }
        ifs.close();
    }
    // PATH_TO_JSON : exported path from pre-definition
    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(StatementTest, ParseJSON) {
    EXPECT_STREQ(statement(invoices, plays).c_str(), "ok");
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}