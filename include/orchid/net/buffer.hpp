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

        Buffer(uint8_t* begin, uint8_t* end)
        {
            buffer.insert(buffer.begin(), begin, end);
        }

        void consume(std::size_t length)
        {
            if (buffer.size() < length)
            {
                std::__throw_runtime_error("cannot consume more than the length of the buffer");
            }
            buffer.erase(buffer.begin(), buffer.begin() + length);
        }

        uint8_t* data()
        {
            return buffer.data();
        }

        uint64_t size()
        {
            return buffer.size();
        }

        void append(uint8_t* data, std::size_t length)
        {
            buffer.insert(buffer.end(), data, data + length);
        }

        void append(const std::string& string)
        {
            buffer.insert(buffer.end(), string.begin(), string.end());
        }

        void append(uint8_t value)
        {
            buffer.push_back(value);
        }

        uint8_t* begin()
        {
            return &*buffer.begin();
        }

        uint8_t* end()
        {
            return &*buffer.end();
        }
    };
}