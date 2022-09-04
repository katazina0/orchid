#include <orchid/net/buffer.hpp>

orchid::Buffer::Buffer(uint8_t* begin, uint8_t* end)
{
    buffer.insert(buffer.begin(), begin, end);
}

void orchid::Buffer::consume(std::size_t length)
{
    if (buffer.size() < length)
    {
        std::__throw_runtime_error("cannot consume more than the length of the buffer");
    }
    buffer.erase(buffer.begin(), buffer.begin() + length);
}

uint8_t* orchid::Buffer::data()
{
    return buffer.data();
}

std::size_t orchid::Buffer::size()
{
    return buffer.size();
}

void orchid::Buffer::insert(uint8_t* data, std::size_t length)
{
    buffer.insert(buffer.end(), data, data + length);
}

void orchid::Buffer::insert(const std::string& string)
{
    buffer.insert(buffer.end(), string.begin(), string.end());
}

void orchid::Buffer::push(uint8_t value)
{
    buffer.push_back(value);
}

uint8_t* orchid::Buffer::begin()
{
    return &*buffer.begin();
}

uint8_t* orchid::Buffer::end()
{
    return &*buffer.end();
}