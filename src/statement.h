#ifndef _STATEMENT_H_INCLUDED
#define _STATEMENT_H_INCLUDED
// contents of the file are here
#include <string>
#include <json/json.h>

std::string statement(Json::Value const& invoice, Json::Value const& plays);

#endif