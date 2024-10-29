/*************************************************************************
    > File Name: MutexLock.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月22日 星期五 17时27分17秒
 ************************************************************************/
#ifndef __MUTEXLOCK_HH
#define __MUTEXLOCK_HH

#include "NoCopyable.hh"
#include <pthread.h>

/**
 * @brief 对象语义，禁止拷贝和赋值
 * 
 */
class MutexLock
:public NoCopyable
{
public:
    MutexLock();
    ~MutexLock();
    void lock();
    void unlock();
    pthread_mutex_t *getMutexPtr();
private:
    // MutexLock(const MutexLock &) = delete;
    // MutexLock & operator=(const MutexLock &) = delete;
    pthread_mutex_t _mutex;
};

#endif