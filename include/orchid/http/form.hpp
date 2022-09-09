#pragma once

#include <string>
#include <vector>

#include <orchid/util/string.hpp>

using namespace orchid::util;

namespace orchid::http
{
    class Form
    {
        std::map<std::string, std::vector<std::string>> base;

    public:
        Form() = default;

        Form(const std::string& form)
        {
            uint64_t k_start = 0;
            while (true)
            {
                auto k_end = form.find('=', k_start + 1);
                if (k_end == std::string::npos) break;
                auto key = form.substr(k_start + 1, k_end - k_start - 1);
                auto value = form.substr(k_end + 1);
                k_start = form.find('&', k_start + 1);
                if (k_start != std::string::npos)
                {
                    add(key, value.substr(0, k_start - k_end - 1));
                    continue;
                }
                else
                {
                    add(key, value);
                    break;
                }
            }
        }

        template <typename T>
        void add(const std::string& key, const T& value)
        {
            if constexpr (std::is_same<T, std::string>::value)
            {
                base[key].push_back(value);
            }
            else
            {
                base[key].push_back(std::to_string(value));
            }
        }

        template <typename T = std::string>
        std::vector<T> values(const std::string& key)
        {
            std::vector<T> vector;
            for (uint64_t i = 0; i < base.size(); i++)
            {
                if constexpr (std::is_same<T, std::string>::value)
                {
                    vector.push_back(base[key]);
                }
                else
                {
                    vector.push_back(string::as<T>(base[key]));
                }
            }
            return vector;
        }

        template <typename T = std::string>
        T get(const std::string& key)
        {
            if constexpr (std::is_same<T, std::string>::value)
            {
                if (base[key].size() > 0)
                {
                    return base[key][0];
                }
                else
                {
                    throw InvalidRequestException();
                }
            }
            else
            {
                if (base[key].size() > 0)
                {
                    return string::as<T>(base[key][0]);
                }
                else
                {
                    throw InvalidRequestException();
                }
            }
        }

        bool contains(const std::string& key)
        {
            return base.contains(key);
        }

        std::string serialize()
        {
            std::string serialized;
            bool begin = true;
            for (auto& entry : base)
            {
                for (uint64_t i = 0; i < entry.second.size(); i++)
                {
                    if (!begin)
                    {
                        serialized.push_back('&');
                    }
                    else
                    {
                        serialized.push_back('?');
                        begin = false;
                    }
                    serialized.append(entry.first);
                    serialized.push_back('=');
                    serialized.append(entry.second[i]);
                }
            }
            return serialized;
        }
    };
}