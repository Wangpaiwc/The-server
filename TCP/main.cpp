#include <iostream>
#include "tcp.h"
#include "mysql.h"
#include <boost/asio.hpp>
#include <memory>


int main_2() {
    try {
        boost::asio::io_context io_context;

        TCPServer server(io_context, 12345);

        std::cout << "Server started. Listening on port 12345..." << std::endl;

        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}


int main() {
    
    main_2();

    return 0;
}
