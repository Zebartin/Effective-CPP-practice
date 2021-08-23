# Item 37: Never redefine a function’s inherited default parameter value.

## 虚函数默认参数的问题

```cpp
class Shape
{
public:
    enum ShapeColor { Red, Green, Blue };
    virtual void draw(ShapeColor color = Red) const = 0;
    ...
};

class Rectangle : public Shape
{
public:
    virtual void draw(ShapeColor color = Green) const;
    ...
};
```

如[shape.cpp](./shape.cpp)所示，当通过static type为`Shape *`、dynamic type为`Rectangle *`的指针调用`draw`时，会调用`Rectangle::draw`，但使用的默认参数是`Shape::Red`。

C++使用这种看似不合理的设定，是为了节省运行时的开销，让默认参数在编译时期就确定好。

## 解决方式

### 把默认参数值改成一样的

这样做，一方面导致代码重复，另一方面导致代码依赖：如果`Shape`中的默认参数需要改动，那所有派生类也必须随之改动。

### 不直接使用虚函数

考虑使用Item 35中提及的替代虚函数的方式，比如NVI idiom。让public的非虚函数处理默认参数值，而private的虚函数处理真正的工作。

## 总结

永远不要重新定义继承下来的默认参数值，因为默认参数值是statically bound的，而虚函数——唯一可以重新定义的函数——是dynamically bound的。
