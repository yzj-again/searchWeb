/*************************************************************************
    > File Name: TaskQueue.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月22日 星期五 17时53分36秒
 ************************************************************************/
#ifndef __TASKQUEUE_HH
#define __TASKQUEUE_HH

#include "NoCopyable.hh"
#include "Condition.hh"
#include "MutexLock.hh"
#include <queue>
#include <functional>

using std::queue;

using ElemType = std::function<void()>;

class TaskQueue
: NoCopyable
{
public:
    TaskQueue(size_t sz);
    ~TaskQueue();
    bool empty() const;
    bool full() const;
    void push(const ElemType &value);
    void push(ElemType &&value);
    ElemType pop();
    void wakeup(); //wakeup before exit

private:
    size_t _queSize;
    queue<ElemType> _que;
    MutexLock _mutex;
    Condition _notEmpty;
    Condition _notFull;
    bool _isExitTask;
};

#endif
