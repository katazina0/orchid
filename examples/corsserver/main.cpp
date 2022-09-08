#include <orchid/http.hpp>

using namespace orchid;

http::Server server;

http::Response onCORSRequest(Socket&, http::Request&& request)
{
    if (!request.hasForm("url"))
    {
        http::Response response;
        response.setStatus(http::Status::NOT_FOUND);
        return response;
    }

    auto url = http::URL(request.getForm("url"));
    request.addHeader("host", url.hostname);
    request.addHeader("origin", url.hostname);
    request.addHeader("referer", url.hostname);
    request.setEndpoint(request.getForm("url"));

    http::Client client(url.hostname);
    auto response = client.request(std::forward<http::Request>(request));
    
    if (response.hasHeader("transfer-encoding"))
        response.removeHeader("transfer-encoding");
        
    response.addHeader("access-control-allow-origin", "*");
    response.addHeader("content-length", response.getBody().size());
    return response;
}

int main()
{
    server.bindEndpoint("/cors", onCORSRequest);
    server.run();
}