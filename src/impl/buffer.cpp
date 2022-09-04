#include <orchid/impl/buffer.hpp>

void orchid::Buffer::consume(std::size_t length)
{
    if (_buffer.size() < length)
        std::__throw_runtime_error("cannot consume more than the length of the buffer");

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