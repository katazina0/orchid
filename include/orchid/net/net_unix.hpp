#pragma once

#include <sys/socket.h>
#include <netinet/in.h>

#include <algorithm>
#include <string>
#include <stdexcept>

#include <orchid/net/buffer.hpp>

namespace orchid
{
    class Socket
    {

    public:

        int fd = 0;
        orchid::Buffer buffer;

        Socket();
        Socket(int fd);

        orchid::Socket accept();
        int bind(uint16_t port);
        int close();
        int connect(const std::string& hostname);
        int listen(int backlog = 128);

        template <typename T = std::string>
        T read(std::size_t length, int flags = 0)
        {
            if (length <= buffer.size())
            {
                auto r_buffer = T(buffer.data(), buffer.data() + length);
                buffer.consume(length);
                return r_buffer;
            }

            uint8_t* t_buffer = static_cast<uint8_t*>(std::malloc(length));
            int result = ::recv(fd, t_buffer, length, flags);
            if (result < 1)
            {
                std::free(t_buffer);
                throw std::runtime_error("socket disconnected");
            }
            buffer.insert(t_buffer, result);
            std::free(t_buffer);
            auto r_buffer = T(buffer.data(), buffer.data() + length);
            buffer.consume(length);
            return r_buffer;
        }

        template <typename T = std::string>
        T read_until(char ch, int flags = 0)
        {
            while (true)
            {
                auto position = std::find(buffer.data(), buffer.data() + buffer.size(), ch);
                if (position != buffer.end())
                {
                    auto r_buffer = T(buffer.data(), position);
                    buffer.consume(position - buffer.data());
                    return r_buffer;
                }

                uint8_t* t_buffer = static_cast<uint8_t*>(std::malloc(2048));
                int result = ::recv(fd, t_buffer, 2048, flags);
                if (result < 1)
                {
                    std::free(t_buffer);
                    throw std::runtime_error("socket disconnected");
                }
                buffer.insert(t_buffer, result);
                std::free(t_buffer);
            }
        }

        int write(orchid::Buffer& buffer, int flags = 0);

    };
}