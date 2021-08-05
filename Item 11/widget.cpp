#include <iostream>

class Bitmap
{
private:
    std::string message;

public:
    Bitmap(const std::string s) : message(s) {}
    void print()
    {
        std::cout << message << std::endl;
    }
};

class Widget
{
public:
    Bitmap *pb;
    Widget(Bitmap *bm) : pb(bm) {}
    void print()
    {
        pb->print();
    }
};

class UnsafeWidget : public Widget
{
public:
    UnsafeWidget(Bitmap *bm) : Widget(bm) {}
    UnsafeWidget &operator=(const UnsafeWidget &rhs)
    {
        delete pb;
        pb = new Bitmap(*rhs.pb);
        return *this;
    }
};

class SafeWidget : public Widget
{
public:
    SafeWidget(Bitmap *bm) : Widget(bm) {}
    SafeWidget &operator=(const SafeWidget &rhs)
    {
        Bitmap *pOrig = pb;
        pb = new Bitmap(*rhs.pb);
        delete pOrig;
        return *this;
    }
};

class AnotherSafeWidget : public Widget
{
private:
    void swap(Widget &rhs)
    {
        Bitmap *temp = pb;
        pb = rhs.pb;
        rhs.pb = temp;
    }

public:
    AnotherSafeWidget(Bitmap *bm) : Widget(bm) {}
    AnotherSafeWidget &operator=(Widget rhs)
    {
        swap(rhs);
        return *this;
    }
};

int main()
{
    UnsafeWidget uw(new Bitmap("Hello"));
    uw.print(); // Hello
    uw = uw;
    uw.print(); // Nothing?

    SafeWidget sw(new Bitmap("World"));
    sw = sw;
    sw.print();

    AnotherSafeWidget asw(new Bitmap("Zebartin"));
    asw = asw;
    asw.print();
}
