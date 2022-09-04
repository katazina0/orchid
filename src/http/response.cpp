#include <orchid/http/response.hpp>

orchid::Buffer orchid::http::Response::serialize()
{
    headers["content-length"] = std::to_string(body.size());

    orchid::Buffer buffer;

    buffer.insert(protocol);
    buffer.push(' ');
    buffer.insert(code);
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
    this->code = std::to_string(static_cast<int>(code));
    this->status = STATUS[code];
}