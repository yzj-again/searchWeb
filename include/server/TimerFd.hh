/*************************************************************************
    > File Name: TimerFd.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月29日 星期五 10时15分49秒
 ************************************************************************/
#ifndef __TIMERFD_HH
#define __TIMERFD_HH

#include <functional>

using std::function;

class TimerFd
{
    using TimerFdCallback = function<void()>; 
public:
    TimerFd(int initSec, int periSec, TimerFdCallback && cb);
    ~TimerFd();
    void start();
    void stop();
private:
    void handleRead();
    void setTimerFd(int initSec, int periSec);
    int createTimerFd();
private:
    int _timerFd;
    int _initSec;
    int _periSec;
    TimerFdCallback _cb;
    bool _isStarted;
};

#endif
