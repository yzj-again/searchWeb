/*************************************************************************
    > File Name: RoundRobin.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年08月05日 星期五 12时03分02秒
 ************************************************************************/

#include "../../include/server/RoundRobin.hh"
#include <iostream>

RoundRobin::RoundRobin(int timeout)
: _curIndex(0)
, _cycleSize(timeout)
, _timeCycle(_cycleSize)
{
    if(_cycleSize < 1) 
    { 
        std::cerr << "无效的超时时间" << std::endl;
        _cycleSize = 30;
    }
    std::cout << "超时轮询模块初始化成功" << std::endl;
}

/**
 * @brief 先将指针移到下一个位置，移到位置的连接就是应该超时的连接
 * 
 * @return unordered_set<RoundRobin::elemType> 超时连接的集合
 */
unordered_set<RoundRobin::elemType> RoundRobin::autoUpdate()
{
    _curIndex = nextIndex();
    //std::cout << "Auto update timeCycle, curIndex = " << _curIndex  << " size = " << _timeCycle[_curIndex].size() << std::endl;
    //unordered_set<elemType> ret(_timeCycle[_curIndex]);
    //使用std::move减少拷贝，此处编译器可能不会优化
    //使用std::move之后原集合_timeCycle[_curIndex]为空，不清楚是不是所有编译器都是一样的效果
    return std::move(_timeCycle[_curIndex]);
}

/**
 * @brief 旧连接若存在，则先删除旧连接。因此对更新旧连接和插入新连接都适用，当然分开比较高效
 *        将数据更新到当前指针指向的集合，下一次触发autoUpdata函数后指针指向当前集合就是一轮循环之后了
 * @param fd 需要更新或插入的数据
 */
void RoundRobin::updateOneConnection(const elemType fd)
{
    for(auto & elem : _timeCycle)
    {
        if(elem.find(fd) != elem.end())
        {
            elem.erase(fd);
            break;
        }
    }
    _timeCycle[_curIndex].insert(fd);
}

void RoundRobin::removeOneConnection(const elemType fd)
{
    for(auto & elem : _timeCycle)
    {
        if(elem.find(fd) != elem.end())
        { 
            elem.erase(fd); 
            return;
        }
    }
}

/**
 * @brief 实现最后一个下标的下一个是第一个下标，以此模拟循环数组
 * 
 * @return size_t 新的下标
 */
size_t RoundRobin::nextIndex()
{
    return (_curIndex + 1) % _cycleSize;
}