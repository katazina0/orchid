#include <orchid/impl/net_unix.hpp>

orchid::Socket::Socket()
{
    fd = ::socket(AF_INET, SOCK_STREAM, 0);
}

int orchid::Socket::accept()
{
    return ::accept(fd, nullptr, nullptr);
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
    return ::connect(fd, nullptr, sizeof(sockaddr));
}

int orchid::Socket::listen(int backlog)
{
    return ::listen(fd, backlog);
}

int orchid::Socket::read(std::size_t length, int flags)
{
    uint8_t* t_buffer = static_cast<uint8_t*>(std::malloc(length));
    int result = ::recv(fd, t_buffer, length, flags);
    buffer.insert(t_buffer, length);
    return result;
}

int orchid::Socket::write(orchid::Buffer& data, int flags)
{
    return ::send(fd, data.data(), data.size(), flags);
}