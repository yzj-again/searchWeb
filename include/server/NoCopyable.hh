/*************************************************************************
    > File Name: include/NoCopyable.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月23日 星期六 11时07分18秒
 ************************************************************************/
#ifndef __NOCOPYABLE_HH
#define __NOCOPYABLE_HH

/**
 * @brief 当其他类继承这个类时，会被禁止进行拷贝构造和赋值
 * 
 */
class NoCopyable
{
protected:
    NoCopyable() = default;
    ~NoCopyable() = default;
    NoCopyable(const NoCopyable &) = delete;
    NoCopyable & operator=(const NoCopyable &) = delete;
};

#endif

