/*************************************************************************
    > File Name: RoundRobin.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年08月05日 星期五 11时53分53秒
 ************************************************************************/
#ifndef __ROUNDROBIN_HH
#define __ROUNDROBIN_HH

#include "TcpConnection.hh"
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::unordered_map;
using std::unordered_set;
using std::vector;

/**
 * @brief 超时轮询模块，实现大规模数据的集体超时
 * 
 */

class RoundRobin
{
    using elemType = int;
public: 
    RoundRobin(int timeout = 30);
    //自动更新连接时间，返回超时数据的集合
    unordered_set<elemType> autoUpdate();
    //插入新连接或更新旧连接
    void updateOneConnection(const elemType fd);
    //移出一个连接
    void removeOneConnection(const elemType fd);
private:
    //用vector模拟循环数组，因此专门设置了一个函数来实现
    size_t nextIndex();
private:
    size_t _curIndex;       //当前指针位置
    size_t _cycleSize;      //数组大小
    vector<unordered_set<elemType>> _timeCycle;
};
#endif

