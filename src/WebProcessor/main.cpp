#include "../../include/WebProcessor/WebPage.hh"
#include "../../include/WebProcessor/PageLibPreprocessor.hh"
#include "../../include/WebProcessor/DirScanner.hh"
#include "../../include/WebProcessor/PageLib.hh"
using std::cerr;

void test0()
{
    Configuration* config = Configuration::getInstance();
    DirScanner dir;
    PageLib p(config,dir);
    //p.create();
    p.storeArticle();
    p.storeOffsetLib();
}

void test1()
{
    Configuration* config = Configuration::getInstance();
    PageLibPreprocessor pageLib(*config);
    pageLib.doProcess();
}

void test2()
{
    string s1 = "<docid>1</docid><title>ETCD的内存问题</title><url>https://coolshell.cn/articles/22242.html</url><content>今天跟大家分享一个etcd的内存大量占用的问题，这是前段时间在我们开源软件Easegress中遇到的问题，问题是比较简</content>";
    string s2 = "<docid>1</docid><title>你的名字叫什么</title><url>https://coolshell.cn/articles/22242.html</url><content></content>";
    //string s2 = "<docid>1</docid><title>你的名字叫什么</title><url>https://coolshell.cn/articles/22242.html</url><content>所以随便写点什么把，就这样。</content>";
    WordSegmentation _jieba;
    WebPage w1(s1, *(Configuration::getInstance()), _jieba);
    WebPage w2(s2, *(Configuration::getInstance()), _jieba);
    
    cout << (w1 == w2) << endl;
}

int main()
{
    test0();
    test1();
    return 0;
}



