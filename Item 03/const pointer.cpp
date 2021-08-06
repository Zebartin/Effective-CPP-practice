#include <iostream>
using namespace std;

int main()
{
    char greeting[] = "Hello";
    char *p0 = greeting;             // non-const pointer, non-const data
    const char *p1 = greeting;       // non-const pointer, const data
    char const *p2 = greeting;       // non-const pointer, const data
    char *const p3 = greeting;       // const pointer, non-const data
    const char *const p4 = greeting; // const pointer, const data
    
    // *p0 = 'B';  // OK
    // *p1 = 'C';  // assignment of read-only location
    // *p2 = 'D';  // assignment of read-only location
    // *p3 = 'F';  // OK
    // *p4 = 'G'   // assignment of read-only location
    cout << greeting << endl;
}