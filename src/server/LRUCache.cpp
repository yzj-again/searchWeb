#include "../../include/server/LRUCache.hh"

/**
 * @brief 未完成部分，没有添加到最终模块中
 * 
 * @param capacity 
 */

LRUCahe::LRUCahe(int capacity)
: _capacity(capacity)
{

}

LRUCahe::~LRUCahe()
{

}

// 往 LRUCache 中添加数据
void LRUCahe::addElement(const string& key, const ElemType& value)
{
    // 假如说 LRU 缓存中没有这个要添加的数据
    if (!_hashMap.count(key))
    {
        // 而且缓存已经满了，此时则需要淘汰最久没有使用的数据
        if (_pendingUpdateList.size() == _capacity)
        {
            _hashMap.erase(_resultList.back().first);
            _pendingUpdateList.pop_back();
        }
    }
    else  // 缓存中有这个要添加的数据
    {
        _pendingUpdateList.erase(_hashMap.at(key));
    }

    // 将元素加入链表头部
    _pendingUpdateList.push_front(make_pair(key, value));
    _hashMap[key] = _pendingUpdateList.begin();
}

// 一般用于系统启动的时候从缓存中读取之前保存的持久化数据
void LRUCahe::readFromFile(const string& filename)
{
    ifstream ifs;
    ifs.open(filename, ios::in);
    if (!ifs.good())
    {
        cerr << ">> int read: open LRUCahe file failed" << endl;
    }

    // 逐行读取数据
    string cacheData;
    while (getline(ifs, cacheData))
    {
        string key;
        ElemType value;
        istringstream iss(cacheData);
        if (iss) { iss >> key; }
        if (iss) { iss >> value; }
        _resultList.push_back(make_pair(key, value));
    }
    ifs.close();
}

void LRUCahe::writeToFile(const string& filename)
{
    ofstream ofs;
    ofs.open(filename);
    if (!ofs.good(), ios::out)
    {
        cerr << ">> in write: open LRUCache file failed" << endl;
        return;
    }
    for (list<pair<string, ElemType>>::iterator it = _pendingUpdateList.begin();
         it != _pendingUpdateList.end(); ++it)
    {
        ofs << (*it).first << " " << (*it).second << endl;
    }       
}
