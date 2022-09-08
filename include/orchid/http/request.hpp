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
        Protocol::PROTOCOL protocol = Protocol::HTTP11;
        Method::METHOD method = Method::GET;
        URL url;
        Headers headers;
        Cookies cookies;
        Body body;

    public:
        Request() = default;

        Request(Socket& socket)
        {
            method = Method::parse(socket.read_until(' '));
            socket.read(1);
            url = socket.read_until(' ');
            socket.read(1);
            protocol = Protocol::parse(socket.read_until('\r'));
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

        void setMime(Mime::MIME mime)
        {
            body.setMime(mime);
        }

        Mime::MIME getMime()
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

        void setCookie(const std::string& key, const std::string& value)
        {
            cookies.set(key, value);
        }

        Cookie getCookie(const std::string& key)
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
            url.form.add(key, value);
        }

        template <typename T = std::string>
        T getForm(const std::string& key)
        {
            return url.form.get<T>(key);  
        }

        template <typename T = std::string>
        T getBody()
        {
            return body.as<T>();
        }

        template <typename T>
        void setBody(const T& body)
        {
            this->body = Body(body);
        }

        template <typename T = std::string>
        std::vector<T> getFormAll(const std::string& key)
        {
            return url.form.values<T>(key);
        }

        Buffer serialize()
        {
            headers["content-length"] = std::to_string(body.size());
            if (!body.empty() && !headers.contains("content-type"))
            {
                headers["content-type"] = "application/json";
            }

            Buffer buffer;
            buffer.append("GET");
            buffer.append(' ');
            buffer.append(url.endpoint);
            buffer.append(' ');
            buffer.append("HTTP/1.1");
            buffer.append('\r');
            buffer.append('\n');
            buffer.append(headers.serialize());
            buffer.append("cookie: ");
            buffer.append(cookies.serialize());
            buffer.append('\r');
            buffer.append('\n');
            buffer.append('\r');
            buffer.append('\n');
            buffer.append(body.as<std::string>());
            return buffer;
        }
    };
}