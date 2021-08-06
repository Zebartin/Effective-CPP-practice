#include <iostream>

template <typename T>
class NamedObject
{
public:
    NamedObject(std::string &name, const T &value) : nameValue(name), objectValue(value) {}

private:
    std::string &nameValue;
    const T objectValue;
};

int main()
{
    std::string newDog("Persephone");
    std::string oldDog("Satch");

    NamedObject<int> p(newDog, 2);
    NamedObject<int> pa(p);         // OK
    NamedObject<int> s(oldDog, 36);

    p = s;  // error, can’t use default assignment operator
    return 0;
}
