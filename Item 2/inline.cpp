#include <iostream>
using namespace std;

#define CALL_WITH_MAX(a, b) f((a) > (b) ? (a) : (b))

int f(int x)
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
    cout << CALL_WITH_MAX(++a, b) << endl;
    cout << a << endl;
    cout << CALL_WITH_MAX(++a, b + 10) << endl;
    cout << a << endl;
    cout << callWithMax(++a, b) << endl;
    cout << a << endl;
}