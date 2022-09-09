#pragma once

#include <map>

#include <orchid/net/socket.hpp>

#include <orchid/http/response.hpp>
#include <orchid/http/request.hpp>

namespace orchid::http
{
    namespace impl
    {
        inline static std::map<std::string, Socket*> sockets;
        inline static std::map<std::string, Cookies> cookies;

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

        template <Method::METHOD method, typename... T>
        constexpr Response request(T&&... args)
        {
            Request request = Request(method, std::forward<T>(args)...);
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
    }

    Response REQUEST(Request&& request)
    {
        return impl::request<Method::GET>(std::forward<Request>(request));
    }

    template <typename... T>
    constexpr Response GET(T&&... args)
    {
        return impl::request<Method::GET>(std::forward<T>(args)...);
    }

    template <typename... T>
    constexpr Response DELETE(T&&... args)
    {
        return impl::request<Method::DELETE>(std::forward<T>(args)...);
    }

    template <typename... T>
    constexpr Response POST(T&&... args)
    {
        return impl::request<Method::POST>(std::forward<T>(args)...);
    }

    template <typename... T>
    constexpr Response PUT(T&&... args)
    {
        return impl::request<Method::PUT>(std::forward<T>(args)...);
    }
}