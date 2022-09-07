#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

#include <algorithm>
#include <string>
#include <stdexcept>

#include <orchid/net/buffer.hpp>

#include <openssl/ssl.h>

namespace orchid
{
    class Socket
    {

    public:
        const SSL_METHOD* method = nullptr;
        SSL_CTX* ctx = nullptr;
        SSL* ssl = nullptr;

        int fd = 0;
        Buffer buffer;
        std::string hostname;

        Socket(bool ssl = true);
        Socket(int fd, bool ssl = true);

        Socket accept();
        int bind(uint16_t port);
        int listen(int backlog = 128);

        void close();
        int connect(const std::string& hostname);
        std::string getAddress();

        std::string read(std::size_t length, int flags = 0);
        std::string read_until(char ch, int flags = 0);

        int write(Buffer&& buffer, int flags = 0);
        int write(const std::string& data, int flags = 0);
    };
}