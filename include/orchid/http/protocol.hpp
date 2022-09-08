#pragma once

#include <string>

#include <orchid/exception.hpp>

using namespace orchid::util;

namespace orchid::http
{
    class Protocol
    {
        inline static const std::vector<std::string> protocols
        {
            "HTTP/0.9",
            "HTTP/1.0",
            "HTTP/1.1",
            "HTTP/2.0"
        };

    public:
        enum PROTOCOL
        {
            HTTP09,
            HTTP10,
            HTTP11,
            HTTP20
        };

        inline static std::string string(PROTOCOL protocol)
        {
            return protocols[(int)protocol];
        }

        inline static PROTOCOL parse(const std::string& protocol)
        {
            for (uint64_t i = 0; i < protocols.size(); i++)
            {
                if (protocols[i] == protocol)
                {
                    return static_cast<PROTOCOL>(i);
                }
            }
            throw InvalidArgumentException();
        }
    };
}