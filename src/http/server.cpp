#include <orchid/http/server.hpp>

void orchid::http::Server::run()
{
    orchid::Socket server;
    server.bind(port);
    server.listen();

    while (true)
    {
        orchid::Socket& client = *new orchid::Socket(server.accept());
        std::jthread([&]
        {
            try
            {
                while (true)
                {
                    if (onRequestReceived != nullptr)
                        onRequestReceived(client, orchid::http::Request(client));
                }
            }
            catch (...) 
            {
                delete &client;
            }
        }).detach();
    }
}

void orchid::http::Server::respond(orchid::Socket& socket, orchid::http::Response&& response)
{
    auto serialized = response.serialize();
    socket.write(serialized);
}

void orchid::http::Server::respondFile(orchid::Socket& socket, const std::string& filename)
{
    auto path = std::filesystem::current_path().append(filename.substr(1));
    FILE* file = fopen(path.c_str(), "rb");
    if (file == nullptr)
    {
        respond(socket, orchid::http::Response(CODE::NOT_FOUND));
    }
    else
    {
        fseek(file, 0, SEEK_END);
        std::size_t size = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        orchid::http::Response response(CODE::OK);
        response.body.resize(size);

        fread(response.body.data(), size, 1, file);
        fclose(file);

        response.headers["content-type"] = MIME[path.extension().string().substr(1)];
        respond(socket, std::forward<Response>(response));
    }
}