#pragma once

#include <string>

namespace orchid::http
{
    class Cookie
    {

    public:
        std::string key;
        std::string value;
        std::string path;
        bool sameSite;
        bool httpOnly = false;
        bool secure = false;

        Cookie() = default;

        Cookie(const std::string& cookie)
        {
            auto s_value = cookie.find('=');
            key = cookie.substr(0, s_value);
            auto e_value = cookie.find(';');
            value = cookie.substr(s_value + 1, e_value - s_value - 1);
        }

        Cookie(const std::string& key, const std::string& value)
        {
            this->key = key;
            this->value = value;
        }
    };

    class Cookies
    {
        std::vector<Cookie> base;

    public:
        Cookies() = default;

        Cookies(const std::string& cookies)
        {
            uint64_t offset = 0;
            while (true)
            {
                Cookie cookie;
                auto s_value = cookies.find('=', offset);
                if (s_value == std::string::npos)
                {
                    break;
                }
                cookie.key = cookies.substr(offset, s_value - offset);
                auto e_value = cookies.find(';', offset);
                cookie.value = cookies.substr(s_value + 1, e_value - s_value - 1);
                offset = e_value + 2;
                base.push_back(cookie);
            }
        }

        void set(const std::string& key, const std::string& value)
        {
            for (uint64_t i = 0; i < base.size(); i++)
            {
                if (base[i].key == key)
                {
                    base[i].value = value;
                }
            }
            base.push_back(Cookie(key, value));
        }

        void add(const Cookie& cookie)
        {
            base.push_back(cookie);
        }

        Cookie get(const std::string& key)
        {
            for (uint64_t i = 0; i < base.size(); i++)
            {
                if (base[i].key == key)
                {
                    return base[i];
                }
            }
            throw InvalidArgumentException();
        }

        Cookie& operator[](const std::string& key)
        {
            for (uint64_t i = 0; i < base.size(); i++)
            {
                if (base[i].key == key)
                {
                    return base[i];
                }
            }
            base.push_back(Cookie(key, std::string()));
            return base[base.size() - 1];
        }

        template <bool SET_COOKIES = false>
        std::string serialize()
        {
            if constexpr (SET_COOKIES)
            {
                std::string serialized;
                static_assert("");
                return serialized;
            }
            else
            {
                std::string serialized;
                for (uint64_t i = 0; i < base.size(); i++)
                {
                    if (!base[i].value.empty())
                    {
                        serialized.append(base[i].key + "=" + base[i].value + "; ");
                    }
                }
                return serialized;
            }
        }
    };
}