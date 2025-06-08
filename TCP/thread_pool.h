#pragma once
#include "mysql.h"
#include <boost/asio/io_context.hpp>
#include <boost/thread.hpp>
#include <boost/asio/post.hpp>

class ThreadPool {
public:
    static ThreadPool& instance()
    {
        ConfigManager& c = ConfigManager::instance();

        static ThreadPool th(c.thread_number);

        return th;
    }

    template<typename F>
    void submit(F task) {
        boost::asio::post(io_ctx_, task);
    }

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;

private:
    explicit ThreadPool(size_t threads);

    ~ThreadPool();

    boost::asio::io_context io_ctx_;
    boost::asio::executor_work_guard<
        boost::asio::io_context::executor_type> work_;
    boost::thread_group threads_;
};
