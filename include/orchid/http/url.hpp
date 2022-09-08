#pragma once

#include <string>

#include <orchid/http/form.hpp>

namespace orchid::http
{
    class URL
    {
        void init(const std::string& url)
        {
            if (url[0] == '/')
            {
                auto s_form = url.find('?');
                if (s_form != std::string::npos)
                {
                    form = Form(url.substr(s_form));
                }
                endpoint = url.substr(0, s_form);
            }
            else
            {
                auto p_end = url.find("://");
                protocol = url.substr(0, p_end);
                auto e_start = url.find('/', p_end + 3);
                hostname = url.substr(p_end + 3, e_start - p_end - 3);
            }
        }

    public:
        std::string protocol;
        std::string hostname;
        std::string endpoint;
        Form form;

        URL() = default;

        URL(const std::string& url)
        {
            init(url);
        }

        URL(const char* url)
        {
            init(url);
        }
    };
}