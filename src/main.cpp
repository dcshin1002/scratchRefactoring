#include <json/json.h>

#include <fstream>
#include <iostream>

#include "statement.h"

// PATH_TO_JSON : exported path from pre-definition
int main(int argc, char* argv[]) {
    std::ifstream ifs;
    std::string jsonPath = PATH_TO_JSON;
    jsonPath += "/invoices.json";
    ifs.open(jsonPath.c_str());
    if (!ifs.is_open()) {
        std::cout << "failed to open invoices.json" << std::endl;
    }

    Json::Value invoices;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;
    if (!parseFromStream(builder, ifs, &invoices, &errs)) {
        std::cout << errs << std::endl;
        return EXIT_FAILURE;
    }
    ifs.close();

    jsonPath = PATH_TO_JSON;
    jsonPath += "/plays.json";
    ifs.open(jsonPath.c_str());
    if (!ifs.is_open()) {
        std::cout << "failed to open plays.json" << std::endl;
        return EXIT_FAILURE;
    }
    Json::Value plays;
    if (!parseFromStream(builder, ifs, &plays, &errs)) {
        std::cout << errs << std::endl;
        return EXIT_FAILURE;
    }
    ifs.close();

    std::cout << statement(invoices, plays) << std::endl;
    return EXIT_SUCCESS;
}