#include "../../include/DictBuilder/DictProducer.hh"
#include "../../include/DictBuilder/SplitTool.hh"
#include <ctype.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <iostream>

using std::ifstream;
using std::ofstream;
using std::istringstream;
using std::cout;
using std::endl;
using std::cerr;

DictProducer::DictProducer(Configuration & conf, SplitTool *splitTool, int tag)
: _conf(conf)
, _splitTool(splitTool)
, _tag(tag)
{
    //1、加载配置文件指示的语料库目录下所有文件
    map<string, string> &confMap = _conf.getConfigMap();

    if(0 == tag){   //英文
        string dir = confMap["corpus-en"];
        getFiles(dir, _files);

        //2、创建词典
        cout << "创建英文词典..." << endl;
        buildDict();

        //3、保存词典文件
        string filepath = confMap["dict-en"];
        cout << "保存英文词典..." << endl;
        storeDict(filepath);

        //4、创建索引
        cout << "创建英文词典索引..." << endl;
        buildIndex(filepath);

        //5、保存索引文件
        cout << "保存英文词典索引..." << endl;
        filepath = confMap["index-en"];
        storeIndex(filepath);

    }else if(1 == tag){ //中文
        string dir = confMap["corpus-cn"];
        getFiles(dir, _files);

        //2、创建词典
        cout << "创建中文词典..." << endl;
        buildDict();

        //3、保存词典文件
        cout << "保存中文词典..." << endl;
        string filepath = confMap["dict-cn"];
        storeDict(filepath);

        //4、创建索引
        cout << "创建中文词典索引..." << endl;
        buildIndex(filepath);

        //5、保存索引文件
        cout << "保存中文词典索引..." << endl;
        filepath = confMap["index-cn"];
        //cout << "filepath in chinese" << filepath << endl;
        storeIndex(filepath);
    }
}


DictProducer::DictProducer(Configuration & conf)
: _conf(conf)
, _splitTool(nullptr)
{
    //1、加载配置文件指示的语料库目录下所有文件
    map<string, string> &confMap = _conf.getConfigMap();
    string dir = confMap["corpus-en"];
    getFiles(dir, _files);

    //2、创建词典
    cout << "创建英文词典..." << endl;
    buildEnDict();

    //3、保存词典文件
    string filepath = confMap["dict-en"];
    cout << "保存英文词典..." << endl;
    storeDict(filepath);

    //4、创建索引
    cout << "创建英文词典索引..." << endl;
    buildIndex(filepath);

    //5、保存索引文件
    cout << "保存英文词典索引..." << endl;
    filepath = confMap["index-en"];
    storeIndex(filepath);
}

DictProducer::DictProducer(Configuration & conf, SplitTool *splitTool)   //构造函数,专为中文处理
: _conf(conf)
, _splitTool(splitTool)
{
    //1、加载配置文件指示的语料库目录下所有文件
    map<string, string> &confMap = _conf.getConfigMap();
    string dir = confMap["corpus-cn"];
    getFiles(dir, _files);

    //2、创建词典
    cout << "创建中文词典..." << endl;
    bulidCnDict();

    //3、保存词典文件
    cout << "保存中文词典..." << endl;
    string filepath = confMap["dict-cn"];
    storeDict(filepath);

    //4、创建索引
    cout << "创建中文词典索引..." << endl;
    buildIndex(filepath);

    //5、保存索引文件
    cout << "保存中文词典索引..." << endl;
    filepath = confMap["index-cn"];
    //cout << "filepath in chinese" << filepath << endl;
    storeIndex(filepath);
}


DictProducer::~DictProducer(){
    if(_splitTool){
        delete _splitTool;
    }
}

void DictProducer::buildDict(){
    //获取stopWordList
    set<string> stopWordList = _conf.getStopWordList();

    for(auto &file_it : _files){
        ifstream ifs(file_it, std::ios::ate);   //打开文件
        if(!ifs.good()){
            cerr << "ifs open file error\n";
            return;
        }

        //一次性读取一篇文章
        size_t length = ifs.tellg();
        ifs.seekg(std::ios::beg);
        char *buff = new char[length + 1];
        ifs.read(buff, length + 1);
        string wholeArticle(buff);

        //cout << "wholeArticle size: " << wholeArticle.size() << endl;

        vector<string> results = _splitTool->cut(wholeArticle);
        
        for(auto &res_it : results)
        {
            if(res_it.length() != 0){  //过滤空字符串
                set<string>::iterator stop_it = stopWordList.find(res_it);
                if(stop_it == stopWordList.end() && (getByteNum_UTF8(res_it[0] == 3) || getByteNum_UTF8(res_it[0] == 1))){     //过滤停词库
                    ++_dict[res_it];
                }
            }
        }
        ifs.close(); 
        delete []buff;
    }
}


void DictProducer::buildEnDict()     //创建英文字典
{
    //通过配置类获取stopWordList
    set<string> stopWordList = _conf.getStopWordList();

    for(auto &file_it : _files){
        ifstream ifs;
        ifs.open(file_it);
        if(!ifs.good()){
            cerr << "ifs open file error\n";
            return;
        }

        string text;
        while(getline(ifs, text)){  //对文件中逐行读取
            
            istringstream line(text);
            string word;
            while(line >> word){
                word = cleanup_str(word);   //对单词进行过滤，并转换为小写
                if(word.length() != 0){  //过滤空字符串
                    //过滤掉stopWord
                    set<string>::iterator stop_it = stopWordList.find(word);
                    if(stop_it != stopWordList.end()){ //命中
                        continue;
                    }else{
                        ++_dict[word];  
                    }
                } 
            }
        }
        ifs.close(); 
    }
}


void DictProducer::bulidCnDict()     //创建中文字典
{
    //获取stopWordList
    set<string> stopWordList = _conf.getStopWordList();

    for(auto &file_it : _files){
        ifstream ifs(file_it, std::ios::ate);   //打开文件
        if(!ifs.good()){
            cerr << "ifs open file error\n";
            return;
        }

        //一次性读取一篇文章
        size_t length = ifs.tellg();
        ifs.seekg(std::ios::beg);
        char *buff = new char[length + 1];
        ifs.read(buff, length + 1);
        string wholeArticle(buff);

        //cout << "wholeArticle size: " << wholeArticle.size() << endl;

        vector<string> results = _splitTool->cut(wholeArticle);
        
        for(auto &res_it : results)
        {
            if(res_it.length() != 0){  //过滤空字符串
                set<string>::iterator stop_it = stopWordList.find(res_it);
                if(stop_it == stopWordList.end() && getByteNum_UTF8(res_it[0] == 3)){     //过滤停词库
                    ++_dict[res_it];
                }
            }
        }
        ifs.close(); 
        delete []buff;
    }
}    


void DictProducer::buildIndex(const string &filepath)      //创建索引
{
    
    //先加载词典文件里的数据至词典vector中
    ifstream ifs;
    ifs.open(filepath);
    if(!ifs.good()){
        cerr << "DictProducer::buildIndex(const string &) open file error\n";
        return;
    }

    string line, word;
    int frequency;
    int i = 0;  //记录的是词典vector的下标
    while(getline(ifs, line)){
        istringstream iss(line);
        iss >> word;

        size_t charNums = word.size() / getByteNum_UTF8(word[0]);
        for(size_t idx = 0, n = 0; n != charNums; ++idx, ++n)
        {
            size_t charLen = getByteNum_UTF8(word[idx]);
            string subWord = word.substr(idx, charLen);
            _index[subWord].insert(i);
            idx += (charLen - 1);
        }
        ++i;
    }
}


void DictProducer::storeDict(const string &filepath)   //将词典写入文件
{

    ofstream ofs;
    ofs.open(filepath, std::ios::out | std::ios::trunc);
    if(!ofs.good()){
        cerr << "ofstream open file error\n";
        return;
    }
    for(auto &elem : _dict){
        ofs << elem.first << " " << elem.second << endl;
    }
    ofs.close();
}

void DictProducer::storeIndex(const string &filepath){
    ofstream ofs;
    ofs.open(filepath, std::ios::out | std::ios::trunc);
    if(!ofs.good()){
        cerr << "ofstream open file error\n";
        return;
    }
    for(auto &elem : _index){
        ofs << elem.first << " ";
        for(auto &it : elem.second){
            ofs << it << " ";
        }
        ofs << endl;
    }
    ofs.close();
}

void DictProducer::showFiles() const     //查看文件路径，作为测试用
{
    cout << "语料库文件的绝对路径集合: " << endl;
    for(auto &it : _files){
        cout << it << endl;
    }
}

void DictProducer::showDict() const      //查看词典，作为测试用
{
    
    for(auto &it : _dict){
        cout << it.first << " " << it.second << endl;
    }
    cout << "英文词典: size: " << _dict.size() << endl;
}


void DictProducer::getFiles(string path, vector<string> &files)    //获取文件的绝对路径,会递归遍历
{
    DIR *dir;
    struct dirent *ptr;

    if ((dir = opendir(path.c_str())) == NULL)
    {
        cerr << "Open dir error...\n";
        return;
    }

    /*
     * 文件(8)、目录(4)
     */

    while ((ptr = readdir(dir)) != NULL)
    {
        if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0)
            continue;
        else if (ptr->d_type == 8)
            files.push_back(path + ptr->d_name);
        else if (ptr->d_type == 4)
        {
            //files.push_back(ptr->d_name);
            getFiles(path + ptr->d_name + "/", files);
        }
    }
    closedir(dir);
}



string DictProducer::cleanup_str(const string &word) //针对英文单词大写转小写
{
    string ret;
    for(auto it = word.begin(); it != word.end(); ++it){
        if(isalpha(*it)){
            ret += tolower(*it);
        }
    }
    return ret;
}


size_t DictProducer::getByteNum_UTF8(const char byte)
{
    int byteNum = 0;
    for(size_t i = 0; i < 6; ++i)
    {
        if(byte & (1 << (7 - i))){
            ++byteNum;
        }else{
            break;
        }
    }
    return byteNum == 0 ? 1 : byteNum;
}

