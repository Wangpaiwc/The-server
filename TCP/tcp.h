#pragma once


#include <boost/asio.hpp>
#include <string>
#include <functional>
#include "plant.h"

using boost::asio::ip::tcp;

class TCPConnection {
public:
    // 回调函数类型定义
    using ReceiveCallback = std::function<void(const std::string&)>;
    using ErrorCallback = std::function<void(const boost::system::error_code&)>;

    // 构造函数 (用于客户端)
    TCPConnection(boost::asio::io_context& io_context);

    // 构造函数 (用于服务器)
    TCPConnection(boost::asio::io_context& io_context, tcp::socket socket);

    // 连接远程主机
    void connect(const std::string& host, const std::string& port);

    // 发送数据
    void send(const std::string& message);

    // 开始异步接收数据
    void startReceiving();

    // 设置接收回调
    void setReceiveCallback(ReceiveCallback callback);

    // 设置错误回调
    void setErrorCallback(ErrorCallback callback);

    // 关闭连接
    void close();

    // 检查连接是否活跃
    bool isConnected() const;

    My_Moo::plant p;

    boost::asio::io_context& get_io_context()
    {
        return io_context_;
    }
private:
    // 异步读取操作
    void doRead();

    // 异步写入操作
    void doWrite();

    // 处理读取完成
    void handleRead(const boost::system::error_code& error, size_t bytes_transferred);

    // 处理写入完成
    void handleWrite(const boost::system::error_code& error, size_t bytes_transferred);

    boost::asio::io_context& io_context_;
    tcp::socket socket_;
    boost::asio::streambuf read_buffer_;
    std::string write_buffer_;

    ReceiveCallback receive_callback_;
    ErrorCallback error_callback_;

    bool connected_ = false;

    
};

class TCPServer {
public:
    TCPServer(boost::asio::io_context& io_context, short port)
        : io_context_(io_context),
        acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        startAccept();
    }

private:
    void startAccept();
    
    boost::asio::io_context& io_context_;
    tcp::acceptor acceptor_;
    std::vector<std::shared_ptr<TCPConnection>> connections_;
};

