#include <iostream>
#include <list>
#include <algorithm>

template <class T>
class Set
{
public:
    bool member(const T &item) const;
    void insert(const T &item);
    void remove(const T &item);
    std::size_t size() const;

private:
    std::list<T> rep;
};

template <typename T>
bool Set<T>::member(const T &item) const
{
    return std::find(rep.begin(), rep.end(), item) != rep.end();
}
template <typename T>
void Set<T>::insert(const T &item)
{
    if (!member(item))
        rep.push_back(item);
}
template <typename T>
void Set<T>::remove(const T &item)
{
    typename std::list<T>::iterator it = std::find(rep.begin(), rep.end(), item);
    if (it != rep.end())
        rep.erase(it);
}
template <typename T>
std::size_t Set<T>::size() const
{
    return rep.size();
}
int main()
{
    Set<int> mySet;
    mySet.insert(1);
    mySet.insert(2);
    mySet.insert(3);
    mySet.insert(2);
    mySet.remove(3);
    std::cout << mySet.size() << std::endl;
}