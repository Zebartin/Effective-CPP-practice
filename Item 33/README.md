# Item 33: Avoid hiding inherited names.

```cpp
class Base
{
private:
    int x;

public:
    virtual void mf1() = 0;
    virtual void mf1(int);
    virtual void mf2();
    void mf3();
    void mf3(double);
    ...
};
class Derived : public Base
{
public:
    virtual void mf1();
    void mf3();
    void mf4();
    ...
};
```

在上述代码中，`Derived`的`mf1`和`mf3`会把`Base`中的同名函数完全隐藏，哪怕参数不同，哪怕是虚函数或不是。

## 避免默认的名称隐藏

这种设计是为了避免从遥远的基类继承意料之外的重载函数。然而，希望能继承重载函数的情况更常见，尤其是使用public继承的时候，继承所有函数是必要的。此时需要声明相应的`using`，重写C++对继承名称的默认隐藏：

```cpp
class Derived : public Base
{
public:
    using Base::mf1;
    using Base::mf3;
    virtual void mf1();
    void mf3();
    void mf4();
    ...
};
```

## 利用默认的名称隐藏

有时候会希望不继承基类中的所有函数。这在private继承的情况下是很合理的。比如说`Derived`只想继承`Base`中不带参数的`mf`，可以通过转发函数来取巧：

```cpp
class Derived : private Base
{
public:
    virtual void mf1() { Base::mf1(); }
    ...
};
```

这个函数是隐式内联的，见Item 30。此时其他版本的`mf1`就被隐藏了。

内联的转发函数也可以用来对付不支持`using`方式的古旧编译器。

如果涉及到模板，就是另一个全新的故事了，见Item 43。

## 总结

- 派生类里的名称会隐藏基类里的名称。在public继承时，这种情况是不希望看到的；
- 要让被隐藏的名称重新可见，使用`using`进行声明，或者使用转发函数（forwarding functions）。
