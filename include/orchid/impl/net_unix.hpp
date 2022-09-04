#pragma once

#include <sys/socket.h>
#include <netinet/in.h>

#include <orchid/impl/isocket.hpp>

namespace orchid
{
    class Socket : orchid::ISocket
    {

    public:

        Socket();

        int accept() override;
        int bind(uint16_t port) override;
        int close() override;
        int connect(const std::string& hostname) override;
        int listen(int backlog = 128) override;
        int read(std::size_t length, int flags = 0) override;
        int write(orchid::Buffer& buffer, int flags = 0) override;

    };
}