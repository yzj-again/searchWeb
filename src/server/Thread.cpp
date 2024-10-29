/*************************************************************************
    > File Name: Thread.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月22日 星期五 15时25分02秒
 ************************************************************************/

#include "../../include/server/Thread.hh"
#include <unistd.h>
#include <iostream>

/***********************基于对象设计*************************************/

__thread int threadIdx = -1;

void* Thread::threadFunc(void *arg)
{
    Thread *pth = static_cast<Thread*>(arg);
    threadIdx = pth->_threadIdx;
    if(pth) { pth->_cb(); }
    pthread_exit(nullptr);
}

void Thread::start()
{
    int ret = pthread_create(&_tid, nullptr, threadFunc, this);
    if(ret)
    {
        perror("pthread_create");
        return;
    }
    _isRunning = true; 
}

void Thread::join()
{
    if(_isRunning) { 
        int ret = pthread_join(_tid, nullptr); 
        if(ret) { perror("pthread_join"); }
        else { _isRunning = false; }
    }
}

/***********************面向对象设计(C++11)*************************************/
