#pragma once

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <initializer_list>

#include <orchid/exception.hpp>

#include <orchid/util/string.hpp>

using namespace orchid::util;

namespace orchid::http
{
    class Headers
    {
        std::map<std::string, std::string> base;

    public:
        Headers() = default;

        Headers(std::initializer_list<std::pair<std::string, std::string>> args)
        {
            for (auto& arg : args)
            {
                base[arg.first] = arg.second;
            }
        }

        template <typename T>
        void set(const std::string& key, const T& value)
        {
            if constexpr (std::is_same<T, std::string>::value)
            {
                base[key] = value;
            }
            else if constexpr (std::is_array<T>::value)
            {
                base[key] = value;
            }
            else
            {
                base[key] = std::to_string(value);
            }
        }

        template <typename T = std::string>
        T get(const std::string& key)
        {
            if constexpr (std::is_same<T, std::string>::value)
            {
                return base[key];
            }
            else
            {
                return string::as<T>(base[key]);
            }
        }

        bool contains(const std::string& key)
        {
            return base.contains(key);
        }

        void remove(const std::string& key)
        {
            base.erase(key);
        }

        std::string serialize()
        {
            std::string serialized;
            for (auto& header : base)
            {
                serialized.append(header.first);
                serialized.push_back(':');
                serialized.push_back(' ');
                serialized.append(header.second);
                serialized.push_back('\r');
                serialized.push_back('\n');
            }
            return serialized;
        }

        std::string& operator[](const std::string& key)
        {
            return base[key];
        }
    };
}