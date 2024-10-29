/*************************************************************************
    > File Name: include/ThreadPool.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月23日 星期六 10时47分48秒
 ************************************************************************/
#ifndef __THREADPOOL_HH
#define __THREADPOOL_HH

#include "Thread.hh"
#include <vector>
#include <memory>

using std::vector;
using std::unique_ptr;

using Task = function<void()>;
class ThreadPool
{
public:
    ThreadPool(size_t threadNum, size_t _queSize);
    ~ThreadPool();

    void start();
    void stop();

    void addTask(Task &task);
    void addTask(Task &&task);
    Task getTask();

    void threadFunc();
private:
    size_t _threadNum;
    size_t _queSize;
    vector<unique_ptr<Thread>> _threads;
    TaskQueue _taskQue;
    bool _isExit;
};


#endif