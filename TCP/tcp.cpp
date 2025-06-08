#include "tcp.h"
#include <iostream>

TCPConnection::TCPConnection(boost::asio::io_context& io_context)
    : io_context_(io_context), socket_(io_context) {
}

TCPConnection::TCPConnection(boost::asio::io_context& io_context, tcp::socket socket)
    : io_context_(io_context), socket_(std::move(socket)) {
    connected_ = true;
    startReceiving();
}

void TCPConnection::connect(const std::string& host, const std::string& port) {
    tcp::resolver resolver(io_context_);
    boost::asio::async_connect(socket_, resolver.resolve(host, port),
        [this](const boost::system::error_code& error, const tcp::endpoint&) {
            if (!error) {
                connected_ = true;
                startReceiving();
            }
            else if (error_callback_) {
                error_callback_(error);
            }
        });
}

void TCPConnection::send(const std::string& message) {
    bool write_in_progress = !write_buffer_.empty();
    write_buffer_ += message;

    if (!write_in_progress) {
        doWrite();
    }
}

void TCPConnection::startReceiving() {
    doRead();
}

void TCPConnection::setReceiveCallback(ReceiveCallback callback) {
    receive_callback_ = std::move(callback);
}

void TCPConnection::setErrorCallback(ErrorCallback callback) {
    error_callback_ = std::move(callback);
}

void TCPConnection::close() {
    if (socket_.is_open()) {
        boost::system::error_code ec;
        socket_.shutdown(tcp::socket::shutdown_both, ec);
        socket_.close(ec);
        connected_ = false;
    }
}

bool TCPConnection::isConnected() const {
    return connected_;
}

void TCPConnection::doRead() {
    boost::asio::async_read_until(socket_, read_buffer_, '\n',
        [this](const boost::system::error_code& error, size_t bytes_transferred) {
            handleRead(error, bytes_transferred);
        });
}

void TCPConnection::doWrite() {
    boost::asio::async_write(socket_, boost::asio::buffer(write_buffer_),
        [this](const boost::system::error_code& error, size_t bytes_transferred) {
            handleWrite(error, bytes_transferred);
        });
}

void TCPConnection::handleRead(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
        std::string message(
            boost::asio::buffers_begin(read_buffer_.data()),
            boost::asio::buffers_begin(read_buffer_.data()) + bytes_transferred);
        read_buffer_.consume(bytes_transferred);

        if (receive_callback_) {
            receive_callback_(message);
        }

        doRead();
    }
    else {
        connected_ = false;
        if (error_callback_) {
            error_callback_(error);
        }
    }
}

void TCPConnection::handleWrite(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
        write_buffer_.erase(0, bytes_transferred);

        if (!write_buffer_.empty()) {
            doWrite();
        }
    }
    else {
        connected_ = false;
        if (error_callback_) {
            error_callback_(error);
        }
    }
}

void TCPServer::startAccept()
{
    acceptor_.async_accept(
        [this](boost::system::error_code ec, tcp::socket socket)
        {
            if (!ec) {
                // 创建新连接
                auto conn = std::make_shared<TCPConnection>(
                    io_context_, std::move(socket));

                // 设置回调
                conn->setReceiveCallback([](const std::string& data) {
                    std::cout << "Received: " << data << std::endl;
                    });

                conn->setErrorCallback([](const boost::system::error_code& ec) {
                    std::cerr << "Error: " << ec.message() << std::endl;
                    });

                // 开始接收数据
                conn->startReceiving();

                // 存储连接
                connections_.push_back(conn);
            }

            // 继续接受新连接
            startAccept();
        });
}
