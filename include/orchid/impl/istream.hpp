#pragma once

#include "buffer.hpp"

namespace orchid
{
    class IStream
    {
        virtual orchid::Buffer read() = 0;
        virtual orchid::Buffer peek() = 0;
        virtual void write() = 0;
    };
}