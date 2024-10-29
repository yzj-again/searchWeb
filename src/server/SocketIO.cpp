/*************************************************************************
    > File Name: SocketIO.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月26日 星期二 16时41分30秒
 ************************************************************************/

#include "../../include/server/SocketIO.hh"

#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>

int SocketIO::readn(char *buf, int len)
{
    if(nullptr == buf || len < 0) { return -1; }
    int left = len;
    char *current = buf;
    int ret = 0;

    while(left > 0)
    {
        ret = read(_fd, current, left);
        if(-1 == ret)
        {
            if(EINTR == errno) { continue; }
            perror("read");
            return len - left;
        } 
        else if (0 == ret) { break; }
        else 
        {
            current += ret;
            left -= ret;
        }
    }

    return len - left;
}

int SocketIO::readLine(char *buf, int len)
{
    int left = len - 1; //保留一位给空字符
    char *current = buf;
    int ret = 0, total = 0;

    while(left > 0)
    {
        //MSG_PEEK不会将缓冲区的数据清空，只会进行拷贝操作
        ret = recv(_fd, current, left, MSG_PEEK);
        if(-1 == ret)
        {
            if(EINTR == errno) { continue; }
            perror("recv");
            return len - left;
        } 
        else if (0 == ret) { break; }
        else 
        {
            for(int idx = 0; idx < ret; ++idx)
            {
                if('\n' == current[idx])
                {
                    int sz = idx + 1;
                    readn(current, sz);
                    current += sz;
                    *current = '\0';

                    return total + sz;
                }
            }
            //将数据从内核态拷贝到用户态，会清空相应的缓存区
            readn(current, ret);
            total += ret;
            current += ret;
            left -= ret;
        }
    }

    //读了len个字节依旧没有读到\n的情况
    *current = '\0';
    return total - left;
}

int SocketIO::readJson(char *buf, int len)
{
    int left = len - 1; //保留一位给空字符
    char *current = buf;
    int ret = 0, total = 0;

    while(left > 0)
    {
        //MSG_PEEK不会将缓冲区的数据清空，只会进行拷贝操作
        ret = recv(_fd, current, left, MSG_PEEK);
        if(-1 == ret)
        {
            if(EINTR == errno) { continue; }
            perror("recv");
            return len - left;
        } 
        else if (0 == ret) { break; }
        else 
        {
            for(int idx = 0; idx < ret; ++idx)
            {
                if('\0' == current[idx])
                {
                    int sz = idx + 1;
                    readn(current, sz);
                    current += sz;
                    //*current = '\0';

                    return total + sz;
                }
            }
            //将数据从内核态拷贝到用户态，会清空相应的缓存区
            readn(current, ret);
            total += ret;
            current += ret;
            left -= ret;
        }
    }

    //读了len个字节依旧没有读到\n的情况
    *current = '\0';
    return total - left;
}

int SocketIO::writen(const char *buf, int len)
{
    int left = len;
    const char *current = buf;
    int ret = 0;

    while(left > 0)
    {
        ret = write(_fd, current, left);
        if(-1 == ret)
        {
            if(EINTR == errno) { continue; }
            perror("write");
            return len - left;
        } 
        else if (0 == ret) { break; }
        else 
        {
            current += ret;
            left -= ret;
        }
    }

    return len - left;
}

