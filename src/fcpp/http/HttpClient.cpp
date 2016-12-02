#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include <curl/curl.h>

#include "fcpp/http/HttpResponse.hpp"
#include "fcpp/http/HttpClient.hpp"

using namespace std;
using namespace fcpp::http;

// CURL write callback
size_t HttpClient::writeCallback(void *buffer, size_t size, size_t nmemb, void *userp)
{
    // Calculate real size
    size_t realsize = size * nmemb;

    // Cast output data to string
    string *ss = (string*)userp;

    // Append current chunk to output data
    ss->append((char*)buffer, realsize);

    // Return the processed size
    return realsize;
}

// HTTP GET method 
HttpResponse HttpClient::get(string url, unordered_map<string, string> headers)
{
    // Declare response code
    long resCode;

    // Declare response headers
    string resHeaders;

    // Declare response body
    string resBody;

    // Global initialization
    curl_global_init(CURL_GLOBAL_DEFAULT);

    // Get request handle
    CURL *req = curl_easy_init();

    // Check we got request handle
    if (req) {

        // Set url
        curl_easy_setopt(req, CURLOPT_URL, url.c_str());

        // Initialize request headers list
        struct curl_slist *reqHeaders = nullptr;
    
        // Loop over request headers in map and add them to list
        // TODO: Abstract this into Util better
        for (auto h : headers) {
            stringstream ss;
            ss << h.first << ": " << h.second;
            const string& tmp = ss.str();  
            reqHeaders = curl_slist_append(reqHeaders, tmp.c_str());
        }
    
        // Set headers into cUrl request
        curl_easy_setopt(req, CURLOPT_HTTPHEADER, reqHeaders);

        // SetcUrlit to be verbose
        // TODO: Remove/comment this whenever fully tested
        // curl_easy_setopt(req, CURLOPT_VERBOSE, 1L);

        // Follow redirects
        curl_easy_setopt(req, CURLOPT_FOLLOWLOCATION, 1L);

        // Don't verify SSL certs
        // ACTHUNG! This is insecure!!
        curl_easy_setopt(req, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(req, CURLOPT_SSL_VERIFYHOST, 0L);

        // Headers write callback and data
        curl_easy_setopt(req, CURLOPT_HEADERFUNCTION, writeCallback);
        curl_easy_setopt(req, CURLOPT_HEADERDATA, &resHeaders);

        // Body write callback and data
        curl_easy_setopt(req, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(req, CURLOPT_WRITEDATA, &resBody);

        // User agent ... mmm should be removed or hacked just in case?
        curl_easy_setopt(req, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        // Perform the request, resCode will get the return code
        CURLcode code = curl_easy_perform(req);

        // Check for errors in request, otherwise get real status code
        if (code != CURLE_OK) {
            cout << "Error downloading from URL \"" << url << "\": " << curl_easy_strerror(code) << endl;
            HttpResponse dummy;
            return move(dummy);
        } else {
            curl_easy_getinfo(req, CURLINFO_RESPONSE_CODE, &resCode);
        }

        // Always cleanup 
        curl_easy_cleanup(req);

        // Cleanup the request headers list
        curl_slist_free_all(reqHeaders);
    }

    // Global cleanup
    curl_global_cleanup();

    // Create HTTP Response
    HttpResponse res(resCode, resHeaders, resBody);

    // Move ref ... not sure if needed
    return move(res);
}
