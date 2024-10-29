/*************************************************************************
    > File Name: main.cpp
    > Author: masker
    > Github: https://github.com/MuYe-Code
    > Created Time: 2022年07月26日 星期二 20时24分19秒
 ************************************************************************/

#include "../../include/DictBuilder/Configuration.hh"
#include "../../include/DictBuilder/DictProducer.hh"
#include "../../include/DictBuilder/CppJiebaEn.hh"
#include "../../include/DictBuilder/CppJiebaCn.hh"

#include <fstream>

using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::cout;
using std::endl;

//测试模块一：关键词中文
void test1(){    
    Configuration * conf = Configuration::getInstance();
    conf->showConfig();
    SplitTool *splitTool = new CppJieba();
    
    DictProducer dictCn(*conf, splitTool);

    DictProducer dictEn(*conf);
}

//融合中英文
void test2(){
    Configuration * conf = Configuration::getInstance();
    conf->showConfig();
    SplitTool *splitToolCn = new CppJieba();
    SplitTool *splitToolEn = new CppJiebaEn();
    
    //tag = 0 : 英文   tag = 1 : 中文
    DictProducer dictEn(*conf, splitToolEn, 0);
    
    DictProducer dictCn(*conf, splitToolCn, 1);
}

int main()
{
    //test1();
    test2();
    return 0;
}
