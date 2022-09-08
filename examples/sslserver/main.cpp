#include <iostream>

#include <orchid/http.hpp>

using namespace orchid;

http::Server server;

http::Response onExampleRequest(Socket&, http::Request&&)
{
    return http::Response();
}

int main(int argc, char** argv)
{   
    if (argc != 3)
    {
        std::cerr << "usage: orchid [certificate chain file] [private key file]" << std::endl;
        std::exit(1);
    }

    try 
    {
        server.setSSLCertificateChain(argv[1]);
        server.setSSLPrivateKey(argv[2]);
    }
    catch (...)
    {
        std::cerr << "invalid certificate/key file" << std::endl;
        std::exit(1);
    }

    server.bindEndpoint("/ok", onExampleRequest);
    server.run();
}