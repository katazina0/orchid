#include <iostream>

#include <orchid/http.hpp>

using namespace orchid;

http::Client client;

int main()
{
    http::Response response = client.GET
    (
        http::URL("https://www.google.com/"),
        http::Headers
        {
            { "user-agent", "something" }
        }
    );
    std::cout << response.getBody() << "\n";
}