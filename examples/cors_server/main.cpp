#include <iostream>

#include <orchid/http/server.hpp>
#include <orchid/http/client.hpp>

using namespace orchid;

http::Server server;

http::Response onCORSRequest(Socket&, http::Request&& request)
{
    if (!request.formData.contains("url"))
    {
        return http::Response(http::CODE::NOT_FOUND);
    }

    auto url = http::URL(request.formData["url"]);
    request.headers["host"] = url.hostname;
    request.headers["origin"] = url.hostname;
    request.headers["referer"] = url.hostname;
    request.endpoint = request.formData["url"];

    http::Client client(url.hostname);
    auto response = client.request(std::forward<http::Request>(request));
    
    if (response.headers.contains("transfer-encoding"))
    {
        response.headers.erase("transfer-encoding");
    }

    response.headers["access-control-allow-origin"] = "*";
    response.headers["content-length"] = std::to_string(response.body.size());
    return response;
}

int main()
{
    server.bindEndpoint("/cors", onCORSRequest);
    server.run();
}