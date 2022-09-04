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
        std::string read(std::size_t length, int flags = 0);
        std::string read_until(char ch, int flags = 0);
        int write(orchid::Buffer& buffer, int flags = 0);

    };
}