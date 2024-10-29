/**************************************************
 
//File Name: DirScanner.hh
//Author: HZ_Alfred
//Created Time: 2022-07-31  星期日  11:42:20
 
**************************************************/
 
#ifndef __DirScanner_H__
#define __DirScanner_H__

#include "Configuration.hh"
#include <string>
#include <vector>

using std::string;
using std::vector;

//目录扫描类——递归扫描
class DirScanner
{
public:
    explicit DirScanner() {}
    //重载()运算符，调用Traverse函数，扫描指定目录下所有文件
    void operator()(const string &dir);
    //返回文件路径容器的引用
    vector<string>& GetFilePath();
    //打印读到的文件及其路径，做测试用
    void printFiles() const;
private:
    vector<string> _files;//存放每个语料文件的绝对路径
};

#endif
