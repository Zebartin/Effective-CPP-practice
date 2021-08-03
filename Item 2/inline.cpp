#include <iostream>
using namespace std;

#define CALL_WITH_MAX(a, b) f((a) > (b) ? (a) : (b))

template <typename T>
T f(T x)
{
    return x;
}

template <typename T>
inline int callWithMax(const T &a, const T &b)
{
    return f(a > b ? a : b);
}

int main()
{
    int a = 5, b = 0;
    cout << CALL_WITH_MAX(a, b) << endl;
    cout << CALL_WITH_MAX(++a, b + 10) << endl;
    cout << "a=" << a << endl;              // a=6
    cout << CALL_WITH_MAX(++a, b) << endl;
    cout << "a=" << a << endl;              // a=8
    cout << callWithMax(++a, b) << endl;
    cout << "a=" << a << endl;              // a=9
}