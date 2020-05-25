#ifndef __STATEMENT_H_INCLUDED
#define __STATEMENT_H_INCLUDED
// contents of the file are here
#include <json/json.h>

#include <string>

Json::Value getJson(std::string const& jsonPath);
std::string statement(Json::Value const& invoice, Json::Value const& plays);

#endif