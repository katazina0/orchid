#pragma once

#include <sys/socket.h>
#include <netinet/in.h>

#include "orchid/impl/buffer.hpp"

class ISocket
{
    virtual orchid::Buffer recv() = 0;
    virtual orchid::Buffer peek() = 0;
    virtual void send() = 0;
    virtual void close() = 0;
    virtual void listen() = 0;
    virtual void bind() = 0;
    virtual void accept() = 0;
    virtual void connect() = 0;
};