//
// Created by phoenixflower on 11/14/20.
//
#include "REST/WebServer.hpp"
namespace LD
{
    void WebServerExample()
    {
        try
        {
            // Check command line arguments.
            auto const address = net::ip::make_address("::");
            unsigned short port = static_cast<unsigned short>(std::atoi("9090"));

            net::io_context ioc{1};

            tcp::acceptor acceptor{ioc, {address, port}};
            tcp::socket socket{ioc};
            http_server(acceptor, socket);

            ioc.run();
        }
        catch(std::exception const& e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
            //return EXIT_FAILURE;
        }
    }
}
