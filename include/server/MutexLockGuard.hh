/*************************************************************************
    > File Name: include/MutexLockGuard.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月23日 星期六 10时28分57秒
 ************************************************************************/
#ifndef __MUTEXLOCKGUARD_HH
#define __MUTEXLOCKGUARD_HH

#include "MutexLock.hh"

/**
 * @brief RAII思想，使用局部变量托管互斥锁，
 * 使得局部变量销毁时一定解锁互斥锁
 */
class MutexLockGuard
: NoCopyable
{
public:
    MutexLockGuard(MutexLock &mutex)
    : _mutex(mutex) 
    { _mutex.lock(); }

    ~MutexLockGuard()
    { _mutex.unlock(); }
private:
    MutexLock &_mutex;
};

#endif
