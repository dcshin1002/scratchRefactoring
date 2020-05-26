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
std::string format(float value) {
    return std::to_string(value);
}

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

int amountFor(Json::Value const& perf, Json::Value const& play) {
    int thisAmount = 0;
    if (!play["type"].asString().compare("tragedy")) {
        thisAmount = 40000;
        if (perf["audience"].asInt() > 30) {
            thisAmount += 1000 * (perf["audience"].asInt() - 30);
        }
    } else if (!play["type"].asString().compare("comedy")) {
        thisAmount = 30000;
        if (perf["audience"].asInt() > 20) {
            thisAmount += 10000 + 500 * (perf["audience"].asInt() - 20);
        }
        thisAmount += 300 * perf["audience"].asInt();
    } else {
        std::stringstream ss;
        ss << "알 수 없는 장르: " << play["type"].asString() << std::endl;
        throw std::logic_error(ss.str());
    }
    return thisAmount;
}

struct InQuery {
    std::string customerName;
    std::map<std::string, int> playAudiences;
    int totalAmount;
    int volumeCredits;

    InQuery(std::string const& name) : customerName(name), totalAmount(0), volumeCredits(0) {}

    std::string getState() {
        std::stringstream result;
        result << "청구 내역 (고객명: " << customerName << ")\n";

        // 청구 내역을 출력한다.
        for (auto& it : playAudiences) {
            std::string const& playID = it.first;
            int audiences = it.second;
            result << "  " << playID << ": " << format(audiences / 100.0) << "("
                << audiences << "석)\n";
        }
        result << "총액: $" << format((float)totalAmount / 100.0) << '\n';
        result << "적립 포인트: " << format(volumeCredits) << "점\n";
        return result.str();
    }
};

InQuery calculateInquery(Json::Value const& invoice, Json::Value const& plays) {
    InQuery result(invoice["customer"].asString());
    for (Json::Value const& perf : invoice["performances"]) {
        Json::Value play = plays[perf["playID"].asCString()];
        int thisAmount = amountFor(perf, play);

        // 포인트를 적립한다.
        result.volumeCredits += std::max(perf["audience"].asInt() - 30, 0);

        // 희극 관객 5명마다 추가 포인트를 제공한다.
        if (!play["type"].asString().compare("comedy"))
            result.volumeCredits += std::floor(perf["audience"].asInt() / 5.0);

        result.totalAmount += thisAmount;
    }
    return result;
}

std::string statement(Json::Value const& invoice, Json::Value const& plays) {
    InQuery query = calculateInquery(invoice, plays);
    return query.getState();
}