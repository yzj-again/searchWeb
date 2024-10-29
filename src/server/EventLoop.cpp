/*************************************************************************
    > File Name: EventLoop.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月26日 星期二 21时34分31秒
 ************************************************************************/
#include "../../include/server/EventLoop.hh"
#include "../../include/server/Acceptor.hh"

#include <ctime>
#include <sys/timerfd.h>
#include <sys/epoll.h>
#include <sys/eventfd.h>
#include <iostream>

using std::cout;
using std::endl;

EventLoop::EventLoop(Acceptor & acceptor)
: _isLooping(false)
, _acceptor(acceptor)
, _evtList(1024)
, _epfd(createEpollFd())
, _evfd(createEventFd())
, _timerfd(createTimerFd())
, _connMap()
, _roundRobin(30)
{  
    //将_listenSock加入监听红黑树
    addEpollReadFd(_acceptor.fd());
    addEpollReadFd(_evfd);   
}

EventLoop::~EventLoop()
{
    if(_epfd >= 0) { close(_epfd); }
    if(_evfd >= 0) { close(_evfd); }
    if(_timerfd >= 0)
    {
        //设置停止定时器
        setTimerFd(0, 0);
        close(_timerfd);
    }
}

void EventLoop::loop()
{
    _isLooping = true;
    cout << "初始化完成!" << endl;
    setTimerFd(1, 1);//从初始化开始，每隔一秒通知一次； 
    addEpollReadFd(_timerfd);
    while(_isLooping)
    { waitEpollFd(); }
}

void EventLoop::unloop()
{
    _isLooping = false;
}

void EventLoop::runInLoop(Functor && cb)
{
    {
        MutexLockGuard autolock(_mutex);
        _pendingsCb.push_back(std::move(cb));
    }
    wakeup();
}

int EventLoop::createEventFd()
{
    int fd = eventfd(0, 0);
    if(fd < 0) { perror("eventfd"); }
    return fd;
}

void EventLoop::setTimerFd(int initsec, int perisec)
{
    struct itimerspec value;
    value.it_value.tv_sec = initsec;
    value.it_value.tv_nsec = 0;

    value.it_interval.tv_sec = perisec;
    value.it_interval.tv_nsec = 0;

    int fd = timerfd_settime(_timerfd, 0, &value, nullptr);
    if(fd < 0)
    {
        perror("timerfd_settime");
        return ;
    }
}

int EventLoop::createTimerFd()
{
    int fd = timerfd_create(CLOCK_REALTIME, 0);
    if(fd < 0)
    {
        perror("TimerFd");
        return -1;
    }
    return fd;
}

void EventLoop::handleNewConnection()
{
    int peerfd = _acceptor.accept();
    if(peerfd < 0) { perror("handleNewConnection"); }
    addEpollReadFd(peerfd);
    //创建TCP连接
    TcpConnectionPtr con(new TcpConnection(peerfd, this));
    //注册事件
    con->setConnectionCallback(_onConnectionCb);
    con->setMessageCallback(_onMessageCb);
    con->setCloseCallback(_onCloseCb);

    _roundRobin.updateOneConnection(peerfd);

    _connMap[peerfd] = con;
    //_connMap.insert(std::pair<int, TcpConnectionPtr>(peerfd, con));
    //执行对应回调函数
    con->handleConnectionCallback();
}

void EventLoop::handleMsg(int fd)
{
    auto it = _connMap.find(fd);
    if(_connMap.end() != it)  
    {
        if(it->second->isClosed())
        {
            it->second->handleCloseCallback();
            delEpollReadFd(fd);
            _roundRobin.removeOneConnection(fd);
            _connMap.erase(it);
        }

        else 
        { 
            _roundRobin.updateOneConnection(fd);
            it->second->handleMessageCallback(); 
        }
    }

    else { cout << "该连接不存在" << endl; }
}

void EventLoop::handleRead()
{
    uint64_t one;
    int ret = read(_evfd, &one, sizeof(one));
    if(ret != sizeof(one))
    {
        perror("read");
        return;
    }
}

void EventLoop::handleTimer()
{
    uint64_t one = 1;
    int ret = read(_timerfd, &one, sizeof(one));
    if(ret != sizeof(one))
    {
        perror("read");
        return;
    }
    unordered_set<int> delConn =  _roundRobin.autoUpdate();
    for(auto & fd: delConn)
    {
        delEpollReadFd(fd);
        cout << _connMap[fd]->toString() << " time out" << endl;
        _connMap.erase(fd);
    }
}

void EventLoop::wakeup()
{
    uint64_t one;
    int ret = write(_evfd, &one, sizeof(one));
    if(ret != sizeof(one))
    {
        perror("write");
        return;
    }
}

void EventLoop::doPendingFunctors()
{
    vector<Functor> tmp;
    {
        MutexLockGuard autoLock(_mutex);
        tmp.swap(_pendingsCb);
    }
    for(auto &cb : tmp) { cb(); }
}

void EventLoop::setConnectionCallback(TcpConnectionCallback && cb)
{ _onConnectionCb = std::move(cb); }

void EventLoop::setMessageCallback(TcpConnectionCallback && cb)
{ _onMessageCb = std::move(cb); }

void EventLoop::setCloseCallback(TcpConnectionCallback && cb)
{ _onCloseCb = std::move(cb); }

void EventLoop::waitEpollFd()
{
    int nready = -1;
    do
    {
        nready = epoll_wait(_epfd, &(*_evtList.begin())
            , _evtList.size(), 3000);
    }while(-1 == nready && EINTR == errno);

    if(-1 == nready) { perror("epoll_wait"); }
    else if(0 == nready) { cout << ">>epoll_wait_timeout" << endl; }
    else
    {
        if((int)_evtList.size() == nready)
        { _evtList.resize(2 * nready); }

        for(int idx = 0; idx < nready; ++idx)
        {
            int fd = _evtList[idx].data.fd;
            //新连接
            if(_acceptor.fd() == fd)
            {
                //读事件
                if(_evtList[idx].events & EPOLLIN)
                { handleNewConnection(); }
            }
            else if(fd == _evfd)
            { 
                if(_evtList[idx].events & EPOLLIN)
                {
                    handleRead(); 
                    doPendingFunctors();
                }
            }
            else if(fd == _timerfd)
            { 
                if(_evtList[idx].events & EPOLLIN)
                {
                    handleTimer();
                }
            }
            //老连接
            else
            {
                //读事件
                if(_evtList[idx].events & EPOLLIN)
                { handleMsg(fd); }
            }
        }
    }
}

int EventLoop::createEpollFd()
{
    int fd = epoll_create1(0);
    if(0 > fd) { perror("epoll_create1"); }
    return fd;
}

void EventLoop::addEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;
    int ret = epoll_ctl(_epfd, EPOLL_CTL_ADD, fd, &evt);
    if(ret < 0) { perror("epoll_ctl"); }
}

void EventLoop::delEpollReadFd(int fd)
{
    struct epoll_event evt;
    evt.data.fd = fd;
    evt.events = EPOLLIN;
    int ret = epoll_ctl(_epfd, EPOLL_CTL_DEL, fd, &evt);
    if(ret < 0) { perror("epoll_ctl"); }
}

