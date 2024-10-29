/*************************************************************************
    > File Name: SocketIO.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月26日 星期二 14时38分40秒
 ************************************************************************/

#ifndef __SOCKETIO_HH
#define __SOCKETIO_HH

/**
 * @brief 对一个TCP连接进行读写操作
 * 
 */
class SocketIO
{
public:
    explicit SocketIO(int fd)
    : _fd(fd) { }

    ~SocketIO() { };

    int readn(char *buf, int len);
    int readLine(char *buf, int len);
    int readJson(char *buf, int len);
    int writen(const char *buf, int len);
private:
    int _fd;
};

#endif