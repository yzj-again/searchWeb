/*************************************************************************
    > File Name: WebPageSearcher.hh
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年08月01日 星期一 17时23分52秒
 ************************************************************************/
#ifndef __WEBPAGESEARCHER_HH
#define __WEBPAGESEARCHER_HH

#include "MyRedis.hh"
#include "WebPageQuery.hh"

/**
 * @brief 协调在redis(缓存)和在pageQuery(磁盘)中查找两种方式
 *        可以写成单例类，本项目中只创建了一个对象
 */

class WebPageSearcher
{
public:
    WebPageSearcher(MyRedis & redis, WebPageQuery & pageQuery);
    string doQuery(const string & sought);
    string returnNoAnswer();
private:
    MyRedis & _redis;           //缓存类对象的引用
    WebPageQuery & _pageQuery;  //用于从磁盘获取文章
};

#endif
