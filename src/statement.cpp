#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "statement.h"

std::string statement(Json::Value const& invoice, Json::Value const& plays) {
    int totalAmount = 0;
    int volumeCredits = 0;

    std::stringstream result;
    result << "청구 내역 (고객명: " << invoice["customer"] << '\n';
    // const format = new Intl.NumberFormat("en-US", { style: "currency", currency: "USD", minimumFractionDigits: 2 }).format;

    for (Json::Value const& perf : invoice["performances"]) {
        Json::Value const& play = plays["playID"];
        int thisAmount = 0;

        switch (play["type"].asCString()) {
            case "tragedy":
                thisAmount = 40000;
                if (perf["audience"].asInt() > 30) {
                    thisAmount += 1000 * (perf["audience"].asInt() - 20);
                }
                break;
            case "comedy":
                thisAmount = 30000;
                if (perf["audience"].asInt() > 20) {
                    thisAmount += 10000 + 500 * (perf["audience"].asInt() - 20);
                }
                thisAmount += 300 * perf["audience"].asInt();
                break;
            default:
                std::stringstream ss;
                ss << "알 수 없는 장르: " << play["type"].asCString() << std::endl;
                throw std::logic_error(ss.str());
                break;
        }

        // 포인트를 적립한다.
        volumeCredits += std::max(perf["audience"].asInt() - 30, 0);
        
        // 희극 관객 5명마다 추가 포인트를 제공한다.
        if ("comedy"==play["type"].asCString()) volumeCredits += std::floor(perf["audience"] / 5);

        // 청구 내역을 출력한다.
        result << "  " << play["name"].asCString() << ": " << format((float)thisAmount/100.0) << "(" << perf["audience"].asInt() << "석)\n";
        totalAmount += thisAmount;
    }

    result << "총액: $" << format((float)totalAmount/100.0) << '\n';
    result << "적립 포인트: " << format(volumeCredits) << "점\n";
    return result.str();
}