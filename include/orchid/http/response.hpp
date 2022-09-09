#pragma once

#include <map>
#include <filesystem>

#include <orchid/net/socket.hpp>

#include <orchid/http/body.hpp>
#include <orchid/http/cookies.hpp>
#include <orchid/http/exception.hpp>
#include <orchid/http/form.hpp>
#include <orchid/http/headers.hpp>
#include <orchid/http/method.hpp>
#include <orchid/http/mime.hpp>
#include <orchid/http/protocol.hpp>
#include <orchid/http/status.hpp>
#include <orchid/http/url.hpp>

namespace orchid::http
{
    class Response
    {
        Protocol::PROTOCOL protocol = Protocol::HTTP11;
        Status::STATUS status = Status::OK;
        Headers headers;
        Cookies cookies;
        Body body;

    public:
        Response() = default;

        Response(Socket& socket)
        {
            protocol = Protocol::parse(socket.read_until(' '));
            socket.read(1);
            status = static_cast<Status::STATUS>(std::stoull(socket.read_until(' ')));
            socket.read(1);
            socket.read_until('\r');
            socket.read(2);

            while (true)
            {
                auto header = socket.read_until('\r');
                socket.read(2);

                auto div = header.find(':');
                if (div == std::string::npos)
                {
                    break;
                }

                auto key = header.substr(0, div);
                string::lower(key);

                if (key != "set-cookie")
                {
                    headers[key] = header.substr(div + 2);
                }
                else
                {
                    cookies.add(Cookie(header.substr(div + 2)));
                }
            }

            if (headers.contains("content-length"))
            {
                uint length = std::stoul(headers["content-length"]);
                if (length == 0)
                {
                    return;
                }
                body = socket.read(length);
            }
            else if (headers.contains("transfer-encoding"))
            {
                while (true)
                {
                    auto chunk_size = socket.read_until('\r');
                    socket.read(2);

                    uint size = 0;
                    std::stringstream stream;
                    for (uint32_t i = 0; i < chunk_size.size(); i++)
                    {
                        stream << chunk_size[i];
                    }
                    stream >> std::hex >> size;
                    if (size == 0)
                    {
                        socket.read(2);
                        break;
                    }
                    auto chunk = socket.read(size);
                    body.append(chunk);
                    socket.read(2);
                }
            }

            if (headers.contains("connection") && headers["connection"] == "close")
            {
                socket.connected = false;
                socket.close();
            }
        }

        Response(const std::string& filename)
        {
            auto path = std::filesystem::canonical("/proc/self/exe").parent_path().append(filename.substr(1));
            FILE* file = fopen(path.c_str(), "rb");
            if (file == nullptr)
            {
                status = Status::NOT_FOUND;
            }
            else
            {
                status = Status::OK;

                fseek(file, 0, SEEK_END);
                std::size_t size = ftell(file);
                fseek(file, 0, SEEK_SET);
                
                body.resize(size);

                fread(body.data(), size, 1, file);
                fclose(file);

                headers.set("content-type", Mime::contentType(Mime::parse(path.extension().string().substr(1))));
            }
        }

        template <typename T>
        void addHeader(const std::string& key, const T& value)
        {
            headers.set(key, value);
        }

        template <typename T = std::string>
        T getHeader(const std::string& key)
        {
            return headers.get<T>(key);
        }

        bool hasHeader(const std::string& key)
        {
            return headers.contains(key);
        }

        void removeHeader(const std::string& key)
        {
            headers.remove(key);
        }

        void setCookie(const std::string& key, const std::string& value)
        {
            cookies.set(key, value);
        }

        Cookie getCookie(const std::string& key)
        {
            return cookies.get(key);
        }

        void setMime(Mime::MIME mime)
        {
            body.setMime(mime);
        }

        Mime::MIME getMime()
        {
            return body.getMime();
        }

        template <typename T = std::string>
        T getBody()
        {
            return body.as<T>();
        }

        template <typename T>
        void setBody(const T& body)
        {
            this->body = Body(body);
        }

        void setStatus(Status::STATUS status)
        {
            this->status = status;
        }

        Status::STATUS getStatus()
        {
            return status;
        }

        Buffer serialize()
        {
            headers.set("content-length", body.size());
            if (!body.empty() && !headers.contains("content-type"))
            {
                headers.set("content-type", Mime::contentType(body.getMime()));
            }

            Buffer buffer;
            buffer.append(Protocol::string(protocol));
            buffer.append(' ');
            buffer.append(std::to_string((int)status));
            buffer.append(' ');
            buffer.append(Status::string(status));
            buffer.append('\r');
            buffer.append('\n');
            buffer.append(headers.serialize());
            buffer.append('\r');
            buffer.append('\n');
            buffer.append(body.as<std::string>());
            return buffer;
        }
    };
}