/*************************************************************************
    > File Name: TimerFd.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月29日 星期五 10时20分14秒
 ************************************************************************/
#include "../../include/server/TimerFd.hh"
#include <ctime>
#include <sys/timerfd.h>
#include <unistd.h>

TimerFd::TimerFd(int initSec, int periSec, TimerFdCallback && cb)
: _initSec(initSec)
, _periSec(periSec)
, _cb(std::move(cb))
, _timerFd(createTimerFd())
, _isStarted(false)
{ }

TimerFd::~TimerFd() 
{ 
    if(_isStarted)
    { 
        setTimerFd(0, 0);
        close(_timerFd);
    }
}

void TimerFd::start()
{
    
}

void TimerFd::stop()
{
    _isStarted = false;
    setTimerFd(0, 0);
} 

void TimerFd::handleRead()
{
    uint64_t one = 1;
    int ret = read(_timerFd, &one, sizeof(one));
    if(ret != sizeof(one))
    { perror("read"); }
}

void TimerFd::setTimerFd(int initSec, int periSec)
{
    struct itimerspec value;
    value.it_value.tv_sec = initSec;
    value.it_value.tv_nsec = 0;
    value.it_interval.tv_sec = periSec;
    value.it_interval.tv_nsec = 0;

    int fd = timerfd_settime(_timerFd, 0, &value, nullptr);
    if(fd < 0)
    {
        perror("timerfd_settime");
        return;
    }
}

int TimerFd::createTimerFd()
{
    int fd = timerfd_create(CLOCK_REALTIME, 0);
    if(fd < 0)
    { perror("timerfd_create"); }
    return fd;
}