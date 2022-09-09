#pragma once

#include <string>
#include <vector>

#include <orchid/http/mime.hpp>

namespace orchid::http
{
    class Body
    {
        std::string body;
        Mime mime = Mime::JSON;

    public:
        Body() = default;

        template <typename T>
        Body(const T& body, const Mime& mime = Mime::JSON)
        {
            this->mime = mime;
            if constexpr (std::is_array<T>::value)
            {
                this->body = body;
            }
            else
            {
                this->body = std::string(body.begin(), body.end());
            }
        }

        template <typename T>
        T as()
        {
            return T(body.begin(), body.end());
        }

        uint64_t size()
        {
            return body.size();
        }

        bool empty()
        {
            return body.empty();
        }

        void append(const std::string& string)
        {
            body.append(string);
        }

        void resize(uint64_t size)
        {
            body.resize(size);
        }

        char* data()
        {
            return body.data();
        }

        void setMime(const Mime& mime)
        {
            this->mime = mime;
        }

        const Mime& getMime()
        {
            return mime;
        }
    };
}