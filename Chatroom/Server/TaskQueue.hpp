#ifndef TASK_QUEUE_H
#define TASK_QUEUE_H

#include <pthread.h>
#include <queue>
// 定义任务结构体
using callback = void(*)(void*);
template<typename T>
struct Task{
    Task<T>(){
        function = nullptr;
        arg = nullptr;
    }
    Task<T>(callback f, void* arg){
        function = f;
        this->arg = static_cast<T*>(arg);
    }
    callback function;
    T* arg;
};

// 任务队列
template<typename T>
class TaskQueue{
public:
    TaskQueue();
    ~TaskQueue();

    // 添加任务
    void addTask(callback func, void* arg);
    void addTask(Task<T>& task);


    // 取出一个任务
    Task<T> takeTask();

    // 获取当前队列中任务个数
    inline int taskNumber(){
        return m_queue.size();
    }

private:
    pthread_mutex_t m_mutex;    // 互斥锁
    std::queue<Task<T>> m_queue;   // 任务队列
};

#endif