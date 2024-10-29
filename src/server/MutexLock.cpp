/*************************************************************************
    > File Name: MutexLock.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月22日 星期五 17时29分23秒
 ************************************************************************/
#include "../../include/server/MutexLock.hh"
#include <iostream>

MutexLock::MutexLock()
{
    int ret = pthread_mutex_init(&_mutex, nullptr);
    if(ret) { perror("pthread_mutex_init"); }
}

MutexLock::~MutexLock()
{
    int ret = pthread_mutex_destroy(&_mutex);
    if(ret) { perror("pthread_mutex_destroy"); }
}

void MutexLock::lock()
{
    int ret = pthread_mutex_lock(&_mutex);
    if(ret) { perror("pthread_mutex_lock"); }
}

void MutexLock::unlock()
{
    int ret = pthread_mutex_unlock(&_mutex);
    if(ret) { perror("pthread_mutex_unlock"); }
}

pthread_mutex_t *MutexLock::getMutexPtr()
{ return &_mutex; }