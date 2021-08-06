#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<int> vec({1,2,3});
    const vector<int>::iterator iter = vec.begin();
    *iter = 10;
    // ++iter;  // error

    vector<int>::const_iterator clter = vec.begin();
    ++clter;
    // *clter = 10;    // error, read-only
    return 0;
}