#include <iostream>
#include <memory>
#include <iterator>
#include <utility>
#include "test.h"

int main(int argc, char* argv[])
{
    auto what = fs::current_path().root_path().wstring();
    try
    {
        auto const address = net::ip::make_address("127.0.0.1");
        unsigned short port = static_cast<unsigned short>(std::atoi("5555"));

        net::io_context ioc{ 1 };
        tcp::acceptor acceptor{ ioc, {address, port} };
        tcp::socket socket{ ioc };
        http_server(acceptor, socket);
        ioc.run();
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}
