#include "table.h"

#include <string>

template <class T1, class T2>
void HashTable<T1, T2>::clear() {};

template <class T1, class T2>
T2 HashTable<T1, T2>::get(const T1& key) const
{
    int currentPos = findPos(key);

    if (isActive(currentPos))
        return table[currentPos].value;

    return NOT_FOUND;
};

template <class T1, class T2>
void HashTable<T1, T2>::set(const T1& key, const T2& value)
{
    int currentPos = findPos(key);
    table[currentPos] = Entry<T1, T2>(key, value, ACTIVE);
};

template <class T1, class T2>
void HashTable<T1, T2>::unset(const T1& key)
{
    int currentPos = findPos(key);

    if (isActive(currentPos))
        table[currentPos].status = DELETED;
};

template <class T1, class T2>
int HashTable<T1, T2>::hash(const T1& key) const
{
    int hashVal = 0;

    for (int i = 0; i < key.length(); i++)
        hashVal = 37 * hashVal + key[i];

    hashVal = hashVal % SIZE;

    if (hashVal < 0)
        hashVal = hashVal + SIZE;

    return hashVal;
};

template <class T1, class T2>
int HashTable<T1, T2>::findPos(const T1& key) const
{
    int i = 0; // number of collusions

    int p = hash(key); // current position
    int initial = p;

    while (table[p].status != EMPTY && table[p].key != key)
    {
        p++; // add the difference
        if (initial == p)
            return -1;

        if (p >= SIZE)    // perform the mod
            p = p - SIZE; // if necessary
    }

    return p;
};

template <class T1, class T2>
bool HashTable<T1, T2>::isActive(int currentPos) const
{
    return table[currentPos].status == ACTIVE;
}
