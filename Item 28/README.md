# Item 28: Avoid returning “handles” to object internals.

## 提高封装性

有以下一段代码：

```cpp
struct RectData {
    Point ulhc;
    Point lrhc;
};
class Rectangle {
public:
    Point& upperLeft() const { return pData->ulhc; }
    Point& lowerRight() const { return pData->lrhc; }
    ...
private:
    std::tr1::shared_ptr<RectData> pData;
};
```

`upperLeft`和`lowerRight`提供了类内部的handle，使得`pData`实质上是`public`的。

不只是引用，这样的handle还包括指针和迭代器。

类内部不仅包括私有的数据，也包括私有的成员函数，返回私有的函数指针也是不可取的（尽管比较罕见）。

## Dangling Handles

上面的例子可以将返回值的类型改为`const Point &`，从而避免通过handle来修改内部数据的问题。但这样仍然会产生dangling handles的问题。比如：

```cpp
class GUIObject { ... };
const Rectangle boundingBox(const GUIObject& obj);

GUIObject *pgo;
const Point *pUpperLeft = &(boundingBox(*pgo).upperLeft());
```

最后一个语句中，生成了一个临时`Rectangle`对象，以及一个`Point`类型的handle。但在这个语句执行结束后，这个临时对象会被销毁，随之这个handle指向的内容也会被销毁，变成了dangling handle。

所以返回指向对象内部的handle是危险的，不论它是引用、指针还是迭代器，无论有没有被`const`修饰。只要你这么做了，你就得承担这个handle可能比它指向的对象还要长寿的风险。

这并不意味着永远不能让成员函数返回handle。比如，`operator[]`通常就会返回容器数据的引用。

## 总结

避免返回对象内部的handles（引用、指针或者迭代器）。这样提高了封装性，帮助`const`成员函数表现得`const`，并防止产生dangling handles。

<a href="../Item%2027"><-</a>

<a href="../Item%2029">-></a>
