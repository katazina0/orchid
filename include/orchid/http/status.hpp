#pragma once

#include <string>
#include <map>

#include <orchid/exception.hpp>

namespace orchid::http
{
    class Status
    {

    public:
        enum STATUS
        {
            OK = 200,
            UNAUTHORIZED = 401,
            FORBIDDEN = 403,
            NOT_FOUND = 404,
            INTERNAL_SERVER_ERROR = 500
        };

    private:
        inline static std::map<STATUS, std::string> statuses
        {
            { STATUS::OK,                     "OK" },
            { STATUS::UNAUTHORIZED,           "Unauthorized" },
            { STATUS::FORBIDDEN,              "Forbidden" },
            { STATUS::NOT_FOUND,              "Not Found" },
            { STATUS::INTERNAL_SERVER_ERROR,  "Internal Server Error" }
        };

    public:
        inline static std::string string(STATUS status)
        {
            return statuses[status];
        }
    };
}