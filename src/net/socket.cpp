#include <orchid/net/socket.hpp>

orchid::Socket::Socket(bool ssl)
{
    this->fd = ::socket(AF_INET, SOCK_STREAM, 0);

    if (!ssl) 
        return;

    this->method = TLS_client_method();
    this->ctx = SSL_CTX_new(method);
    this->ssl = SSL_new(ctx);
}

orchid::Socket::Socket(int fd, bool ssl)
{
    this->fd = fd;

    if (!ssl) 
        return;

    this->method = TLS_client_method();
    this->ctx = SSL_CTX_new(method);
    this->ssl = SSL_new(ctx);
}

orchid::Socket orchid::Socket::accept()
{
    return orchid::Socket(::accept(fd, nullptr, nullptr), ssl);
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

    if (ssl)
    {
        SSL_CTX_free(ctx);
        SSL_free(ssl);
    }
}

int orchid::Socket::connect(const std::string& hostname)
{
    this->hostname = hostname;

	addrinfo hints = {};
    addrinfo* result = nullptr;

    SSL_CTX_set_default_verify_paths(ctx);
	SSL_set_tlsext_host_name(ssl, hostname.c_str());

    if (getaddrinfo(hostname.c_str(), "443", &hints, &result))
        freeaddrinfo(result);

    while (true)
    {
        auto err_connect = ::connect(fd, result->ai_addr, sizeof(sockaddr));
        if (err_connect)
            result = result->ai_next;
        else
            break;
    }
    freeaddrinfo(result);

    SSL_set_fd(ssl, fd);
	SSL_connect(ssl);

    return 0;
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

    while (true)
    {
        uint8_t* t_buffer = static_cast<uint8_t*>(std::malloc(length));
        int result = 0;
        if (ssl)
        {
            result = SSL_read(ssl, t_buffer, length);
        }
        else
        {
            result = ::recv(fd, t_buffer, length, flags);
        }
        if (result < 1)
        {
            std::free(t_buffer);
            close();
            throw std::runtime_error("socket disconnected");
        }
        buffer.append(t_buffer, result);
        std::free(t_buffer);

        if (buffer.size() >= length)
            break;
    }
    
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
        int result = 0;
        if (ssl)
        {
            result = SSL_read(ssl, t_buffer, 2048);
        }
        else
        {
            result = ::recv(fd, t_buffer, 2048, flags);
        }
        if (result < 1)
        {
            std::free(t_buffer);
            close();
            throw std::runtime_error("socket disconnected");
        }
        buffer.append(t_buffer, result);
        std::free(t_buffer);
    }
}

int orchid::Socket::write(orchid::Buffer& data, int flags)
{
    if (ssl)
    {
        return SSL_write(ssl, data.data(), data.size());
    }
    else
    {
        return ::send(fd, data.data(), data.size(), flags);
    }
}

int orchid::Socket::write(const std::string& data, int flags)
{
    if (ssl)
    {
        return SSL_write(ssl, data.data(), data.size());
    }
    else
    {
        return ::send(fd, data.data(), data.size(), flags);
    }
}

std::string orchid::Socket::getAddress()
{
    sockaddr_in address = {};
    socklen_t len;
    [[maybe_unused]] auto result = ::getpeername(fd, (sockaddr*)(&address), &len);
    return std::string(::inet_ntoa(address.sin_addr));
}