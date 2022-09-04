#pragma once

#include <orchid/impl/buffer.hpp>

namespace orchid
{
    class ISocket
    {

    public:
        int fd = 0;
        orchid::Buffer buffer = orchid::Buffer();

        virtual int accept() = 0;
        virtual int bind(uint16_t port) = 0;
        virtual int close() = 0;
        virtual int connect(const std::string& hostname) = 0;
        virtual int listen(int backlog = 128) = 0;
        virtual int read(std::size_t length, int flags = 0) = 0;
        virtual int write(orchid::Buffer& buffer, int flags = 0) = 0;
    };
}