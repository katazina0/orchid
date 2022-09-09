#pragma once

#include <string>
#include <vector>

#include <orchid/exception.hpp>

namespace orchid::http
{
    class Mime
    {
        inline static const std::vector<std::string> extensions
        {
            "html",
            "css",
            "js",
            "ttf",
            "bin",
            "json",
            "png",
            "gif",
            "webm"
        };

        inline static const std::vector<std::string> contentTypes
        {
            "text/html",
            "text/css",
            "text/javascript",
            "application/octet-stream",
            "application/octet-stream",
            "application/json",
            "image/png",
            "image/gif",
            "video/webm"
        };

    public:
        enum MIME
        {
            HTML,
            JS,
            CSS,
            TTF,
            BIN,
            JSON,
            PNG,
            GIF,
            WEBM
        };

    public:
        inline static std::string extension(MIME mime)
        {
            return extensions[(int)mime];
        }

        inline static std::string contentType(MIME mime)
        {
            return contentTypes[(int)mime];
        }

        inline static MIME parse(const std::string& mime)
        {
            if (mime.find('/') != std::string::npos)
            {
                for (uint64_t i = 0; i < contentTypes.size(); i++)
                {
                    if (contentTypes[i] == mime)
                    {
                        return static_cast<MIME>(i);
                    }
                }
                return Mime::BIN;
            }
            else
            {
                for (uint64_t i = 0; i < extensions.size(); i++)
                {
                    if (extensions[i] == mime)
                    {
                        return static_cast<MIME>(i);
                    }
                }
                return Mime::BIN;
            }
        }
    };
}