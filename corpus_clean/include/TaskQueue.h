#pragma once
#include <iostream>
#include <queue>

// 定义任务结构体
using callback = void (*)(void *, void *);
struct Task
{
    Task()
    {
        function = nullptr;
        arg = nullptr;
        arg_exten = nullptr;
    }
    /*
    Task(callback f, void *arg)
    {
        function = f;
        this->arg = arg;
    }*/
    // 传入2组参数，arg表示每个线程独有的变量，arg_exten用于存储线程之间公用的变量
    Task(callback f, void *arg, void *arg_exten)
    {
        function = f;
        this->arg = arg;
        this->arg_exten = arg_exten;
    }
    callback function;
    void *arg;
    void *arg_exten;
};

// 任务队列
class TaskQueue
{
public:
    TaskQueue();
    ~TaskQueue();

    // 添加任务
    void addTask(Task &task);
    // void addTask(callback func, void *arg);
    void addTask(callback func, void *arg, void *arg_exten);

    // 取出一个任务
    Task takeTask();

    // 获取当前队列中任务个数
    inline int taskNumber()
    {
        return m_queue.size();
    }

private:
    pthread_mutex_t m_mutex;  // 互斥锁
    std::queue<Task> m_queue; // 任务队列
};
