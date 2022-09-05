#include <orchid/http/request.hpp>

orchid::http::Request::Request(Socket& socket, bool read_form)
{
    method = socket.read_until(' ');
    socket.read(1);
    endpoint = socket.read_until(' ');
    socket.read(1);
    protocol = socket.read_until('\r');
    socket.read(2);


    if (read_form)
    {
        uint32_t offset = 0;
        auto k_start = endpoint.find('?');
        if (k_start != std::string::npos)
        {
            while (true)
            {
                auto k_end = endpoint.find('=', offset);
                if (k_end == std::string::npos) break;

                auto key = endpoint.substr(k_start + 1, k_end - k_start - 1);

                /*
                if (key == "url")
                {
                    formData[key] = endpoint.substr(k_end + 1);
                    endpoint = endpoint.substr(0, endpoint.find('?'));
                    break;
                }
                */
                auto value = endpoint.substr(k_end + 1);

                k_start = endpoint.find('&', offset);
                if (k_start != std::string::npos)
                {
                    formData[key] = value.substr(0, k_start - k_end - 1);
                    offset = k_start + 1;
                    continue;
                }
                else
                {
                    formData[key] = value;
                    break;
                }
            }
            endpoint = endpoint.substr(0, endpoint.find('?'));
        }
    }

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
        to_lowercase(key);
        headers[key] = header.substr(div + 2);
    }

    if (method == "GET" || method == "DELETE")
    {
        return;
    }

    if (headers.contains("content-length"))
    {
        body = socket.read(std::stoul(headers["content-length"]));
    }
    else
    {
        throw std::runtime_error("ill-formed request");
    }
}

orchid::Buffer orchid::http::Request::serialize()
{
    headers["content-length"] = std::to_string(body.size());
    if (!body.empty() && !headers.contains("content-type"))
    {
        headers["content-type"] = "application/json";
    }

    Buffer buffer;
    buffer.append(method);
    buffer.append(' ');
    buffer.append(endpoint);
    buffer.append(' ');
    buffer.append(protocol);
    buffer.append('\r');
    buffer.append('\n');
    for (auto& header : headers)
    {
        buffer.append(header.first);
        buffer.append(':');
        buffer.append(' ');
        buffer.append(header.second);
        buffer.append('\r');
        buffer.append('\n');
    }
    buffer.append('\r');
    buffer.append('\n');
    buffer.append(body);
    return buffer;
}