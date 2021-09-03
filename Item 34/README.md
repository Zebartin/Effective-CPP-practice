# Item 34: Differentiate between inheritance of interface and inheritance of implementation.

以下是三种不同的继承方式：

```cpp
class Shape
{
public:
    virtual void draw() const = 0;
    virtual void error(const std::string &msg);
    int objectID() const;
    ...
};
class Rectangle : public Shape { ... };
class Ellipse : public Shape { ... };
```

## 纯虚函数的继承

纯虚函数要求派生类继承它的接口，派生类必须为它提供实现。

纯虚函数可以有自己的函数实现，但唯一一种能调用它的方式是指明它对应的类名，比如`Shape::draw();`。这个特性通常来说没什么用，但可以用来为非纯普通虚函数提供安全的默认实现。

## 普通虚函数的继承

普通（非纯）虚函数要求派生类继承它的接口和默认实现。

使用普通虚函数可能是危险的。如果一个新的派生类继承了基类但忘了修改默认实现，编译器不会发出任何警告。可以将其改为纯虚函数+protected成员函数解决这个问题：

```cpp
class Airplane
{
public:
    virtual void fly(const Airport &destination) = 0;
    ...
protected :
    void defaultFly(const Airport &destination);
};
```

所有派生类都被要求为虚函数提供实现，其中使用默认实现的派生类可以通过protected成员函数来避免代码重复，使用特殊实现的派生类在缺乏相应实现的时候会编译失败。

事实上，如前所述，纯虚函数可以有自己的函数体。这样一来就不需要额外的`defaultFly`，派生类直接调用`Airplane::fly`即可。

## 非虚函数的继承

public继承下，派生类中的非虚函数行为应与基类一致。此时它要求派生类继承接口及其强制性实现。

## 两个常见错误

### 把所有函数都声明为非虚函数

除非想设计一个不用作基类的类，否则：

- 派生类没有特别之处
- 非虚析构函数会导致问题（[Item 7](../Item%2007)）

这种错误可能是出于担心虚函数的性能开销。这里要重提八二原则：在担心虚函数影响性能之前，先确定这是否与那真正有所影响的20%的代码有关系。

### 把所有函数都声明为虚函数

除非想设计一个Interface class，否则派生类需要重新定义所有函数。如果不需要，为什么不改成非虚函数呢？

## 总结

- 接口的继承与实现的继承是不同的。在public继承时，派生类总是继承基类的接口；
- 纯虚函数只指定了接口的继承；
- 非纯普通虚函数指定了接口的继承，以及接口的一个默认实现；
- 非虚函数指定了接口的继承，以及接口的一个强制性实现。
