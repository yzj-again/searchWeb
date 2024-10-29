/*************************************************************************
    > File Name: Thread.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月22日 星期五 15时24分53秒
 ************************************************************************/
#ifndef __THREAD_HH
#define __THREAD_HH

#include "NoCopyable.hh"
#include "TaskQueue.hh"

#include <pthread.h>
#include <functional>
#include <memory>

using std::function;
using std::bind;

extern __thread int threadIdx;

/**
 * @brief 基于对象的设计
 * 可以避免继承
 */
using ThreadCallBack = function<void()>;
class Thread
: NoCopyable
{
public:
    Thread(ThreadCallBack &&cb, int threadIndex)
    : _cb(std::move(cb))
    , _tid(0)
    , _isRunning(false) 
    , _threadIdx(threadIndex)
    { }

    Thread(ThreadCallBack &cb, int threadIndex)
    : _cb(std::move(cb)) 
    , _tid(0)
    , _isRunning(false) 
    , _threadIdx(threadIndex)
    { }

    ~Thread()
    {
        if(_isRunning) { pthread_detach(_tid); }
    }

    static void * threadFunc(void *pThread);

    void start();
    void join();
private:
    ThreadCallBack _cb;
    pthread_t _tid;
    bool _isRunning;
    int _threadIdx;     //该线程的编号
};

#endif
