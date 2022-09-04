#include "orchid/impl/buffer.hpp"

void orchid::Buffer::consume(std::size_t length)
{
    if (_buffer.size() < length)
        std::__throw_runtime_error("cannot consume more than the length of the buffer");

    _buffer.erase(_buffer.begin(), _buffer.begin() + length);
}