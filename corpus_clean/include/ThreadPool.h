#pragma once
#include <iostream>
#include <thread>
#include "TaskQueue.h"

class ThreadPool
{
public:
    ThreadPool(int min, int max);
    ~ThreadPool();

    int m_doneNum;
    // 添加任务
    void addTask(Task task);

private:
    // 工作的线程的任务函数
    static void *worker(void *arg);
    // 管理者线程的任务函数
    static void *manager(void *arg);

    // 关掉线程池
    void threadExit();

private:
    pthread_mutex_t m_lock;
    pthread_cond_t m_notEmpty;
    pthread_t *m_threadIDs;
    pthread_t m_managerID;
    TaskQueue *m_taskQ;
    int m_minNum;
    int m_maxNum;
    int m_exitNum;
    int m_busyNum;
    int m_aliveNum;
    bool m_shutdown = false;
    static const int NUMBER = 2;
};
