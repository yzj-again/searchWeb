/*************************************************************************
    > File Name: TaskQueue.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月22日 星期五 17时53分41秒
 ************************************************************************/
#include "../../include/server/TaskQueue.hh"
#include "../../include/server/MutexLockGuard.hh"

TaskQueue::TaskQueue(size_t sz)
: _queSize(sz)
, _que()
, _mutex()
, _notEmpty(_mutex)
, _notFull(_mutex)
, _isExitTask(false)
{

}

TaskQueue::~TaskQueue()
{ }

bool TaskQueue::empty() const
{ return 0 == _que.size(); }

bool TaskQueue::full() const
{ return _que.size() == _queSize; }

void TaskQueue::push(const ElemType &value)
{
    //先需要进行上锁（RAII思想）
    MutexLockGuard lockGuard(_mutex);

    //等待在满的线程，需要进行wait
    while(full())
    { _notFull.wait(); }

    //只有queue不为满的时候，才可以去进行push数据
    _que.push(value);

    //此时可以唤醒等待在条件变量为空的线程
    _notEmpty.notify();
}

void TaskQueue::push(ElemType &&value)
{
    //先需要进行上锁（RAII思想）
    MutexLockGuard lockGuard(_mutex);

    //等待在满的线程，需要进行wait
    while(full())
    { _notFull.wait(); }

    //只有queue不为满的时候，才可以去进行push数据
    _que.push(value);

    //此时可以唤醒等待在条件变量为空的线程
    _notEmpty.notify();
}

ElemType TaskQueue::pop()
{
    MutexLockGuard lockGuard(_mutex);
    while(!_isExitTask && empty())
    { _notEmpty.wait(); }

    if(_isExitTask) { return 0; }

    ElemType ret = _que.front();
    _que.pop();
    _notFull.notify();
    return ret;
}

/**
 * @brief 在使用wakeup之前，应该保证任务队列中没有任务
 * 
 */
void TaskQueue::wakeup()
{
    _isExitTask = true;
    _notEmpty.notifyAll();
}
