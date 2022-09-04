#include <orchid/http/server.hpp>

void orchid::HTTPServer::run()
{
    orchid::Socket socket;
    socket.bind(this->getPort());
    socket.listen();

    while (true)
    {
        int client = socket.accept();
    }
}