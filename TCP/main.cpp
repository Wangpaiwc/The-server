#include <iostream>
#include "tcp.h"
#include "mysql.h"
#include <boost/asio.hpp>

#include <memory>
#include "TCP.h"

int main_2() {
    try {
        boost::asio::io_context io_context;

        // 创建服务器，监听端口12345
        TCPServer server(io_context, 12345);

        std::cout << "Server started. Listening on port 12345..." << std::endl;

        // 运行IO服务
        io_context.run();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}

int main_1()
{
    std::string host = "127.0.0.1";
    int port = 33060;
    std::string user = "root";
    std::string password = "root";
    std::string database = "list";

    MySQLConnector m;
    
    auto s = m.getUserRelationsByUsername("one");

    for (auto &i : s)
    {
        std::cout << i.user << " " << i.contend;
    }
       

    return 0;
}

int main() {
    
    main_2();
    return 0;
}
