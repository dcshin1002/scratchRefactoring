#include <gtest/gtest.h>

#include <fstream>
#include <iostream>

#include "Statement.h"

// PATH_TO_JSON : exported path from pre-definition
struct StatementTest : public testing::Test {
    Json::Value invoices;
    Json::Value plays;

    void SetUp() override {}
    void TearDown() override {}
};

TEST_F(StatementTest, ParseJSON) {
    std::string invoicesJson(PATH_TO_JSON);
    invoicesJson.append("/invoices.json");
    invoices = getJson(invoicesJson);

    std::string playsJson(PATH_TO_JSON);
    playsJson.append("/plays.json");
    plays = getJson(playsJson);
    std::string result = statement(invoices, plays);
    EXPECT_NO_THROW();
}

TEST_F(StatementTest, CheckAmountFor) {
    std::string const& testInvoices = R"(
        {
            "customer": "BigCo",
            "performances": [
                { "playID": "AAA", "audience": 30 },
                { "playID": "AAB", "audience": 31 },
                { "playID": "BBB", "audience": 20 },
                { "playID": "BCB", "audience": 21 },
            ]
        }
    )";
    std::string const& testPlays = R"(
        {
            "AAA": { "name": "this is AAA", "type": "tragedy" },
            "AAB": { "name": "this is AAB", "type": "tragedy" },
            "BBB": { "name": "this is BBB", "type": "comedy" },
            "BCB": { "name": "this is BCB", "type": "comedy" },
        }
    )";
    Json::CharReaderBuilder builder;
    Json::CharReader* reader = builder.newCharReader();
    JSONCPP_STRING errs;
    bool parsed = reader->parse(testInvoices.c_str(), testInvoices.c_str()+testInvoices.size(), &invoices, &errs);
    parsed = reader->parse(testPlays.c_str(), testPlays.c_str()+testPlays.size(), &plays, &errs);
    EXPECT_TRUE(parsed);

    delete reader;

    std::string result = statement(invoices, plays);
    EXPECT_NO_THROW();
    // TODO: check results
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}