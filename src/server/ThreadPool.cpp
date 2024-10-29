/*************************************************************************
    > File Name: src/ThreadPool.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月23日 星期六 10时47分59秒
 ************************************************************************/

#include "../../include/server/ThreadPool.hh"
#include "../../include/server/TaskQueue.hh"
#include <unistd.h>

ThreadPool::ThreadPool(size_t threadNum, size_t queSize)
: _threadNum(threadNum)
, _queSize(queSize)
, _taskQue(_queSize)
, _isExit(false)
{ _threads.reserve(_threadNum); }

ThreadPool::~ThreadPool()
{
    if(!_isExit) 
    { 
        stop(); 
        _isExit = true;
    }
}

void ThreadPool::start()
{
    for(size_t cnt = 0; cnt < _threadNum; ++cnt)
    { 
        unique_ptr<Thread> thrd(new Thread(bind(&ThreadPool::threadFunc, this), cnt));
        _threads.push_back(std::move(thrd));
        _threads.back()->start();
    }
}

void ThreadPool::stop()
{
    while(!_taskQue.empty())
    { sleep(1); }

    _isExit = true;
    _taskQue.wakeup();

    for(auto &thrd: _threads)
    { thrd->join(); }
}

void ThreadPool::addTask(Task &task)
{ 
    if(task)
    { _taskQue.push(std::move(task)); } 
}

void ThreadPool::addTask(Task &&task)
{ 
    if(task)
    { _taskQue.push(std::move(task)); }
}

Task ThreadPool::getTask()
{ return _taskQue.pop(); }

void ThreadPool::threadFunc()
{
    while(!_isExit)
    {
        Task taskcb = getTask();
        if(taskcb) { taskcb(); }
    }
}