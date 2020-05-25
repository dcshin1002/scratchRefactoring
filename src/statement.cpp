#include "statement.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

// const format = new Intl.NumberFormat("en-US", { style: "currency", currency: "USD",
// minimumFractionDigits: 2 }).format;
std::string format(float value) {
    return std::to_string(value);
}

std::string statement(Json::Value const& invoice, Json::Value const& plays) {
    int totalAmount = 0;
    int volumeCredits = 0;

    std::stringstream result;
    result << "청구 내역 (고객명: " << invoice["customer"].asCString() << '\n';

    for (Json::Value const& perf : invoice["performances"]) {
        Json::Value const& play = perf["playID"];
        int thisAmount = 0;

        std::string playType = plays[play.asCString()]["type"].asCString();
        std::cout << "playType: " << playType << std::endl;
        if (!playType.compare("tragedy")) {
            thisAmount = 40000;
            if (perf["audience"].asInt() > 30) {
                thisAmount += 1000 * (perf["audience"].asInt() - 20);
            }
        } else if (!playType.compare("comedy")) {
            thisAmount = 30000;
            if (perf["audience"].asInt() > 20) {
                thisAmount += 10000 + 500 * (perf["audience"].asInt() - 20);
            }
            thisAmount += 300 * perf["audience"].asInt();
        } else {
            std::stringstream ss;
            ss << "알 수 없는 장르: " << playType << std::endl;
            throw std::logic_error(ss.str());
        }

        // 포인트를 적립한다.
        volumeCredits += std::max(perf["audience"].asInt() - 30, 0);

        // 희극 관객 5명마다 추가 포인트를 제공한다.
        if (!playType.compare("comedy"))
            volumeCredits += std::floor(perf["audience"].asInt() / 5.0);

        // 청구 내역을 출력한다.
        result << "  " << play.asCString() << ": " << format(thisAmount / 100.0) << "("
               << perf["audience"].asInt() << "석)\n";
        totalAmount += thisAmount;
    }

    result << "총액: $" << format((float)totalAmount / 100.0) << '\n';
    result << "적립 포인트: " << format(volumeCredits) << "점\n";
    std::cout << result.str() << std::endl;
    return result.str();
}