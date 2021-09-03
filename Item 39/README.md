# Item 39: Use private inheritance judiciously.

## private继承的含义

### private继承的行为表现

- 编译器通常不会把派生类对象转化为基类对象；
- 从基类继承的成员会统统变成派生类中的private成员。

### private继承的含义

private继承意味着is-implemented-in-terms-of。它完全是一个实现技巧，不继承接口，只继承实现。private继承对软件设计毫无意义，只在软件实现方面有所意义。

## 与组合比较

大多数情况下，因为组合更容易理解，使用组合是更合适的选择，除了以下两种情况。

### 较为普遍的情况

当一个类需要访问另一个类的protected成员时，或者需要重新定义虚函数，而两者又不是is-a关系，此时使用private继承比较合适，比如：

```cpp
class Widget : private Timer
{
private:
    virtual void onTick() const;
    ...
};
```

此时实际上是可以用组合来代替的：

```cpp
class Widget
{
private:
    class WidgetTimer : public Timer
    {
    public:
        virtual void onTick() const;
        ...
    };
    WidgetTimer timer;
    ...
};
```

这种实现比较复杂，但也有不得不使用它的场景：

- 需要阻止`Widget`的派生类重新定义`onTick`，类似于C#和Java中的`final`关键字；
- 需要尽可能减少`Widget`的编译依赖。private继承势必会造成`Widget`对`Timer`的依赖。

### 边缘情况：EBO

空类，也就是没有非静态数据、虚函数、虚基类的类，从概念上说大小应该是0。但是C++规定单独的对象的大小必须不为0，比如下面的`Empty e`大小不为0：

```cpp
class Empty {};
class HoldsAnInt
{
private:
    int x;
    Empty e;
};
```

而对于不单独的对象，比如作为基类被继承，派生类中基类部分的大小就为0。这种优化被称作空基类优化（empty base optimization，EBO）。

```cpp
class HoldsAnInt : private Empty
{
private:
    int x;
};
```

而大多数类都不是空的，所以这种情况事实上很少见。

## 总结

- private继承意味着is-implemented-in-terms-of。通常它都不如组合（composition）合适，但是当一个派生类需要访问基类的protected成员时，或者需要重新定义虚函数时，private继承也很合适；
- 不像组合，private继承可以实现空基类优化（empty base optimization）。这对想要尽可能减小对象大小的库开发者来说比较重要。
