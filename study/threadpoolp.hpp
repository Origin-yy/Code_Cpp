#ifndef TG_THREAD_POOL_H
#define TG_THREAD_POOL_H
 
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
 
class tg_thread_pool
{
public:
    tg_thread_pool(size_t);
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
    ~tg_thread_pool();
private:
    std::vector<std::thread> _threads;
    std::queue<std::function<void()>> _que;
 
    std::mutex _mutex;
    std::condition_variable _condition;
    bool _stop;
};
 
inline tg_thread_pool::tg_thread_pool(size_t num) : _stop(false)
{
    for(size_t i = 0;i<num;++i)
    {
        _threads.emplace_back(
            [this]
            {
                for(;;)
                {
                    std::function<void()> task;
 
                    {
                        std::unique_lock<std::mutex> lock(this->_mutex);
                        this->_condition.wait(lock, [this]{ return this->_stop || !this->_que.empty(); });
                        if(this->_stop && this->_que.empty())
                        {
                            return;
                        }
 
                        task = std::move(this->_que.front());
                        this->_que.pop();
                    }
 
                    task();
                }
            }
        );
    }
}
 
template<class F, class... Args>
auto tg_thread_pool::enqueue(F&& f, Args&&... args)
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;
 
    auto task = std::make_shared< std::packaged_task<return_type()> >(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
 
    std::future<return_type> res = task->get_future();
    {
        std::unique_lock<std::mutex> lock(_mutex);
 
        if(_stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");
 
        _que.emplace([task](){ (*task)(); });
    }
    _condition.notify_one();
    return res;
}
 
inline tg_thread_pool::~tg_thread_pool()
{
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _stop = true;
    }
    _condition.notify_all();
    for(std::thread &worker: _threads)
        worker.join();
}
 
#endif // TG_THREAD_POOL_H