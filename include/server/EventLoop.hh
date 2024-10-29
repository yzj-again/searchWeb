/*************************************************************************
    > File Name: EventLoop.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月26日 星期二 21时34分15秒
 ************************************************************************/
#ifndef __EVENTLOOP_HH
#define __EVENTLOOP_HH

#include "TcpConnection.hh"
#include "MutexLockGuard.hh"
#include "RoundRobin.hh"

#include <memory>
#include <vector>
#include <unordered_map>
#include <functional>

using std::vector;
using std::unordered_map;
using std::shared_ptr;
using std::function;

class Acceptor;

class EventLoop
{
    using TcpConnectionPtr = shared_ptr<TcpConnection>;
    using TcpConnectionCallback = function<void(const TcpConnectionPtr &)>;
    using Functor = function<void()>;
public:
    EventLoop(Acceptor & acceptor);
    ~EventLoop();

    void loop();
    void unloop();

    void setConnectionCallback(TcpConnectionCallback &&);
    void setMessageCallback(TcpConnectionCallback &&);
    void setCloseCallback(TcpConnectionCallback &&);

    void wakeup();
    void handleRead();
    void handleTimer();
    void runInLoop(Functor && cb);
    void doPendingFunctors();
private:
    void waitEpollFd();
    int createEpollFd();
    int createEventFd();
    int createTimerFd();
    void setTimerFd(int initsec, int perisrc);
    void handleNewConnection();
    void handleMsg(int fd);
    void addEpollReadFd(int fd);
    void delEpollReadFd(int fd);
private:
    int _epfd;
    int _evfd;
    int _timerfd;
    bool _isLooping;
    MutexLock _mutex;
    Acceptor & _acceptor;
    RoundRobin _roundRobin;
    
    vector<Functor> _pendingsCb;
    vector<struct epoll_event> _evtList;
    unordered_map<int, TcpConnectionPtr> _connMap;

    TcpConnectionCallback _onConnectionCb;
    TcpConnectionCallback _onMessageCb;
    TcpConnectionCallback _onCloseCb;
};

#endif