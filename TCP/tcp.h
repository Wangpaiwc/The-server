#pragma once


#include <boost/asio.hpp>
#include <string>
#include <functional>
#include "plant.h"

using boost::asio::ip::tcp;

class TCPConnection {
public:
    // �ص��������Ͷ���
    using ReceiveCallback = std::function<void(const std::string&)>;
    using ErrorCallback = std::function<void(const boost::system::error_code&)>;

    // ���캯�� (���ڿͻ���)
    TCPConnection(boost::asio::io_context& io_context);

    // ���캯�� (���ڷ�����)
    TCPConnection(boost::asio::io_context& io_context, tcp::socket socket);

    // ����Զ������
    void connect(const std::string& host, const std::string& port);

    // ��������
    void send(const std::string& message);

    // ��ʼ�첽��������
    void startReceiving();

    // ���ý��ջص�
    void setReceiveCallback(ReceiveCallback callback);

    // ���ô���ص�
    void setErrorCallback(ErrorCallback callback);

    // �ر�����
    void close();

    // ��������Ƿ��Ծ
    bool isConnected() const;

    My_Moo::plant p;

    boost::asio::io_context& get_io_context()
    {
        return io_context_;
    }
private:
    // �첽��ȡ����
    void doRead();

    // �첽д�����
    void doWrite();

    // �����ȡ���
    void handleRead(const boost::system::error_code& error, size_t bytes_transferred);

    // ����д�����
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

