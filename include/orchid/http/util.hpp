#pragma once

#include <map>
#include <string>

namespace orchid::http
{
    typedef std::map<std::string, std::string> Headers;

    enum class CODE
    {
        OK = 200,
        NOT_FOUND = 404
    };

    enum class METHOD
    {
        GET,
        POST,
        PUT,
        DELETE,
        PATCH
    };

    inline std::map<CODE, std::string> STATUS =
    {
        { CODE::OK,         "OK" },
        { CODE::NOT_FOUND,  "Not Found" }
    };

    inline std::map<std::string, std::string> MIME =
    {
        { "html",   "text/html" },
        { "js",     "text/javascript" },
        { "css",    "text/css" },
        { "ttf",    "application/octet-stream" }
    };
}