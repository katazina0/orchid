#pragma once

#include <map>

#include <orchid/util/string.hpp>

#include <orchid/net/socket.hpp>

#include <orchid/http/body.hpp>
#include <orchid/http/cookies.hpp>
#include <orchid/http/exception.hpp>
#include <orchid/http/form.hpp>
#include <orchid/http/headers.hpp>
#include <orchid/http/method.hpp>
#include <orchid/http/protocol.hpp>
#include <orchid/http/url.hpp>

namespace orchid::http
{
    class Request
    {
        Method::METHOD method = Method::GET;
        URL url;
        Form form;
        Headers headers;
        Cookies cookies;
        Body body;

    public:
        Request() = default;

        template <typename... T>
        constexpr Request(Method::METHOD method, T&&... args)
        {
            this->method = method;
            ([&] constexpr
            {
                if constexpr (std::is_same<T, Method::METHOD>::value)
                {
                    this->method = method;
                }
                else if constexpr (std::is_same<T, URL>::value)
                {
                    url = std::forward<URL>(args);
                }
                else if constexpr (std::is_same<T, Headers>::value)
                {
                    headers = std::forward<Headers>(args);
                }
                else if constexpr (std::is_same<T, Cookies>::value)
                {
                    cookies = std::forward<Cookies>(args);
                }
                else if constexpr (std::is_same<T, Body>::value)
                {
                    body = std::forward<Body>(args);
                }
                else
                {
                    static_assert("");
                }
            }(), ...);
        }

        explicit Request(Socket& socket)
        {
            method = Method::parse(socket.read_until(' '));
            socket.read(1);
            auto fullUrl = socket.read_until(' ');
            auto s_form = fullUrl.find('?');
            if (s_form != std::string::npos)
            {
                form = Form(fullUrl.substr(s_form));
                url = fullUrl.substr(0, s_form);
            }
            else
            {
                url = fullUrl;
            }
            socket.read(1);
            socket.read_until('\r');
            socket.read(2);

            while (true)
            {
                auto header = socket.read_until('\r');
                socket.read(2);
                auto div = header.find(':');
                if (div == std::string::npos)
                {
                    break;
                }
                auto key = header.substr(0, div);
                string::lower(key);
                if (key != "cookie")
                {
                    headers[key] = header.substr(div + 2);
                }
                else
                {
                    cookies = Cookies(header.substr(div + 2));
                }
            }

            if (headers.contains("content-length"))
            {
                body = socket.read(std::stoul(headers["content-length"]));
                if (headers.contains("content-type"))
                {
                    body.setMime(Mime::parse(headers["content-type"]));
                }
                else
                {
                    body.setMime(Mime::BIN);
                }
            }
        }

        void setMime(const Mime& mime)
        {
            body.setMime(mime);
        }

        const Mime& getMime()
        {
            return body.getMime();
        }

        template <typename T>
        void addHeader(const std::string& key, const T& value)
        {
            headers.set(key, value);
        }

        template <typename T = std::string>
        T getHeader(const std::string& key)
        {
            return headers.get<T>(key);
        }

        void setHeaders(Headers&& headers)
        {
            this->headers = std::forward<Headers>(headers);
        }

        void setCookie(const std::string& key, const std::string& value)
        {
            cookies.set(key, value);
        }

        void setCookies(Cookies&& cookies)
        {
            this->cookies = std::forward<Cookies>(cookies);
        }

        const Cookie& getCookie(const std::string& key)
        {
            return cookies.get(key);
        }

        void setEndpoint(const std::string& endpoint)
        {
            url.endpoint = endpoint;
        }

        std::string getEndpoint()
        {
            return url.endpoint;
        }

        template <typename T>
        void addForm(const std::string& key, const T& value)
        {
            form.add(key, value);
        }

        template <typename T = std::string>
        T getForm(const std::string& key)
        {
            return form.get<T>(key);  
        }

        URL getURL()
        {
            return url;
        }

        void setURL(const URL& url)
        {
            this->url = url;
        }

        template <typename T = std::string>
        T getBody()
        {
            return body.as<T>();
        }
    
        void setMethod(Method::METHOD method)
        {
            this->method = method;
        }

        bool hasForm(const std::string& key)
        {
            return form.contains(key);
        }

        void setBody(Body&& body)
        {
            this->body = std::forward<Body>(body);
        }

        template <typename T>
        void setBody(const T& body)
        {
            this->body = Body(body);
        }

        template <typename T = std::string>
        std::vector<T> getFormAll(const std::string& key)
        {
            return form.values<T>(key);
        }

        Buffer serialize()
        {
            if (method != Method::GET)
            {
                headers["content-length"] = std::to_string(body.size());
                if (!body.empty() && !headers.contains("content-type"))
                {
                    headers["content-type"] = "application/json";
                }
            }

            Buffer buffer;
            buffer.append(Method::string(method));
            buffer.append(' ');
            buffer.append(url.endpoint);
            buffer.append(' ');
            buffer.append(Protocol::string(Protocol::HTTP11));
            buffer.append('\r');
            buffer.append('\n');
            buffer.append(headers.serialize());
            if (!cookies.empty())
            {
                buffer.append("cookie: ");
                buffer.append(cookies.serialize());
                buffer.append('\r');
                buffer.append('\n');
            }
            buffer.append('\r');
            buffer.append('\n');
            if (method != Method::GET)
            {
                buffer.append(body.as<std::string>());
            }
            return buffer;
        }
    };
}