#include <orchid/http/response.hpp>

orchid::http::Response::Response(Socket& socket)
{
    protocol = socket.read_until(' ');
    socket.read(1);
    code = static_cast<CODE>(std::stoul(socket.read_until(' ')));
    socket.read(1);
    status = socket.read_until('\r');
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
        to_lowercase(key);
        headers[key] = header.substr(div + 2);
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
    else
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
            body.insert(body.end(), chunk.begin(), chunk.end());
            socket.read(2);
        }
    }
}

orchid::Buffer orchid::http::Response::serialize()
{
    headers["content-length"] = std::to_string(body.size());
    if (!body.empty() && !headers.contains("content-type"))
    {
        headers["content-type"] = "application/octet-stream";
    }

    Buffer buffer;
    buffer.append(protocol);
    buffer.append(' ');
    buffer.append(std::to_string(static_cast<int>(code)));
    buffer.append(' ');
    buffer.append(status);
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

orchid::http::Response::Response(orchid::http::CODE code)
{
    this->code = code;
    this->status = STATUS[code];
}

orchid::http::Response::Response(const std::string& filename)
{
    auto path = std::filesystem::canonical("/proc/self/exe").parent_path().append(filename.substr(1));
    FILE* file = fopen(path.c_str(), "rb");
    if (file == nullptr)
    {
        this->code = CODE::NOT_FOUND;
        this->status = STATUS[CODE::NOT_FOUND];
    }
    else
    {
        this->code = CODE::OK;
        this->status = STATUS[CODE::OK];

        fseek(file, 0, SEEK_END);
        std::size_t size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        body.resize(size);

        fread(body.data(), size, 1, file);
        fclose(file);

        headers["content-type"] = MIME[path.extension().string().substr(1)];
    }
}