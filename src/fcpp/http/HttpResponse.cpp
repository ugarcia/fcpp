#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include "json/json.h"

#include "fcpp/util/Util.hpp"
#include "fcpp/http/HttpResponse.hpp"

using namespace std;
using namespace fcpp::http;
using namespace fcpp::util;

HttpResponse::HttpResponse(long statusCode, unordered_map<string, string> headers, string body, Json::Value json)
{
    this->statusCode = statusCode;
    this->headers = headers;
    this->body = body;
    this->json = json;
}

long HttpResponse::getStatusCode()
{
    return this->statusCode;
}

HttpResponse HttpResponse::setStatusCode(long statusCode)
{
    this->statusCode = statusCode;
    return *this;
}

string HttpResponse::getBody()
{
    return this->body;
}

HttpResponse HttpResponse::setBody(string body)
{
    this->body = body;
    this->json = Json::Value(body);
    return *this;
}

Json::Value HttpResponse::getJson()
{
    return this->json;
}

HttpResponse HttpResponse::setJson(Json::Value json)
{
    this->body = json.asString();
    this->json = json;
    return *this;
}

unordered_map<string, string> HttpResponse::getHeaders()
{
    return this->headers;
}

HttpResponse HttpResponse::setHeaders(unordered_map<string, string> headers)
{
    this->headers = headers;
    return *this;
}

HttpResponse HttpResponse::setHeaders(string headers)
{
    this->headers = this->parseHeadersString(headers);
    return *this;
}

// Parses a headers string into a Map
unordered_map<string, string> HttpResponse::parseHeadersString(string headers)
{
    unordered_map<string, string> m;
    istringstream iheaders(headers);
    string header;
    string::size_type index;
    while (getline(iheaders, header)) {
        if (header.empty()) {
            continue;
        }
        index = header.find(':', 0);
        if(index != string::npos) {
            m.insert(make_pair(
                Util::trim(header.substr(0, index)), 
                Util::trim(header.substr(index + 1))
            ));
        }
    }
    return move(m);
}
