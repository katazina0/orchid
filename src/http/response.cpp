#include <orchid/http/response.hpp>

orchid::Buffer orchid::http::Response::serialize()
{
    headers["content-length"] = std::to_string(body.size());

    Buffer buffer;

    buffer.insert(protocol);
    buffer.push(' ');
    buffer.insert(std::to_string(static_cast<int>(code)));
    buffer.push(' ');
    buffer.insert(status);

    buffer.push('\r');
    buffer.push('\n');

    for (auto& header : headers)
    {
        buffer.insert(header.first);
        buffer.push(':');
        buffer.push(' ');
        buffer.insert(header.second);
        buffer.push('\r');
        buffer.push('\n');
    }

    buffer.push('\r');
    buffer.push('\n');
    buffer.insert(body);

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