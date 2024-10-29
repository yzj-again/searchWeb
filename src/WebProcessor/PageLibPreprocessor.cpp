#include "../../include/WebProcessor/PageLibPreprocessor.hh"

#include <iostream>

PageLibPreprocessor::PageLibPreprocessor(Configuration& conf)
: _config(conf.getConfigMap())
{
    /* 通过配置文件类对象初始化一个map<string, string>容器 *
     * 其中key为文件名，value为文件路径 */    
}

// 根据配置信息读取网页库和偏移库的内容
void PageLibPreprocessor::readInfoFromFile()
{
    // 打开网页库文件
    cout << "加载网页库..." << endl;
    string webpagePath = _config.at("webpage"); 
    ifstream ifs;
    ifs.open(webpagePath, ios::in);
    if (!ifs.good())
    {
        std::cerr << ">> webpage open failed" << std::endl;
        return;
    }

    // 将网页库装入容器
    string doc;
    while (std::getline(ifs, doc))
    {
        WebPage page(doc, *(Configuration::getInstance()), _jieba);
        _pageList.push_back(page);
    }
    ifs.close();

    // 将网页偏移库装入容器;
    cout << "加载网页偏移库..." << endl;
    ifstream offsetIfs;
    string offsetFilePath = _config.at("newoffset");
    offsetIfs.open(offsetFilePath, ios::in);
    if (!offsetIfs.good())
    {
        cerr << ">> offset data load failed" << endl;
        return;
    }
    string offsetData;
    while (std::getline(offsetIfs, offsetData))
    {
        int docId;
        int startPosition;
        int docSize;
        istringstream iss(offsetData);
        if (iss >> docId)
            ;
        else 
        {
            cerr << ">> docId load failed" << endl;
            return;
        }
        if (iss >> startPosition)
            ;
        else
        {
            cerr << ">> startPosition load failed" << endl;
            return;
        }
        if (iss >> docSize)
            ;
        else
        {
            cerr << ">> docSize load failed" << endl;
            return;
        }
        _offsetLib[docId] = make_pair(startPosition, docSize);
    }
    offsetIfs.close();
}


// 使用Simhasher对网页进行去重处理
void PageLibPreprocessor::cutRedundantPages()
{
    cout << "网页库去重..." << endl;
    cout << "去重前文章数量: " << _pageList.size() << endl;
    // 外层循环作为对比基准 
    for (list<WebPage>::iterator tarPos = _pageList.begin(); 
         tarPos != _pageList.end(); ++tarPos)
    {
        list<WebPage>::iterator pit = tarPos;
        // 内层循环去除所有与外层基准相同的文档
        if (++pit != _pageList.end())
        {
            // 定位到对比基准迭代器的下一个位置
            for (list<WebPage>::iterator it = pit; it != _pageList.end(); )
            {
                if ((*tarPos) == (*it))
                {
                    _offsetLib.erase((*it).getDocId());
                    _pageList.erase(it++);
                }
                else it++;
            }
        }
    }
    cout << "去重后文章数量: " << _pageList.size() << endl;
}

void PageLibPreprocessor::buildInvertIndexTable()
{
    cout << "建立倒排索引库..." << endl;
    // 整个网页库的文档数量
    long wholeDocNum = _pageList.size();
    
    // 01 建立总词频统计
    map<string, int> documentFrequency;
    for (auto it = _pageList.begin(); it != _pageList.end(); ++it)
    {
        // 获取文档的词频统计
        // 使用unorder_map用迭代器无法遍历完毕
        map<string, int> curDocFrequency = (*it).getWordsMap();

        // 遍历文档的词频统计
        // 存入dfmap
        for (auto mapIt = curDocFrequency.begin(); mapIt != curDocFrequency.end(); ++mapIt)
        {
            string key = (*mapIt).first;
            documentFrequency[key] += (*mapIt).second;
        }
    }
    
    // 建立每个文章每个单词的权重库
    map<int, vector<pair<string, double>>> weightMap;
    for (auto it = _pageList.begin(); it != _pageList.end(); ++it)
    {
        int docId = (*it).getDocId();

        // 获取某篇文章某个单词的词频
        map<string, int> curDocFrequency = (*it).getWordsMap();
        for (auto mapIt = curDocFrequency.begin(); mapIt != curDocFrequency.end(); ++mapIt)
        {
            string word = (*mapIt).first;  // 某个词在文章中出现的次数
            int termFrequency = (*mapIt).second;  // 某个词在所有文章中出现的次数
            double inverseDocFreq = log2(wholeDocNum / documentFrequency.at(word) + 1);  // 逆文档率 
            double weight = termFrequency * inverseDocFreq;  // 权重 
            weightMap[docId].push_back(make_pair(word, weight));
        }
    }

    // 归一化处理
    for (auto it = weightMap.begin(); it != weightMap.end(); ++it)
    {
        // 计算此文档的归一量
        double wholeFreq = 0.0;
        for (auto& freq : (*it).second)
        {
            wholeFreq += freq.second * freq.second;
        }
        wholeFreq = sqrt(wholeFreq);

        // 计算此文档的倒排索引权重值
        for (auto& freq : (*it).second)
        {
            // 计算最终权重值并保留到成员变量中
            double realWeight = freq.second / wholeFreq;
            _invertIndexTable[freq.first].push_back(make_pair(it->first, realWeight));
        }
    }
}

// 将倒排索引表、去重后的网页库和网页偏移库存入本地磁盘
void PageLibPreprocessor::storeOnDisk()
{
    // 存入去重后的网页偏移库
    ofstream newOffsetOfs;
    newOffsetOfs.open(_config.at("newoffset"), ios::out);
    if (!newOffsetOfs.good())
    {
        cerr << ">> newoffset data file open failed" << endl;
        return;
    }
    for (unordered_map<int, pair<int, int>>::iterator it = _offsetLib.begin();
         it != _offsetLib.end(); ++it)
    {
        newOffsetOfs << (*it).first << " " << (*it).second.first << " "
             << (*it).second.second << endl;
    }
    cout << "网页偏移库更新成功！" << endl;
    newOffsetOfs.close();

    // 存入倒排索引库
    string invertindex = _config.at("invertindex");
    ofstream ofs;
    ofs.open(invertindex, ios::out);
    if (!ofs.good())
    {
        cerr << ">> invertindex open failed" << endl;
        return;
    }
    for (auto& mapElem : _invertIndexTable)
    {
        ofs << mapElem.first << " ";
        for (auto& vecElem : mapElem.second)
        {
            ofs << vecElem.first << " " << vecElem.second << " ";
        }
        ofs << endl;
    }
    //cout << "倒排索引库写入成功！" << endl;
    ofs.close();
}

void PageLibPreprocessor::doProcess()
{
    readInfoFromFile();
    cutRedundantPages();
    buildInvertIndexTable();
    storeOnDisk();
}
