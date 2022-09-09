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
#include <orchid/net/exception.hpp>

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
        bool connected = false;

        Socket(bool useSSL = true)
        {
            fd = ::socket(AF_INET, SOCK_STREAM, 0);

            if (!useSSL) 
            {
                return;
            }

            method = TLS_client_method();
            ctx = SSL_CTX_new(method);
            ssl = SSL_new(ctx);
            SSL_set_fd(ssl, fd);

            SSL_set_ciphersuites
            (
                ssl, 
                "TLS_CHACHA20_POLY1305_SHA256"
                ":TLS_AES_128_GCM_SHA256"
                ":TLS_AES_256_GCM_SHA384"
            );

            SSL_set_cipher_list
            (
                ssl,
                "ECDHE-ECDSA-CHACHA20-POLY1305"
                ":ECDHE-RSA-CHACHA20-POLY1305"
                ":ECDHE-ECDSA-AES128-GCM-SHA256"
                ":ECDHE-RSA-AES128-GCM-SHA256"
                ":ECDHE-ECDSA-AES256-GCM-SHA384"
                ":ECDHE-RSA-AES256-GCM-SHA384"
                ":ECDHE-ECDSA-AES128-SHA"
                ":ECDHE-RSA-AES128-SHA"
                ":ECDHE-ECDSA-AES256-SHA"
                ":ECDHE-RSA-AES256-SHA"
                ":AES128-GCM-SHA256"
                ":AES256-GCM-SHA384"
                ":AES128-SHA"
                ":AES256-SHA"
                ":DES-CBC3-SHA"
            );

            SSL_set1_sigalgs_list
            (
                ssl,
                "ecdsa_secp256r1_sha256"
                ":rsa_pss_rsae_sha256"
                ":rsa_pkcs1_sha256"
                ":ecdsa_secp384r1_sha384"
                ":rsa_pss_rsae_sha384"
                ":rsa_pkcs1_sha384"
                ":rsa_pss_rsae_sha512"
                ":rsa_pkcs1_sha512"
                ":rsa_pkcs1_sha1"
            );
        }

        ~Socket()
        {
            ::shutdown(fd, SHUT_RDWR);
            SSL_free(ssl);
            ssl = nullptr;
        }

        Socket accept()
        {
            Socket socket;
            socket.fd = ::accept(fd, nullptr, nullptr);

            if (!ssl)
            {
                return socket;
            }

            socket.ssl = SSL_new(ctx);
            SSL_set_fd(socket.ssl, socket.fd);
            SSL_accept(socket.ssl);
            return socket;
        }

        void bind(uint16_t port)
        {
            const int reuse = 1;
            setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int));

            sockaddr_in address = {};
            address.sin_family = AF_INET;
            address.sin_port = htons(port);
            if (::bind(fd, (sockaddr*)(&address), sizeof(sockaddr)))
            {
                throw PortInUseException();
            }
        }

        void close()
        {
            ::shutdown(fd, SHUT_RDWR);
        }

        int connect(const std::string& hostname)
        {
            this->hostname = hostname;

            addrinfo hints = {};
            addrinfo* result = nullptr;

            SSL_CTX_set_default_verify_paths(ctx);
            SSL_set_tlsext_host_name(ssl, hostname.c_str());

            if (getaddrinfo(hostname.c_str(), "443", &hints, &result))
            {
                freeaddrinfo(result);
            }

            while (true)
            {
                if (::connect(fd, result->ai_addr, sizeof(sockaddr)))
                {
                    result = result->ai_next;
                }
                else
                {
                    break;
                }
            }
            freeaddrinfo(result);

            SSL_set_fd(ssl, fd);
            SSL_connect(ssl);

            connected = true;

            return 0;
        }

        int listen(int backlog = 128)
        {
            return ::listen(fd, backlog);
        }

        std::string read(std::size_t length, int flags = 0)
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
                    ::shutdown(fd, SHUT_RDWR);
                    connected = false;
                    throw SocketClosedException();
                }
                buffer.append(t_buffer, result);
                std::free(t_buffer);

                if (buffer.size() >= length)
                {
                    break;
                }
            }
            
            auto r_buffer = std::string(buffer.data(), buffer.data() + length);
            buffer.consume(length);
            return r_buffer;
        }

        std::string read_until(char ch, int flags = 0)
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

                uint8_t t_buffer[2048];
                int transferred = 0;
                if (ssl)
                {
                    transferred = SSL_read(ssl, t_buffer, 2048);
                }
                else
                {
                    transferred = ::recv(fd, t_buffer, 2048, flags);
                }
                if (transferred < 1)
                {
                    ::shutdown(fd, SHUT_RDWR);
                    connected = false;
                    throw SocketClosedException();
                }
                buffer.append(t_buffer, transferred);
            }
        }

        int write(orchid::Buffer&& data, int flags = 0)
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

        int write(const std::string& data, int flags = 0)
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

        std::string getAddress()
        {
            sockaddr_in address = {};
            socklen_t len;
            [[maybe_unused]] auto result = ::getpeername(fd, (sockaddr*)(&address), &len);
            return std::string(::inet_ntoa(address.sin_addr));
        }
    };
}