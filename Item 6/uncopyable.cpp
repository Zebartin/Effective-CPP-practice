#include <iostream>

class Uncopyable
{
protected:
    Uncopyable() {}
    ~Uncopyable() {}

private:
    Uncopyable(const Uncopyable &);
    Uncopyable &operator=(const Uncopyable &);
};

class HomeForSale : private Uncopyable
{
};

int main()
{
    HomeForSale h1;
    HomeForSale h2(h1); // error, declared private
}
