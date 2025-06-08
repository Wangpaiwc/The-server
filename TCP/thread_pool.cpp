#include "thread_pool.h"

ThreadPool::ThreadPool(size_t threads) :
    work_(boost::asio::make_work_guard(io_ctx_)) {
    for (size_t i = 0; i < threads; ++i) {
        threads_.create_thread([this]() { io_ctx_.run(); });
    }
}

ThreadPool::~ThreadPool()
{
    
    work_.reset();
    threads_.join_all();
    
}

