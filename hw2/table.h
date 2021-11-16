#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <string>
#include <vector>

using namespace std;

enum EntryType
{
    ACTIVE,
    EMPTY,
    DELETED
};

template <class T1, class T2>
struct Entry
{
    T1 key;
    T2 value;
    EntryType status;
    Entry() : status(EMPTY) {};
    Entry(T1 k, T2 v, EntryType s) : key(k), value(v), status(s) {};
};

template <class T1, class T2>
class HashTable
{
public:
    explicit HashTable(const T2 & NOT_FOUND) : NOT_FOUND(NOT_FOUND)
    {
        vector<Entry<T1, T2>> *temp = new vector<Entry<T1, T2>> (SIZE);
        table = *temp;
    };
    void clear();
    T2 get(const T1& key) const;
    void set(const T1& key, const T2& value);
    void unset(const T1& key);

private:
    vector<Entry<T1, T2>> table;

    const int SIZE = 4096;
    const T2 NOT_FOUND;
    int hash(const T1& key) const;
    bool isActive(int currentPos) const;
    int findPos(const T1& key) const;
};

#include "table.cpp"
#endif
