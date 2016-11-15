#ifndef FC_HTTPRESPONSE_H
#define FC_HTTPRESPONSE_H

#include <string>
#include <unordered_map>

#include "json/json.h"

#include "fcpp/util/Util.hpp"

using namespace std;
using namespace fcpp::util;

namespace fcpp
{
    namespace http
    {
        class HttpResponse
        {
            private:
                long statusCode;
                string body;
                Json::Value json;
                unordered_map<string, string> headers;

                // Parses a headers string into a Map
                unordered_map<string, string> parseHeadersString(string headers);

            public:
                HttpResponse(long statusCode, unordered_map<string, string> headers, string body, Json::Value json);
                HttpResponse(long statusCode, string headers, string body, Json::Value json) 
                    : HttpResponse(statusCode, this->parseHeadersString(headers), body, json) {}
                HttpResponse(long statusCode, unordered_map<string, string> headers, string body) 
                    : HttpResponse(statusCode, headers, body, Util::parseJson(body)) {}
                HttpResponse(long statusCode, unordered_map<string, string> headers, Json::Value json) 
                    : HttpResponse(statusCode, headers, json.asString(), json) {}
                HttpResponse(long statusCode, string headers, string body) 
                    : HttpResponse(statusCode, headers, body, Util::parseJson(body)) {}
                HttpResponse(long statusCode, string headers, Json::Value json) 
                    : HttpResponse(statusCode, headers, json.asString(), json) {}
                HttpResponse(long statusCode, unordered_map<string, string> headers) 
                    : HttpResponse(statusCode, headers, this->body) {}
                HttpResponse(long statusCode, string body) 
                    : HttpResponse(statusCode, this->headers, body) {}
                HttpResponse(long statusCode, Json::Value json) 
                    : HttpResponse(statusCode, this->headers, json.asString(), json) {}
                HttpResponse(unordered_map<string, string> headers, string body) 
                    : HttpResponse(this->statusCode, headers, body) {}
                HttpResponse(unordered_map<string, string> headers, Json::Value json) 
                    : HttpResponse(this->statusCode, headers, json) {}
                HttpResponse(string headers, string body) 
                    : HttpResponse(this->statusCode, headers, body) {}
                HttpResponse(string headers, Json::Value json) 
                    : HttpResponse(this->statusCode, headers, json) {}
                HttpResponse(long statusCode) 
                    : HttpResponse(statusCode, this->body) {}
                HttpResponse(unordered_map<string, string> headers) 
                    : HttpResponse(this->statusCode, headers) {}
                HttpResponse(string body) 
                    : HttpResponse(this->statusCode, this->headers, body) {}
                HttpResponse(Json::Value json) 
                    : HttpResponse(this->statusCode, json) {}
                HttpResponse() 
                    : HttpResponse(this->statusCode) {}

                long getStatusCode();
                HttpResponse setStatusCode(long statusCode);
                string getBody();
                HttpResponse setBody(string body);
                Json::Value getJson();
                HttpResponse setJson(Json::Value json);
                unordered_map<string, string> getHeaders();
                HttpResponse setHeaders(unordered_map<string, string> headers);
                HttpResponse setHeaders(string headers);
        };
    }
}

#endif // FC_HTTPRESPONSE_H
