#pragma once

#include <map>

#include <orchid/net/socket.hpp>

#include <orchid/http/response.hpp>
#include <orchid/http/request.hpp>

namespace orchid::http
{
    class Client
    {
        std::map<std::string, Socket*> sockets;
        std::map<std::string, Cookies> cookies;

        void connectHostname(const std::string& hostname)
        {
            for (auto& pair : sockets)
            {
                if (pair.first == hostname && pair.second->connected)
                {
                    return;
                }
            }
            sockets.emplace(hostname, new Socket());
            sockets[hostname]->connect(hostname);
        }

        consteval bool shouldHaveBody(Method::METHOD method)
        {
            return method == Method::GET;
        }

        template <Method::METHOD method = Method::GET, typename... T>
        constexpr Response request(T&&... args)
        {
            Request request;
            request.setMethod(method);

            ([&] constexpr
            {
                if constexpr (std::is_same<T, URL>::value)
                {
                    request.setURL(std::forward<URL>(args));
                }
                else if constexpr (std::is_same<T, Headers>::value)
                {
                    request.setHeaders(std::forward<Headers>(args));
                }
                else if constexpr (std::is_same<T, Cookies>::value)
                {
                    request.setCookies(std::forward<Cookies>(args));
                }
                else if constexpr (std::is_same<T, Body>::value)
                {
                    request.setBody(std::forward<Body>(args));
                }
                else
                {
                    static_assert("");
                }
            }(), ...);

            auto url = request.getURL();
            if (url.hostname == "")
            {
                throw InvalidArgumentException();
            }
            request.addHeader("host", url.hostname);
            connectHostname(url.hostname);
            sockets[url.hostname]->write(request.serialize());
            return Response(*sockets[url.hostname]);
        }

    public:
        ~Client()
        {
            for (auto& pair : sockets)
            {
                delete pair.second;
            }
        }

        template <typename... T>
        constexpr Response GET(T&&... args)
        {
            return request<Method::GET>(std::forward<T>(args)...);
        }

        template <typename... T>
        constexpr Response DELETE(T&&... args)
        {
            return request<Method::DELETE>(std::forward<T>(args)...);
        }

        template <typename... T>
        constexpr Response POST(T&&... args)
        {
            return request<Method::POST>(std::forward<T>(args)...);
        }

        template <typename... T>
        constexpr Response PUT(T&&... args)
        {
            return request<Method::PUT>(std::forward<T>(args)...);
        }
    };
}