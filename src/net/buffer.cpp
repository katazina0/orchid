#include <orchid/net/buffer.hpp>

orchid::Buffer::Buffer(uint8_t* begin, uint8_t* end)
{
    _buffer.insert(_buffer.begin(), begin, end);
}

void orchid::Buffer::consume(std::size_t length)
{
    if (_buffer.size() < length)
    {
        std::__throw_runtime_error("cannot consume more than the length of the buffer");
    }

    _buffer.erase(_buffer.begin(), _buffer.begin() + length);
}

uint8_t* orchid::Buffer::data()
{
    return _buffer.data();
}

std::size_t orchid::Buffer::size()
{
    return _buffer.size();
}

void orchid::Buffer::insert(uint8_t* data, std::size_t length)
{
    _buffer.insert(_buffer.end(), data, data + length);
}

void orchid::Buffer::push(uint8_t value)
{
    _buffer.push_back(value);
}

uint8_t* orchid::Buffer::begin()
{
    return &*_buffer.begin();
}

uint8_t* orchid::Buffer::end()
{
    return &*_buffer.end();
}