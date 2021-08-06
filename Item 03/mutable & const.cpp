#include <iostream>
#include <cstring>

class CTextBlock
{
public:
    CTextBlock(char *p): pText(p) {}
    std::size_t length() const;

private:
    char *pText;
    mutable std::size_t textLength;
    mutable bool lengthIsValid;
};

std::size_t CTextBlock::length() const
{
    if (!lengthIsValid)
    {
        std::cout << "First time" << std::endl;
        textLength = std::strlen(pText);
        lengthIsValid = true;
    }
    return textLength;
}

int main()
{
    char s[] = "Hello";
    const CTextBlock ctb(s);
    std::cout << ctb.length() << std::endl;
    std::cout << ctb.length() << std::endl;
}