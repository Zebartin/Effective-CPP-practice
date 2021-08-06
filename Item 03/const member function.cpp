#include <iostream>
using namespace std;

class TextBlock
{
public:
    TextBlock(string s) : text(s) {}
    const char &operator[](std::size_t position) const
    {
        cout << "const [] is called" << endl;
        return text[position];
    }
    char &operator[](std::size_t position)
    {
        cout << "non-const [] is called" << endl;
        return const_cast<char &>(
            static_cast<const TextBlock &>(*this)[position]);
    }

private:
    string text;
};

void print(const TextBlock &ctb)
{
    // ctb[0] = 'G';   // error
    cout << ctb[0] << endl;
}

int main()
{
    TextBlock tb("Hello");
    tb[0] = 'M';
    cout << tb[0] << endl;
    print(tb);
}