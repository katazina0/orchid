#include <iostream>

#include <orchid/http.hpp>

using namespace orchid;

int main()
{
    http::Response response = http::GET
    (
        http::URL("https://www.google.com/"),
        http::Headers
        {
            { "user-agent", "something" }
        }
    );
    std::cout << response.getBody() << "\n";
}