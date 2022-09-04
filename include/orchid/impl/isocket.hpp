#pragma once

#include "orchid/impl/buffer.hpp"

namespace orchid
{
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
}