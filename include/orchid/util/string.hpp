#pragma once

#include <string>

#include <orchid/exception.hpp>

namespace orchid::util::string
{
    template <typename T>
    inline T as(const std::string& string)
    {
        if constexpr (std::is_same<T, uint64_t>::value)
        {
            return std::stoull(string);
        }
        else
        {
            static_assert("not implemented");
        }
        throw InvalidArgumentException();
    }

    inline std::string& lower(std::string& string)
    {
        for (auto& ch : string)
        {
            if (ch > 64 && ch < 91)
            {
                ch += 32;
            }
        }
        return string;
    }
}