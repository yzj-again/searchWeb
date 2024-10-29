/*************************************************************************
    > File Name: Condition.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月22日 星期五 17时33分44秒
 ************************************************************************/
#ifndef __CONDITION_HH
#define __CONDITION_HH

#include "NoCopyable.hh"
#include <pthread.h>

class MutexLock;

class Condition
: public NoCopyable
{
public:
    Condition(MutexLock &);
    void wait();
    void notify();
    void notifyAll();
    ~Condition();
private:
    pthread_cond_t _cond;
    MutexLock & _mutex;
};

#endif