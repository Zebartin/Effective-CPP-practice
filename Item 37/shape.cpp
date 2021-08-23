#include <iostream>
using namespace std;

class Shape
{
public:
    enum ShapeColor { Red, Green, Blue};
    virtual void draw(ShapeColor color = Red) const = 0;
    void nviDraw(ShapeColor color = Red) const
    {
        doDraw(color);
    }
private:
    virtual void doDraw(ShapeColor color) const = 0;
};

class Rectangle : public Shape
{
public:
    virtual void draw(ShapeColor color = Green) const;
private:
    virtual void doDraw(ShapeColor color) const;
};

void Shape::draw(ShapeColor color) const
{
    cout << "Shape::draw is called" << endl;
    if (color == Red)
        cout << "Color is red" << endl;
}

void Rectangle::draw(ShapeColor color) const
{
    cout << "Rectangle::draw is called" << endl;
    if (color == Red)
        cout << "Color is red" << endl;
}

void Shape::doDraw(ShapeColor color) const
{
    cout << "Shape::doDraw is called" << endl;
}

void Rectangle::doDraw(ShapeColor color) const
{
    cout << "Rectangle::doDraw is called" << endl;
}

int main()
{
    Shape *pr = new Rectangle;
    pr->draw();
    pr->nviDraw();
    return 0;
}