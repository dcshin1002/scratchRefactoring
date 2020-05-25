#include <json/json.h>

#include <fstream>
#include <iostream>

#include "Statement.h"

int main(int argc, char* argv[]) {
    // PATH_TO_JSON : exported path from pre-definition
    std::string invoicesJson(PATH_TO_JSON);
    invoicesJson.append("/invoices.json");
    Json::Value invoices = getJson(invoicesJson);

    std::string playsJson(PATH_TO_JSON);
    playsJson.append("/plays.json");
    Json::Value plays = getJson(playsJson);

    std::cout << statement(invoices, plays) << std::endl;
    return EXIT_SUCCESS;
}