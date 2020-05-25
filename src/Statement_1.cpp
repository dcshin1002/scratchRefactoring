#include "Statement.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>

// const format = new Intl.NumberFormat("en-US", { style: "currency", currency:
// "USD", minimumFractionDigits: 2 }).format;
std::string format(float value) { return std::to_string(value); }

Json::Value getJson(std::string const& jsonPath) {
    Json::Value result;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errs;

    std::ifstream ifs;
    ifs.open(jsonPath.c_str());
    if (!ifs.is_open()) {
        std::string errMsg("failed to open file: ");
        errMsg.append(jsonPath);
        throw std::logic_error(errMsg.c_str());
    }

    if (!parseFromStream(builder, ifs, &result, &errs)) {
        throw std::logic_error(errs.c_str());
    }
    ifs.close();
    return result;
}

std::string statement(Json::Value const& invoice, Json::Value const& plays) {
    int totalAmount = 0;
    int volumeCredits = 0;

    std::stringstream result;
    result << "청구 내역 (고객명: " << invoice["customer"].asString() << ")\n";

    for (Json::Value const& perf : invoice["performances"]) {
        int thisAmount = 0;
        std::string playID = perf["playID"].asString();
        std::string playType = plays[playID]["type"].asString();
        if (!playType.compare("tragedy")) {
            thisAmount = 40000;
            if (perf["audience"].asInt() > 30) {
                thisAmount += 1000 * (perf["audience"].asInt() - 30);
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
        result << "  " << playID << ": " << format(thisAmount / 100.0)
               << "(" << perf["audience"].asInt() << "석)\n";
        totalAmount += thisAmount;
    }

    result << "총액: $" << format((float)totalAmount / 100.0) << '\n';
    result << "적립 포인트: " << format(volumeCredits) << "점\n";
    return result.str();
}