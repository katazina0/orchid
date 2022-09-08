#pragma once

#include <string>
#include <vector>

using namespace orchid::util;

namespace orchid::http
{
    class Method
    {
        inline static const std::vector<std::string> methods
        {
            "GET",
            "HEAD",
            "POST",
            "PUT",
            "DELETE",
            "CONNECT",
            "OPTIONS",
            "TRACE",
            "PATCH"
        };

    public:
        enum METHOD
        {
            GET,
            HEAD,
            POST,
            PUT,
            DELETE,
            CONNECT,
            OPTIONS,
            TRACE,
            PATCH
        };

        inline static std::string string(METHOD method)
        {
            return methods[(int)method];
        }

        inline static METHOD parse(const std::string& method)
        {
            for (uint64_t i = 0; i < methods.size(); i++)
            {
                if (methods[i] == method)
                {
                    return static_cast<METHOD>(i);
                }
            }
            throw InvalidArgumentException();
        }
    };
}