#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace orchid
{
    class Buffer
    {

    private:
        std::vector<uint8_t> buffer;

    public:
        Buffer() = default;
        Buffer(uint8_t* begin, uint8_t* end);

        void consume(std::size_t length);
        std::size_t size();
        uint8_t* data();
        void append(uint8_t* data, std::size_t length);
        void append(const std::string& string);
        void append(uint8_t value);
        uint8_t* begin();
        uint8_t* end();
    };
}