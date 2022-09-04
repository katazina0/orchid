#include <orchid/http/server.hpp>

void onRequestReceived()
{

}

int main()
{
    orchid::HTTPServer server;
    server.run();
}