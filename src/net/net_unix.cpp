#include <orchid/net/net_unix.hpp>

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
    return ::connect(fd, nullptr, sizeof(sockaddr));
}

int orchid::Socket::listen(int backlog)
{
    return ::listen(fd, backlog);
}

int orchid::Socket::write(orchid::Buffer& data, int flags)
{
    return ::send(fd, data.data(), data.size(), flags);
}