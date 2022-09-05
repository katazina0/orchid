#pragma once

#include <map>
#include <string>

namespace orchid::http
{
    typedef std::map<std::string, std::string> Headers;
    typedef std::map<std::string, std::string> FormData;

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

    inline std::string& to_lowercase(std::string& string)
    {
        for (auto& ch : string)
            if (ch > 64 && ch < 91)
                ch += 32;
        return string;
    }

    struct URL
    {
        std::string protocol;
        std::string hostname;
        std::string endpoint;

        URL(const std::string& url)
        {
            auto p_end = url.find("://");
            protocol = url.substr(0, p_end);
            auto e_start = url.find('/', p_end + 3);
            hostname = url.substr(p_end + 3, e_start - p_end - 3);
            if (e_start == std::string::npos)
                endpoint = "/";
            else
                endpoint = url.substr(e_start);
        }
    };
}