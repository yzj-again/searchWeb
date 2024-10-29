/*************************************************************************
    > File Name: Condition.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月22日 星期五 17时33分49秒
 ************************************************************************/

#include "../../include/server/Condition.hh"
#include "../../include/server/MutexLock.hh"
#include <iostream>

Condition::Condition(MutexLock & mutex)
: _mutex(mutex)
{ 
    int ret = pthread_cond_init(&_cond, nullptr);
    if(ret) { perror("pthread_cond_init"); }
}

void Condition::wait()
{
    int ret = pthread_cond_wait(&_cond, _mutex.getMutexPtr());
    if(ret) { perror("pthread_cond_wait"); }
}

void Condition::notify()
{
    int ret = pthread_cond_signal(&_cond);
    if(ret) { perror("pthread_cond_signal"); }
}

void Condition::notifyAll()
{
    int ret = pthread_cond_broadcast(&_cond);
    if(ret) { perror("pthread_cond_broadcast"); }
}

Condition::~Condition()
{ 
    int ret = pthread_cond_destroy(&_cond);
    if(ret) { perror("pthread_cond_destroy"); }
}