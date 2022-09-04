#include <orchid/net/socket.hpp>

orchid::Socket::Socket()
{
    fd = ::socket(AF_INET, SOCK_STREAM, 0);
}

orchid::Socket::Socket(int fd)
{
    this->fd = fd;
}

orchid::Socket orchid::Socket::accept()
{
    return orchid::Socket(::accept(fd, nullptr, nullptr));
}

int orchid::Socket::bind(uint16_t port)
{
    const int reuse = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

    sockaddr_in address = {};
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    return ::bind(fd, (sockaddr*)(&address), sizeof(sockaddr));
}

int orchid::Socket::close()
{
    return ::shutdown(fd, SHUT_RDWR);
}

int orchid::Socket::connect(const std::string& hostname)
{
    hostname.substr(0);
    return ::connect(fd, nullptr, sizeof(sockaddr));
}

int orchid::Socket::listen(int backlog)
{
    return ::listen(fd, backlog);
}

std::string orchid::Socket::read(std::size_t length, int flags)
{
    if (length <= buffer.size())
    {
        auto r_buffer = std::string(buffer.data(), buffer.data() + length);
        buffer.consume(length);
        return r_buffer;
    }

    uint8_t* t_buffer = static_cast<uint8_t*>(std::malloc(length));
    int result = ::recv(fd, t_buffer, length, flags);
    if (result < 1)
    {
        std::free(t_buffer);
        throw std::runtime_error("socket disconnected");
    }
    buffer.insert(t_buffer, result);
    std::free(t_buffer);
    auto r_buffer = std::string(buffer.data(), buffer.data() + length);
    buffer.consume(length);
    return r_buffer;
}

std::string orchid::Socket::read_until(char ch, int flags)
{
    while (true)
    {
        auto position = std::find(buffer.data(), buffer.data() + buffer.size(), ch);
        if (position != buffer.end())
        {
            auto r_buffer = std::string(buffer.data(), position);
            buffer.consume(position - buffer.data());
            return r_buffer;
        }

        uint8_t* t_buffer = static_cast<uint8_t*>(std::malloc(2048));
        int result = ::recv(fd, t_buffer, 2048, flags);
        if (result < 1)
        {
            std::free(t_buffer);
            throw std::runtime_error("socket disconnected");
        }
        buffer.insert(t_buffer, result);
        std::free(t_buffer);
    }
}

int orchid::Socket::write(orchid::Buffer& data, int flags)
{
    return ::send(fd, data.data(), data.size(), flags);
}

std::string orchid::Socket::getAddress()
{
    sockaddr_in address = {};
    socklen_t len;
    auto result = ::getpeername(fd, (sockaddr*)(&address), &len);
    return std::string(::inet_ntoa(address.sin_addr));
}