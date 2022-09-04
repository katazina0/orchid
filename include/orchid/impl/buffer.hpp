#pragma once

#include <cstdint>
#include <deque>

namespace orchid
{
    class Buffer
    {

    private:
        std::deque<std::uint8_t> _buffer;

    public:
        void consume(std::size_t length);
        
    };
}