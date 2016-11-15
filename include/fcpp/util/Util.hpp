#ifndef FC_UTIL_H
#define FC_UTIL_H

#include <string>

#include "json/json.h"

using namespace std;

namespace fcpp
{
    namespace util
    {
        class Util
        {
            private:


            public:

                // Trims a string (both sides, spaces, tabs and endlines)
                static string trim(string s);

                // Parses a Json string into Json Object
                static Json::Value parseJson(string s);
        };
    }
}

#endif // FC_UTIL_H
