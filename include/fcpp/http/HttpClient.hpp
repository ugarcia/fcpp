#ifndef FC_HTTPCLIENT_H
#define FC_HTTPCLIENT_H

#include <string>
#include <unordered_map>

#include "fcpp/http/HttpResponse.hpp"

using namespace std;

namespace fcpp
{
    namespace http
    {
        class HttpClient
        {
            private:

                // CURL write callback
                static size_t writeCallback(void *buffer, size_t size, size_t nmemb, void *userp);

            public:

                // HTTP GET method               
                static HttpResponse get(string url, unordered_map<string, string> headers);
        };
    }
}

#endif // FC_HTTPCLIENT_H
