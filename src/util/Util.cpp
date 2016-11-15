#include <string>

#include "json/json.h"

#include "fcpp/util/Util.hpp"

using namespace std;
using namespace fcpp::util;

// Trims a string (both sides, spaces, tabs and endlines)
string Util::trim(string s)
{
    s.erase(0, s.find_first_not_of(" \n\r\t"));                                                                                               
    s.erase(s.find_last_not_of(" \n\r\t")+1);
    return s;
}

// Parses a Json string into Json Object
Json::Value Util::parseJson(string s)
{
    Json::Value v;
    Json::Reader r;
    r.parse(s, v); 
    return move(v);
}
